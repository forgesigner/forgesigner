#include "ImageViewer.h"

#include <QMouseEvent>

ImageViewer::ImageViewer(QWidget* parent)
    : QLabel(parent) {
    // setAlignment(Qt::AlignCenter);
    setAlignment(Qt::AlignTop);
    setIndent(0);
    setMargin(0);
}

void ImageViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    // Capture the position of the mouse click relative to the image
    const auto imageSize = pixmap().size();
    const auto xOffset = (width() - imageSize.width()) / 2;
    const auto yOffset = (height() - imageSize.height()) / 2;
    const auto x = event->pos().x() - xOffset;  // - pos().x() 
    const auto y = event->pos().y() - yOffset;  // - pos().y() 

    qDebug() << "Mouse click position relative to the image: ("
             << x << ", " << y << ")";
    // qDebug() << event->position();

    // Draw a small red square at the clicked position
    const auto circleLabel = new QLabel(this);
    QPixmap circlePixmap(10, 10);
    circlePixmap.fill(Qt::red);

    circleLabel->setPixmap(circlePixmap);
    circleLabel->setAlignment(Qt::AlignCenter);

    auto circlePos = event->position();
    // // Center it.
    // circlePos -= QPoint(circlePixmap.width() / 2,
    //                     circlePixmap.height() / 2);

    circleLabel->move(event->position().toPoint());
    circleLabel->show();
}

ImageViewer::~ImageViewer() {
}
