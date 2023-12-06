#pragma once

#include <QLabel>

class QWidget;
class QMouseEvent;
class QPixmap;

class SignatureTargetWidget : public QLabel {
    Q_OBJECT

public:
    SignatureTargetWidget(const QPixmap& signaturePic, QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event) override;  // Hide and destroy on click
    void mouseMoveEvent(QMouseEvent* event) override;  // Support dragging
    ~SignatureTargetWidget() override;
};
