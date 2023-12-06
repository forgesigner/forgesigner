#pragma once

#include <QLabel>

class QWidget;
class QMouseEvent;

class ImageViewer : public QLabel {
    Q_OBJECT

public:
    explicit ImageViewer(QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event) override;
    ~ImageViewer() override;
};
