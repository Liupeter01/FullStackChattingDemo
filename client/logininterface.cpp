#include "logininterface.h"
#include "ui_logininterface.h"

LoginInterface::LoginInterface(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginInterface)
{
    ui->setupUi(this);

    /*register pushbutton signal for page swiping*/
    registerSignal();

    /*set login password*/
    setLoginAttribute();
}

LoginInterface::~LoginInterface()
{
    delete ui;
}

void LoginInterface::registerSignal()
{
    connect(this->ui->register_button, &QPushButton::clicked, this, &LoginInterface::switchWindow);
    connect(this->ui->forgot_passwd_label, &ForgotPassword::clicked, this, &LoginInterface::slot_forgot_passwd);
}

void LoginInterface::setLoginAttribute()
{
    /*set password editing attribute*/
    this->ui->passwd_edit->setEchoMode(QLineEdit::Password);
}

void LoginInterface::slot_forgot_passwd()
{
    emit switchReset();
    return;
}
