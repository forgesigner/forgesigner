#include "MainWindow.h"

#include "ImageViewer.h"
#include "Sidebar.h"

#include <QDir>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>

namespace {
    QStringList getFilePathList() {
        QDir imageDir("images");
        QStringList filters;
        filters << "*.png"
                << "*.jpg"
                << "*.jpeg"
                << "*.bmp";
        imageDir.setNameFilters(filters);
        auto filePathList = imageDir.entryList();
        for (auto& fileName : filePathList) {
            fileName = imageDir.absoluteFilePath(fileName);
        }
        return filePathList;
    }

    QString getImageFilePath(const int index = 0) {
        const auto filePathList = getFilePathList();
        if (index < 0 || index >= filePathList.size()) {
            return QString();
        }
        return filePathList[index];
    }

    QWidget* getSeparatorWidget(QWidget* parent) {
        QFrame* separator = new QFrame(parent);
        separator->setFrameShape(QFrame::VLine);
        separator->setFrameShadow(QFrame::Sunken);
        return separator;
    }
}  // namespace

MainWindow::MainWindow(const QString& pdfFilePath, QWidget* parent)
    : QMainWindow(parent)
    , m_pdfDoc(this)
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
    m_imageViewer->setPixmap(getImageFilePath());

    m_signatures.resize(m_pdfDoc.pageCount());

    connect(m_sidebar, &Sidebar::gotNextPage, this, &MainWindow::onNextPage);
    connect(m_sidebar, &Sidebar::gotPrevPage, this, &MainWindow::onPrevPage);
    connect(m_imageViewer, &ImageViewer::gotNewSignature, this, &MainWindow::onNewSignature);
}

MainWindow::~MainWindow() {
}

void MainWindow::onNewSignature(QPoint position) {
    auto signature = QPointer(new SignatureTargetWidget(QPixmap("images/signature.png"), m_imageViewer));
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
    const auto filePath = getImageFilePath(pageIndex);
    if (filePath.isEmpty()) {
        return;
    }

    QPixmap pixmap(filePath);
    m_imageViewer->setPixmap(pixmap);

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
