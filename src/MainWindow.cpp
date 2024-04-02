#include "MainWindow.h"

#include "ImageViewer.h"
#include "Sidebar.h"

#include <QDesktopServices>
#include <QDir>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPdfWriter>
#include <QVBoxLayout>

namespace {
    QWidget* getSeparatorWidget(QWidget* parent) {
        QFrame* separator = new QFrame(parent);
        separator->setFrameShape(QFrame::VLine);
        separator->setFrameShadow(QFrame::Sunken);
        return separator;
    }

    QPixmap getSignaturePixmap() {
        auto pixmap = QPixmap("images/signature.png");
        pixmap = pixmap.scaled(pixmap.size() * 0.3, Qt::KeepAspectRatio);
        return pixmap;
    }
}  // namespace

MainWindow::MainWindow(const QString& pdfFilePath, QWidget* parent)
    : QMainWindow(parent)
    , m_pdfFilePath(pdfFilePath)
    , m_pdfDoc(this)
    , m_initialHintProvider()
    , m_sidebar(new Sidebar(this))
    , m_imageViewer(new ImageViewer(this)) {
    setWindowTitle("ForgeSigner");
    setCentralWidget(new QWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget());

    mainLayout->addWidget(m_sidebar);
    mainLayout->addWidget(getSeparatorWidget(this));
    mainLayout->addWidget(m_imageViewer);

    const auto docLoadStatus = m_pdfDoc.load(pdfFilePath);
    if (docLoadStatus != QPdfDocument::Error::None) {
        qFatal() << "couldn't load PDF file" << pdfFilePath << "error:" << docLoadStatus;
        std::terminate();
    }
    if (m_pdfDoc.status() != QPdfDocument::Status::Ready) {
        qFatal() << "couldn't load PDF file" << pdfFilePath << "status:" << docLoadStatus;
        std::terminate();
    }
    m_currentPageIndex = m_pdfDoc.pageCount() - 1;
    auto lastPage = getPage(m_currentPageIndex);
    m_imageViewer->setPixmap(QPixmap::fromImage(lastPage));

    m_signatures.resize(m_pdfDoc.pageCount());

    connect(m_sidebar, &Sidebar::gotNextPage, this, &MainWindow::onNextPage);
    connect(m_sidebar, &Sidebar::gotPrevPage, this, &MainWindow::onPrevPage);
    connect(m_sidebar, &Sidebar::gotFinishedSigning, this, &MainWindow::onFinishedSigning);

    connect(m_imageViewer, &ImageViewer::gotNewSignature, this, &MainWindow::onNewSignature);

    m_initialHintProvider.loadModel("examples/best_checkpoint.onnx");
    auto signatureHints = m_initialHintProvider.provideHintsForSinglePage(lastPage);
    if (!signatureHints.empty()) {
        for (const auto pos : signatureHints) {
            onNewSignature(pos);
        }
    }
}

MainWindow::~MainWindow() {
}

void MainWindow::onNewSignature(QPoint position) {
    auto signature = QPointer(new SignatureTargetWidget(getSignaturePixmap(), m_imageViewer));
    m_signatures[m_currentPageIndex].append(signature);

    // Center it.
    position -= QPoint(signature->width() / 2,
                       signature->height() / 2);
    signature->move(position);
    signature->show();
}

void MainWindow::onNextPage() {
    goToPage(m_currentPageIndex + 1);
}

void MainWindow::onPrevPage() {
    goToPage(m_currentPageIndex - 1);
}

void MainWindow::goToPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex >= m_pdfDoc.pageCount()) {
        return;
    }

    m_imageViewer->setPixmap(QPixmap::fromImage(getPage(pageIndex)));

    filterRemovedSignatures(m_currentPageIndex);
    hideSignatures(m_currentPageIndex);
    showSignatures(pageIndex);
    m_currentPageIndex = pageIndex;
}

void MainWindow::hideSignatures(int pageIndex) {
    for (auto signature : m_signatures[pageIndex]) {
        if (signature != nullptr) {
            signature->hide();
        }
    }
}

void MainWindow::showSignatures(int pageIndex) {
    for (auto signature : m_signatures[pageIndex]) {
        if (signature != nullptr) {
            signature->show();
        }
    }
}

void MainWindow::filterRemovedSignatures(int pageIndex) {
    erase_if(m_signatures[pageIndex], [](auto p) { return p == nullptr; });
}

QImage MainWindow::getPage(int pageIndex) {
    const auto size = m_pdfDoc.pagePointSize(pageIndex).toSize();
    /* TODO
    auto scaleFactor = 
    size *= std::min(size.height()....
    */
    auto image = m_pdfDoc.render(pageIndex, size);
    if (image.hasAlphaChannel()) {  // if transparent background, fill it with white
        QImage imageWithBackground(size, QImage::Format_ARGB32);
        imageWithBackground.fill(Qt::white);

        QPainter painter(&imageWithBackground);
        painter.drawImage(0, 0, image);
        painter.end();
        return imageWithBackground;
    }
    return image;
}

void MainWindow::onFinishedSigning() {
    const auto pdfFileName = QString("output.pdf");
    writeSignedPdf(pdfFileName);
}

void MainWindow::writeSignedPdf(const QString& pdfFileName) {
    qDebug() << "MainWindow::writeSignedPdf, writing to" << pdfFileName;

    QPdfWriter pdfWriter(pdfFileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter pdfPainter(&pdfWriter);
    pdfPainter.setRenderHint(QPainter::Antialiasing);
    for (int pageIndex = 0; pageIndex < m_pdfDoc.pageCount(); ++pageIndex) {
        const auto signedPage = paintSignaturesOnPage(pageIndex);
        const auto paintRect = QRect(0, 0, pdfWriter.logicalDpiX() * 8.3, pdfWriter.logicalDpiY() * 11.7);
        pdfPainter.drawImage(paintRect, signedPage);
        if (pageIndex != m_pdfDoc.pageCount() - 1) {
            pdfWriter.newPage();
        }
    }
    pdfPainter.end();

    qDebug() << "MainWindow::writeSignedPdf, finished writing";
    QDesktopServices::openUrl(pdfFileName);
}

QImage MainWindow::paintSignaturesOnPage(int pageIndex) {
    auto page = getPage(pageIndex);
    const auto signature = getSignaturePixmap();

    QPainter painter(&page);
    for (const auto signatureWidgetPtr : m_signatures[pageIndex]) {
        if (signatureWidgetPtr == nullptr) {
            continue;
        }
        const auto position = signatureWidgetPtr->pos();
        painter.drawPixmap(position, signature);
    }
    painter.end();

    return page;
}
