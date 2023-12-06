#pragma once

#include "Sidebar.h"

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
    void onNextImage();
    void onPrevImage();

private:
    int m_currentImageIndex{0};
    QLabel* m_image{nullptr};
    Sidebar* m_sidebar{nullptr};
};
