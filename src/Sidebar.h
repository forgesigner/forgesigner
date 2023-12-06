#pragma once

#include <QWidget>

class QPushButton;

class Sidebar : public QWidget {
    Q_OBJECT

public:
    explicit Sidebar(QWidget* parent = nullptr);
    ~Sidebar() override;

Q_SIGNALS:
    void gotNextPage();
    void gotPrevPage();
    void gotFinishedSigning();

private:
    QPushButton* m_nextPageBtn{nullptr};
    QPushButton* m_prevPageBtn{nullptr};
    QPushButton* m_finishSigningBtn{nullptr};
};
