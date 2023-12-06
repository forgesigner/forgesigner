#pragma once

#include <QMainWindow>

class QScrollArea;
class QLabel;
class QPixmap;

class ImageViewer : public QMainWindow {
    Q_OBJECT

public:
    explicit ImageViewer(QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event) override;
    ~ImageViewer() override;

private:
    QLabel* m_image{nullptr};
};
