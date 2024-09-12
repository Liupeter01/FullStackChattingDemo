#include "chattingstackpage.h"
#include "ui_chattingstackpage.h"

ChattingStackPage::ChattingStackPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChattingStackPage)
{
    ui->setupUi(this);
}

ChattingStackPage::~ChattingStackPage()
{
    delete ui;
}
