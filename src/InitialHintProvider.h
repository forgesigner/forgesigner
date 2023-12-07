//TODO:
#pragma once

#include <onnxruntime/onnxruntime_cxx_api.h>

#include <QList>
#include <QPoint>

class QString;
class QPixmap;

class InitialHintProvider {
public:
    InitialHintProvider();
    void loadModel(const QString& pathToOnnxModel);
    bool isAvailable() const {
        return m_isAvailable;
    }
    
    QList<QList<QPoint>> provideHints(const QList<QPixmap> pages) const;

private:
    // OutputTensor runModel(InputTensor inputTensor) const;

private:
    bool m_isAvailable{false};

    Ort::Env m_onnxEnv{ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel"};
    Ort::Session m_onnxSession{nullptr};
    Ort::SessionOptions m_onnxSessionOptions{};
    Ort::RunOptions m_runOptions{};
};
