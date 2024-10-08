#include "addusernamecardwidget.h"
#include "addnewuserstackwidget.h"
#include "ui_addnewuserstackwidget.h"

AddNewUserStackWidget::AddNewUserStackWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddNewUserStackWidget)
{
    ui->setupUi(this);
}

AddNewUserStackWidget::~AddNewUserStackWidget()
{
    delete ui;
}

