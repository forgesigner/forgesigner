#include "ImageViewer.h"

#include <QDir>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QScrollArea>
#include <QVBoxLayout>

ImageViewer::ImageViewer(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Image Viewer");
    setCentralWidget(new QWidget);

    QDir imageDir("images");
    QStringList filters;
    filters << "*.png"
            << "*.jpg"
            << "*.jpeg"
            << "*.bmp";
    imageDir.setNameFilters(filters);
    const auto filePath = imageDir.absoluteFilePath(imageDir.entryList()[0]);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget());

    m_image = new QLabel(this);
    m_image->setAlignment(Qt::AlignCenter);
    m_image->setIndent(0);
    m_image->setMargin(0);
    QPixmap pixmap(filePath);
    m_image->setPixmap(pixmap);
    mainLayout->addWidget(m_image);
}

void ImageViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    // Capture the position of the mouse click relative to the image
    const auto labelPos = m_image->pos();
    const auto imageSize = m_image->pixmap().size();

    const auto xOffset = (m_image->width() - imageSize.width()) / 2;
    const auto yOffset = (m_image->height() - imageSize.height()) / 2;

    const auto x = event->pos().x() - labelPos.x() - xOffset;
    const auto y = event->pos().y() - labelPos.y() - yOffset;

    qDebug() << "Mouse click position relative to the image: ("
             << x << ", " << y << ")";

    // Draw a small circle at the clicked position
    const auto circleLabel = new QLabel(m_image);
    QPixmap circlePixmap(10, 10);  // Size of the circle
    circlePixmap.fill(Qt::red);  // Color of the circle

    circleLabel->setPixmap(circlePixmap);
    circleLabel->setAlignment(Qt::AlignCenter);

    auto circlePos = event->pos() - labelPos;
    circlePos -= QPoint(circlePixmap.size().width() / 2,
                        circlePixmap.size().height() / 2);

    circleLabel->move(circlePos);
    circleLabel->show();
}

ImageViewer::~ImageViewer() {
}
