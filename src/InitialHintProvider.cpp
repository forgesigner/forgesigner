#include "InitialHintProvider.h"

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QtLogging>

#include <algorithm>

namespace {
    /* Return nullopt if model unexpectedly has more than 1 input */
    std::optional<std::string> getInputName(const Ort::Session& session) {
        const auto inputCount = session.GetInputCount();
        if (inputCount != 1) {
            return std::nullopt;
        }
        const auto namePtr = session.GetInputNameAllocated(0, Ort::AllocatorWithDefaultOptions());
        return std::string(namePtr.get());
    }

    /* Return nullopt if model unexpectedly has more than 1 output */
    std::optional<std::string> getOutputName(const Ort::Session& session) {
        const auto outputCount = session.GetOutputCount();
        if (outputCount != 1) {
            return std::nullopt;
        }
        const auto namePtr = session.GetOutputNameAllocated(0, Ort::AllocatorWithDefaultOptions());
        return std::string(namePtr.get());
    }

    /* Return nullopt if model unexpectedly has more than 1 input */
    std::optional<const std::vector<int64_t>> getInputShape(const Ort::Session& session) {
        const auto inputCount = session.GetInputCount();
        if (inputCount != 1) {
            return std::nullopt;
        }
        return session.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    }

    /* Return nullopt if model unexpectedly has more than 1 output */
    std::optional<const std::vector<int64_t>> getOutputShape(const Ort::Session& session) {
        const auto outputCount = session.GetOutputCount();
        if (outputCount != 1) {
            return std::nullopt;
        }
        return session.GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
    }
}

InitialHintProvider::InitialHintProvider() {
}

void InitialHintProvider::loadModel(const QString& pathToOnnxModel) {
    if (!QFile::exists(pathToOnnxModel)) {
        qWarning() << "couldn't load onnx model, file not found:" << pathToOnnxModel;
        m_isAvailable = false;
        return;
    }

    try {
        m_onnxSession = Ort::Session(m_onnxEnv, pathToOnnxModel.toUtf8().constData(), m_onnxSessionOptions);
        m_isAvailable = true;
    } catch (const Ort::Exception& e) {
        m_isAvailable = false;
        qWarning() << "couldn't load onnx model, InitialHintProvider unavailable,"
                   << "Ort::Exception::what() prints" << e.what();
    }
}

QList<QPoint> InitialHintProvider::provideHintsForSinglePage(QImage page) {
    if (!isAvailable()) {
        return {};
    }
    if (page.isNull()) {
        qCritical() << "page is null";
        return {};
    }

    // Step 1: rescale QImage to model input size
    const auto inputShapeOpt = getInputShape(m_onnxSession);
    if (!inputShapeOpt || inputShapeOpt->size() != 4) {
        return  {};
    }
    const auto& inputShape = inputShapeOpt.value();
    const auto batchSize = inputShape[0];
    // TODO: why aren't we using grayscale in the model?
    // Answer: because torchvision.models.resnet18 backbone expects 3 input channels.
    const auto numChannels = inputShape[1];
    assert(numChannels == 3);
    const auto height = inputShape[2];
    const auto width = inputShape[3];
    page = page.scaled(width, height, Qt::IgnoreAspectRatio);

    // Step 2: ensure 32-bit RGB format (0xffRRGGBB).
    page = page.convertToFormat(QImage::Format_RGB32);
    const size_t bytesPerPixel = 4;
    const size_t oneChannelSize = height * width;

    // Step 3: convert QImage to raw data for Ort input tensor.
    const auto inputSize = batchSize * numChannels * oneChannelSize;
    // Think of this as (C, H, W) pytorch tensor, to which torch.flatten has been applied
    std::vector<float> rawInput(inputSize, 0);

    for (size_t h = 0; h < height; ++h) {
        for (size_t w = 0; w < width; ++w) {
            const size_t channelIndex = h * width + w;
            const auto pixel = page.pixel(h, w);

            rawInput[channelIndex] = qRed(pixel);
            rawInput[channelIndex + oneChannelSize] = qGreen(pixel);
            rawInput[channelIndex + 2 * oneChannelSize] = qBlue(pixel);
        }
    }

    auto normalizeToZeroOne = [](auto x) {
        return x / 255.0;
    };
    std::transform(rawInput.begin(), rawInput.end(), rawInput.begin(), normalizeToZeroOne);

    auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, rawInput.data(), rawInput.size(), inputShape.data(), inputShape.size()
    );

    // Step 4: run the model on the input tensor
    const auto inputNamesOpt = getInputName(m_onnxSession);
    if (!inputNamesOpt) {
        return {};
    }
    const auto inputNames = inputNamesOpt.value().c_str();

    const auto outputNamesOpt = getOutputName(m_onnxSession);
    if (!outputNamesOpt) {
        return {};
    }
    const auto outputNames = outputNamesOpt.value().c_str();

    const auto allOutputs = m_onnxSession.Run(
        Ort::RunOptions{}, &inputNames, &inputTensor, 1, &outputNames, 1
    );
    if (allOutputs.size() == 0) {
        return {};
    }
    const auto& outputTensor = allOutputs[0];

    // Step 5: interpret outputTensor as 2D grayscale heatmap, extract argmax coordinates
    // TODO: currently supports only single point, use a smarter algo to output N hints.
    const float* const rawOutput = outputTensor.GetTensorData<float>();
    const size_t outputSize = outputTensor.GetTensorTypeAndShapeInfo().GetElementCount();
    if (outputSize != oneChannelSize) {
        return {};
    }
    const float* const maxElementPtr = std::max_element(rawOutput, rawOutput + outputSize);
    const size_t argmaxIndex = std::distance(rawOutput, maxElementPtr);

    const size_t argmaxHeightIndex = argmaxIndex / width;
    const size_t argmaxWidthIndex = argmaxIndex % width;

    return {QPoint(argmaxWidthIndex, argmaxHeightIndex)};
}
