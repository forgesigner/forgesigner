#include "ImageViewer.h"

#include "SignatureTargetWidget.h"

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
    const auto signature = new SignatureTargetWidget(QPixmap("images/signature.png"), this);

    auto squarePos = event->position().toPoint();
    // Center it.
    squarePos -= QPoint(signature->width() / 2,
                        signature->height() / 2);

    signature->move(squarePos);
    signature->show();
}

ImageViewer::~ImageViewer() {
}
