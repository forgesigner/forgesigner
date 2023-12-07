#include "InitialHintProvider.h"

#include <QFile>
#include <QDebug>
#include <QtLogging>

InitialHintProvider::InitialHintProvider() {
}

void InitialHintProvider::loadModel(const QString& pathToOnnxModel) {
    if (!QFile::exists(pathToOnnxModel)) {
        qWarning() << "couldn't load onnx model, file not found:" << pathToOnnxModel;
        m_isAvailable = false;
        return;
    }

    const char* pathToModel = pathToOnnxModel.toUtf8().constData();
    try {
        m_onnxSession = Ort::Session(m_onnxEnv, pathToModel, m_onnxSessionOptions);
        m_isAvailable = true;
    } catch (const Ort::Exception& e) {
        m_isAvailable = false;
        qWarning() << "couldn't load onnx model, InitialHintProvider unavailable,"
                   << "Ort::Exception::what() prints" << e.what();
    }
}

QList<QList<QPoint>> InitialHintProvider::provideHints(const QList<QPixmap> pages) const {
    std::vector<float> input_data = { /* your input data here */ };
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        m_onnxEnv, input_data.data(), input_data.size(), );
    return {};
}

// OutputTensor runModel(InputTensor inputTensor) {
void runModel() {
    // const char* inputNames[] = {"input"};
    // const char* outputNames[] = {"output"};
    // Ort::Value outputTensor = onnx_session.Run(runOptions, inputNames, &inputTensor, 1, outputNames, 1);
}
