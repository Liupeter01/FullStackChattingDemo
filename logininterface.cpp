#include "logininterface.h"
#include "ui_logininterface.h"

LoginInterface::LoginInterface(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginInterface)
{
    ui->setupUi(this);

    /*register pushbutton signal for page swiping*/
    registerSignal();
}

LoginInterface::~LoginInterface()
{
    delete ui;
}

void LoginInterface::registerSignal()
{
    connect(this->ui->register_button, &QPushButton::clicked, this, &LoginInterface::switchWindow);
}
