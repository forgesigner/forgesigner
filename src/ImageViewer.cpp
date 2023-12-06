#include "ImageViewer.h"

#include <QMouseEvent>

ImageViewer::ImageViewer(QWidget* parent)
    : QLabel(parent) {
    setAlignment(Qt::AlignTop);
    setIndent(0);
    setMargin(0);
}

void ImageViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    qDebug() << event->position();

    // Draw a small red square at the clicked position
    const auto squareLabel = new QLabel(this);
    QPixmap squarePixmap(10, 10);
    squarePixmap.fill(Qt::red);

    squareLabel->setPixmap(squarePixmap);
    squareLabel->setAlignment(Qt::AlignCenter);

    auto squarePos = event->position().toPoint();
    // Center it.
    squarePos -= QPoint(squarePixmap.width() / 2,
                        squarePixmap.height() / 2);

    squareLabel->move(squarePos);
    squareLabel->show();
}

ImageViewer::~ImageViewer() {
}
