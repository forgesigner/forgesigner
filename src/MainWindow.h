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

    void initSignaturesFromHints();  // TODO: ??
    void onNewSignature(QPoint position);
    // void addNewSignature(QPoint position);  // TODO: needed ?

    void hideSignatures(int pageIndex);
    void showSignatures(int pageIndex);
    void filterRemovedSignatures(int pageIndex);

    QPixmap getPage(int pageIndex);

    void onFinishedSigning();
    void writeSignedPdf(const QString& pdfFilePath);
    QPixmap paintSignaturesOnPage(int pageIndex);


private:
    const QString m_pdfFilePath;
    QPdfDocument m_pdfDoc{};

    int m_currentPageIndex{0};
    QList<SignatureSet> m_signatures{};

    InitialHintProvider m_initialHintProvider{};

    ImageViewer* m_imageViewer{nullptr};
    Sidebar* m_sidebar{nullptr};
};
