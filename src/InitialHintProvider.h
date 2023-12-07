//TODO:
#pragma once

#include <QList>
#include <QPoint>

class QString;
class QPixmap;

class InitialHintProvider {
public:
    explicit InitialHintProvider(const QString& pathToOnnxModel);
    bool isAvailable() const {
        return m_isAvailable;
    }
    
    QList<QList<QPoint>> provideHints(const QList<QPixmap> pages) const;

private:
    // OutputTensor runModel(InputTensor inputTensor) const;

private:
    bool m_isAvailable{false};
};
