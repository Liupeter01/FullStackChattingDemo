#include "tools.h"
#include "registerinterface.h"
#include "ui_registerinterface.h"
#include "httpnetworkconnection.h"
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>

registerinterface::registerinterface(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerinterface)
{
    ui->setupUi(this);

    /*set multiple attributes*/
    setRegisterAttribute();

    /*register network event from registeration acts*/
    registerNetworkEvent();

    /*register callback functions to update interface accroding to network responses*/
    regisrerCallBackFunctions();
}

registerinterface::~registerinterface()
{
    delete ui;
}

void registerinterface::registerNetworkEvent()
{
    connect(HttpNetworkConnection::get_instance().get(),
            &HttpNetworkConnection::signal_registeration_finished,
            this, &registerinterface::signal_registeration_finished
    );

    connect(HttpNetworkConnection::get_instance().get(),
            &HttpNetworkConnection::signal_verification_finished,
            this, &registerinterface::signal_verification_finished
    );
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

void registerinterface::regisrerCallBackFunctions()
{
    m_callbacks.insert(std::pair<ServiceType, CallBackFunc>(
        ServiceType::SERVICE_VERIFICATION, [this](QJsonObject &&json){
            auto error = json["error"].toInt();

            if(error != static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS)){
                Tools::setWidgetAttribute(
                        this->ui->emailerror_label,
                        QString("Service Error!"),
                    false
                );
                return;
            }

            QString email = json["email"].toString();
            qDebug() << "E-mail = " << email << '\n';

            Tools::setWidgetAttribute(
                this->ui->emailerror_label,
                QString("code has already been sent to email"),
                true
            );
        })
    );

    m_callbacks.insert(std::pair<ServiceType, CallBackFunc>(
        ServiceType::SERVICE_REGISTERATION, [this](QJsonObject &&json){
            auto error = json["error"].toInt();

            if(error != static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS)){
                Tools::setWidgetAttribute(
                    this->ui->register_label,
                    QString("Register Failed! Internel Error"),
                    false
                );
                return;
            }

            Tools::setWidgetAttribute(
                this->ui->register_label,
                QString("Register Successful"),
                true
            );
        })
    );
}

void registerinterface::on_confirm_button_clicked()
{
    [[maybe_unused]] QString username = this->ui->newuser_edit->text();
    [[maybe_unused]] QString passwd = this->ui->newpassed_edit->text();
    [[maybe_unused]] QString confirm_passwd = this->ui->confirmpasswd_edit->text();
    [[maybe_unused]] QString email_text = this->ui->email_edit->text();
    [[maybe_unused]] QString cpatcha = this->ui->verification_edit->text();

    QRegularExpression reg_email(tr("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,}$"));
    if(!reg_email.match(email_text).hasMatch()){
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Invalid E-mail address"), false);
        return;
    }

    /* at least one capital letter(A-Z)
     * at least one lower case letter(a-z)
     * at least one number(0-z)
     * at least 8 character
     */
    QRegularExpression reg_password(tr("(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9]).{8,}"));

    /*does not match the regular expression critiera!*/
    if(!reg_password.match(passwd).hasMatch()){
        Tools::setWidgetAttribute(this->ui->passwderror_label, QString("Invalid Password!"), false);
        return;
    }

    Tools::setWidgetAttribute(this->ui->passwderror_label, QString("Valid Password"), true);

    /*does not match origin password*/
    if(passwd != confirm_passwd){
        Tools::setWidgetAttribute(this->ui->passwdmatch_label, QString("Password does not match!"), false);
        return;
    }

    Tools::setWidgetAttribute(this->ui->passwdmatch_label, QString("Password match"), true);

    /*Sending e-mail verification code*/
    QJsonObject json;
    json["username"] = username;
    json["password"] = confirm_passwd;
    json["email"] = email_text;
    json["cpatcha"] = cpatcha;

    HttpNetworkConnection::get_instance()->postHttpRequest(
        Tools::getTargetUrl("/post_registration"),
        json,
        ServiceType::SERVICE_REGISTERATION
    );
}

void registerinterface::on_verification_button_clicked()
{
    QString email_text = this->ui->email_edit->text();
    QRegularExpression reg(tr("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,}$"));
    if(!reg.match(email_text).hasMatch()){
        Tools::setWidgetAttribute(
            this->ui->emailerror_label,
            QString("Invalid E-mail address"),
            false
        );
        return;
    }

    Tools::setWidgetAttribute(
        this->ui->emailerror_label,
        QString("Valid E-mail address"),
        true
    );

    /*Sending e-mail verification code*/
    QJsonObject json;
    json["email"] = email_text;
    HttpNetworkConnection::get_instance()->postHttpRequest(
        Tools::getTargetUrl("/get_verification"),
        json,
        ServiceType::SERVICE_VERIFICATION
    );
}

void registerinterface::signal_registeration_finished(
    ServiceType srv_type,
    QString json_data,
    ServiceStatus srv_status)
{
    /*handle network error*/
    if(!json_data.length() && srv_status == ServiceStatus::NETWORK_ERROR){
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Network Error!"), false);
        return;
    }

    QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
    if(json_obj.isNull()){  //converting failed
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json object is null!\n";
        return;
    }

    if(!json_obj.isObject()){
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json can not be converted to an object!\n";
        return;
    }

    m_callbacks[srv_type](std::move(json_obj.object()));
}

void registerinterface::signal_verification_finished(
    ServiceType srv_type,
    QString json_data,
    ServiceStatus srv_status)
{
    /*handle network error*/
    if(!json_data.length() && srv_status == ServiceStatus::NETWORK_ERROR){
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Network Error!"), false);
        return;
    }

    //json_data
    QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
    if(json_obj.isNull()){  //converting failed
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json object is null!\n";
        return;
    }

    if(!json_obj.isObject()){
        Tools::setWidgetAttribute(this->ui->emailerror_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json can not be converted to an object!\n";
        return;
    }

    m_callbacks[srv_type](std::move(json_obj.object()));
}
