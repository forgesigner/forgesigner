#include "Sidebar.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

Sidebar::Sidebar(QWidget* parent)
    : QWidget(parent)
    , m_nextPageBtn(new QPushButton("next", this))
    , m_prevPageBtn(new QPushButton("prev", this))
    , m_finishSigningBtn(new QPushButton("Finish", this)) {
    setFixedWidth(100);

    connect(m_nextPageBtn, &QPushButton::clicked, this, &Sidebar::gotNextPage);
    connect(m_prevPageBtn, &QPushButton::clicked, this, &Sidebar::gotPrevPage);
    connect(m_finishSigningBtn, &QPushButton::clicked, this, &Sidebar::gotFinishedSigning);

    auto twoBtnsWidget = new QWidget(this);
    twoBtnsWidget->setFixedHeight(50);
    auto twoBtnsLayout = new QHBoxLayout(twoBtnsWidget);
    twoBtnsLayout->addWidget(m_prevPageBtn);
    twoBtnsLayout->addWidget(m_nextPageBtn);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(twoBtnsWidget);
    mainLayout->addWidget(m_finishSigningBtn);
}

Sidebar::~Sidebar() {
}
