//TODO:
#include "InitialHintProvider.h"

#include <onnxruntime/onnxruntime_cxx_api.h>
// #include <onnxruntime/cpu_execution_provider.h>
// #include <onnxruntime/providers.h>

InitialHintProvider::InitialHintProvider(const QString& pathToOnnxModel) {
    Ort::Env onnxEnv(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel");

    Ort::SessionOptions sessionOptions;

    const char* pathToModel = pathToOnnxModel.toUtf8().constData();
    Ort::Session onnxSession(onnxEnv, pathToModel, sessionOptions);

    Ort::RunOptions runOptions;
}

QList<QList<QPoint>> InitialHintProvider::provideHints(const QList<QPixmap> pages) const {
    std::vector<float> input_data = { /* your input data here */ };
    // Ort::Value input_tensor = Ort::Value::CreateTensor<float>(env, input_data.data(), input_data.size(), {1, 3, 224, 224});
    return {};
}


// OutputTensor runModel(InputTensor inputTensor) {
void runModel() {
    const char* inputNames[] = {"input"};
    const char* outputNames[] = {"output"};
    // Ort::Value outputTensor = onnx_session.Run(runOptions, inputNames, &inputTensor, 1, outputNames, 1);
}
