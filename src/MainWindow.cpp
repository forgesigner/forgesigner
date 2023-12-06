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

    unsigned getPageCount() {
        return getFilePathList().size();
    }

    QWidget* getSeparatorWidget(QWidget* parent) {
        QFrame* separator = new QFrame(parent);
        separator->setFrameShape(QFrame::VLine);
        separator->setFrameShadow(QFrame::Sunken);
        return separator;
    }
}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_sidebar(new Sidebar(this))
    , m_imageViewer(new ImageViewer(this)) {
    setWindowTitle("Image Viewer");
    setCentralWidget(new QWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget());

    mainLayout->addWidget(m_sidebar);
    mainLayout->addWidget(getSeparatorWidget(this));

    m_imageViewer->setPixmap(getImageFilePath());
    mainLayout->addWidget(m_imageViewer);

    connect(m_sidebar, &Sidebar::gotNextPage, this, &MainWindow::onNextImage);
    connect(m_sidebar, &Sidebar::gotPrevPage, this, &MainWindow::onPrevImage);

    connect(m_imageViewer, &ImageViewer::gotNewSignature, this, &MainWindow::onNewSignature);

    m_signatures.resize(getPageCount());
}

MainWindow::~MainWindow() {
}

void MainWindow::onNewSignature(QPoint position) {
    // Draw a small red square at the clicked position
    auto signature = QPointer(new SignatureTargetWidget(QPixmap("images/signature.png"), m_imageViewer));
    m_signatures[m_currentImageIndex].append(signature);

    // Center it.
    position -= QPoint(signature->width() / 2,
                       signature->height() / 2);
    signature->move(position);
    signature->show();
}

void MainWindow::onNextImage() {
    const auto filePath = getImageFilePath(m_currentImageIndex + 1);
    if (filePath.isEmpty()) {
        return;
    }

    QPixmap pixmap(filePath);
    m_imageViewer->setPixmap(pixmap);

    hideSignatures(m_currentImageIndex);
    ++m_currentImageIndex;
    showSignatures(m_currentImageIndex);
}

void MainWindow::onPrevImage() {
    const auto filePath = getImageFilePath(m_currentImageIndex - 1);
    if (filePath.isEmpty()) {
        return;
    }

    QPixmap pixmap(filePath);
    m_imageViewer->setPixmap(pixmap);

    hideSignatures(m_currentImageIndex);
    --m_currentImageIndex;
    showSignatures(m_currentImageIndex);
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
