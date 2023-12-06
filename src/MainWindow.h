#pragma once

#include "SignatureTargetWidget.h"

#include <QList>
#include <QMainWindow>
#include <QPointer>

class QLabel;
class QPixmap;
class ImageViewer;
class Sidebar;

using SignatureSet = QList<QPointer<SignatureTargetWidget>>;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void onNextPage();
    void onPrevPage();
    void goToPage(int pageIndex);

    void onNewSignature(QPoint position);

    void hideSignatures(int pageIndex);
    void showSignatures(int pageIndex);
    void filterRemovedSignatures(int pageIndex);

private:
    int m_currentPageIndex{0};
    QList<SignatureSet> m_signatures;

    ImageViewer* m_imageViewer{nullptr};
    Sidebar* m_sidebar{nullptr};
};
