#include <onnxruntime/onnxruntime_cxx_api.h>
// #include <onnxruntime/experimental_onnxruntime_cxx_api.h>

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <numeric>
#include <optional>

void printOutputToFile(const auto& data, const std::string& file_path) {
    std::ofstream output_file(file_path);
    if (!output_file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    for (const auto& value : data) {
        output_file << value << " ";
    }
}

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

int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel");

    const std::string modelPath = "bottom_right_pixel_model.onnx";

    Ort::Session session(env, modelPath.c_str(), Ort::SessionOptions{});

    auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    const auto singleInputName = getInputName(session).value();
    const char* const inputNames[] = {singleInputName.c_str()};
    const auto inputShape = getInputShape(session).value();
    const int64_t inputSize = std::accumulate(inputShape.begin(), inputShape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> rawInput(inputSize, 0);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, rawInput.data(), rawInput.size(), inputShape.data(), inputShape.size()
    );

    const auto singleOutputName = getOutputName(session).value();
    const char* const outputNames[] = {singleOutputName.c_str()};
    const auto outputShape = getOutputShape(session).value();
    const int64_t outputSize = std::accumulate(outputShape.begin(), outputShape.end(), 1, std::multiplies<int64_t>());
    std::vector<float> rawOutput(outputSize, 0);
    Ort::Value outputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, rawOutput.data(), rawOutput.size(), outputShape.data(), outputShape.size()
    );

    session.Run(Ort::RunOptions{}, inputNames, &inputTensor, 1,
                             outputNames, &outputTensor, 1);
    
    // Write tensor to disk, then examine the output visually in Python, for simplicity
    printOutputToFile(rawOutput, "onnx_model_raw_output.txt");
}
