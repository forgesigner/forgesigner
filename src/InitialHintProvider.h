#pragma once

#include <onnxruntime/onnxruntime_cxx_api.h>

#include <QList>
#include <QPoint>
#include <QImage>

class QString;
class QPixmap;

class InitialHintProvider {
public:
    explicit InitialHintProvider();
    void loadModel(const QString& pathToOnnxModel);
    bool isAvailable() const {
        return m_isAvailable;
    }
    
    QList<QPoint> provideHintsForSinglePage(QImage page) const;

private:
    bool m_isAvailable{false};

    Ort::Env m_onnxEnv{ORT_LOGGING_LEVEL_WARNING, "ONNXRuntimeModel"};
    Ort::Session m_onnxSession{nullptr};
    Ort::SessionOptions m_onnxSessionOptions{};
};
