#include "chattingpagewidget.h"
#include "ui_chattingpagewidget.h"

ChattingPageWidget::ChattingPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChattingPageWidget)
{
    ui->setupUi(this);
}

ChattingPageWidget::~ChattingPageWidget()
{
    delete ui;
}
