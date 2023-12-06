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
    QString getImageFilePath(const int index = 0) {
        QDir imageDir("images");
        QStringList filters;
        filters << "*.png"
                << "*.jpg"
                << "*.jpeg"
                << "*.bmp";
        imageDir.setNameFilters(filters);

        const auto fileNames = imageDir.entryList();
        if (index < 0 || index >= fileNames.size()) {
            return QString();
        }
        const auto filePath = imageDir.absoluteFilePath(fileNames[index]);
        return filePath;
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
}

void MainWindow::onNextImage() {
    const auto filePath = getImageFilePath(m_currentImageIndex + 1);
    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        m_imageViewer->setPixmap(pixmap);
        ++m_currentImageIndex;
    }
}

void MainWindow::onPrevImage() {
    const auto filePath = getImageFilePath(m_currentImageIndex - 1);
    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        m_imageViewer->setPixmap(pixmap);
        --m_currentImageIndex;
    }
}

MainWindow::~MainWindow() {
}
