#include "SignatureTargetWidget.h"

#include <QMouseEvent>

SignatureTargetWidget::SignatureTargetWidget(const QPixmap& signaturePic,
                                             QWidget* parent)
    : QLabel(parent) {
    setPixmap(signaturePic);
    setFixedSize(signaturePic.size());

    setIndent(0);
    setMargin(0);
    setAlignment(Qt::AlignCenter);

    setStyleSheet("border: 2px solid red;");
}

void SignatureTargetWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        deleteLater();
    }
}

void SignatureTargetWidget::mouseMoveEvent(QMouseEvent* event) {
    const auto globalPos = event->globalPosition().toPoint();
    const auto mappedPos = parentWidget()->mapFromGlobal(globalPos);
    const auto offset = QPoint(pixmap().width() / 2, pixmap().height() / 2);

    move(mappedPos - offset);
}

SignatureTargetWidget::~SignatureTargetWidget() {
}
