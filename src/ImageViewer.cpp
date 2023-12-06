#include "ImageViewer.h"

#include <QDir>
#include <QFrame>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

QString getImageFilePath() {
    QDir imageDir("images");
    QStringList filters;
    filters << "*.png"
            << "*.jpg"
            << "*.jpeg"
            << "*.bmp";
    imageDir.setNameFilters(filters);
    const auto filePath = imageDir.absoluteFilePath(imageDir.entryList()[0]);
    return filePath;
}

QWidget* getSeparatorWidget(QWidget* parent) {
    QFrame* separator = new QFrame(parent);
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    return separator;
}

ImageViewer::ImageViewer(QWidget* parent)
    : QMainWindow(parent)
    , m_sidebar(new Sidebar(this)) {
    setWindowTitle("Image Viewer");
    setCentralWidget(new QWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget());

    mainLayout->addWidget(m_sidebar);
    mainLayout->addWidget(getSeparatorWidget(this));

    m_image = new QLabel(this);
    m_image->setAlignment(Qt::AlignCenter);
    m_image->setIndent(0);
    m_image->setMargin(0);
    QPixmap pixmap(getImageFilePath());
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

    // Draw a small red square at the clicked position
    const auto circleLabel = new QLabel(m_image);
    QPixmap circlePixmap(10, 10);
    circlePixmap.fill(Qt::red);

    circleLabel->setPixmap(circlePixmap);
    circleLabel->setAlignment(Qt::AlignCenter);

    auto circlePos = event->pos() - labelPos;
    // Center it.
    circlePos -= QPoint(circlePixmap.width() / 2,
                        circlePixmap.height() / 2);

    circleLabel->move(circlePos);
    circleLabel->show();
}

ImageViewer::~ImageViewer() {
}
