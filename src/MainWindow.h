#pragma once

#include <QMainWindow>

class QLabel;
class QPixmap;
class ImageViewer;
class Sidebar;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void onNextImage();
    void onPrevImage();

private:
    int m_currentImageIndex{0};
    ImageViewer* m_imageViewer{nullptr};
    Sidebar* m_sidebar{nullptr};
};
