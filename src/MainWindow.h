#pragma once

#include "SignatureTargetWidget.h"

#include <QMainWindow>
#include <QPointer>
#include <QList>
#include <QPointer>

class QLabel;
class QPixmap;
class ImageViewer;
class Sidebar;

using SignatureSet = QList<QPointer<SignatureTargetWidget>>;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void onNextImage();
    void onPrevImage();

    void onNewSignature(QPoint position);

    void hideSignatures(int pageIndex);
    void showSignatures(int pageIndex);
    void filterRemovedSignatures(int pageIndex);

private:
    int m_currentImageIndex{0};
    QList<SignatureSet> m_signatures;

    ImageViewer* m_imageViewer{nullptr};
    Sidebar* m_sidebar{nullptr};
};
