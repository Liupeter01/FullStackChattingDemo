#include "tools.h"
#include "registerinterface.h"
#include "ui_registerinterface.h"
#include <QRegularExpression>

registerinterface::registerinterface(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerinterface)
{
    ui->setupUi(this);

    /*set multiple attributes*/
    setRegisterAttribute();
}

registerinterface::~registerinterface()
{
    delete ui;
}


void registerinterface::setRegisterAttribute()
{
    /*set password editing attribute*/
    this->ui->newpassed_edit->setEchoMode(QLineEdit::Password);
    this->ui->confirmpasswd_edit->setEchoMode(QLineEdit::Password);

    /*set password error notification*/
    Tools::setWidgetAttribute(this->ui->passwderror_label, QString("No Input"), false);
    Tools::setWidgetAttribute(this->ui->emailerror_label, QString("No Input"), false);
}

void registerinterface::on_verification_button_clicked()
{
    QString email_text = this->ui->email_edit->text();
    QRegularExpression reg(tr("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,}$"));
    if(reg.match(email_text).hasMatch()){
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Valid E-mail address"), true);
        //TO DO
    }
    else{
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Invalid E-mail address"), false);
    }
}

