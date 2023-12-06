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
    Q_EMIT gotNewSignature(event->position().toPoint());
}

ImageViewer::~ImageViewer() {
}
