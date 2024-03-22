#pragma once

#include "InitialHintProvider.h"
#include "SignatureTargetWidget.h"

#include <QList>
#include <QMainWindow>
#include <QPdfDocument>
#include <QPointer>

class QLabel;
class QPixmap;
class ImageViewer;
class Sidebar;

using SignatureSet = QList<QPointer<SignatureTargetWidget>>;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const QString& pdfFilePath, QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void onNextPage();
    void onPrevPage();
    void goToPage(int pageIndex);

    void initSignaturesFromHints(int pageIndex);
    void onNewSignature(QPoint position);

    void hideSignatures(int pageIndex);
    void showSignatures(int pageIndex);
    void filterRemovedSignatures(int pageIndex);

    QImage getPage(int pageIndex);

    void onFinishedSigning();
    void writeSignedPdf(const QString& pdfFilePath);
    QImage paintSignaturesOnPage(int pageIndex);

private:
    const QString m_pdfFilePath;
    QPdfDocument m_pdfDoc{};

    int m_currentPageIndex{0};
    QList<SignatureSet> m_signatures{};

    InitialHintProvider m_initialHintProvider{};

    Sidebar* m_sidebar{nullptr};
    ImageViewer* m_imageViewer{nullptr};
};
