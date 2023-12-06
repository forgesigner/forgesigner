#include "Sidebar.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

Sidebar::Sidebar(QWidget* parent)
    : QWidget(parent)
    , m_nextPageBtn(new QPushButton("next", this))
    , m_prevPageBtn(new QPushButton("prev", this)) {
    setFixedWidth(100);

    connect(m_nextPageBtn, &QPushButton::clicked, this, &Sidebar::gotNextPage);
    connect(m_prevPageBtn, &QPushButton::clicked, this, &Sidebar::gotPrevPage);

    auto twoBtnsWidget = new QWidget(this);
    auto twoBtnsLayout = new QHBoxLayout(twoBtnsWidget);
    twoBtnsLayout->addWidget(m_prevPageBtn);
    twoBtnsLayout->addWidget(m_nextPageBtn);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(twoBtnsWidget);
}

Sidebar::~Sidebar() {
}
