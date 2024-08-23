#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include "tools.h"
#include "registerinterface.h"
#include "ui_registerinterface.h"
#include "httpnetworkconnection.h"


registerinterface::registerinterface(QWidget *parent)
    : m_countdown(5/*seconds*/)
    , m_timer(new QTimer(this))
    , QDialog(parent)
    , ui(new Ui::registerinterface)
{
    ui->setupUi(this);

    /*register return to login interface*/
    registerSignal();

    /*
     * control back to login timeout setting
     * returning back to login page within 5s
     */
    registerTimeoutSetting();

    /*set multiple attributes*/
    setRegisterAttribute();

    /*register edit finished event on QLineEdit*/
    registerEditFinishedEvent();

    /*register network event from registeration acts*/
    registerNetworkEvent();

    /*register callback functions to update interface accroding to network responses*/
    regisrerCallBackFunctions();

    /*switch reg info page*/
    switchRegInfoPage();

    /*load registeration interface's image*/
    Tools::loadImgResources(
        {
            "show_password.png",
            "show_passwd_selected.png",
            "invisiable_password.png",
            "invisiable_passwd_selected.png"
        },
        (ui->display_passwd->width() + ui->display_confirm->width()) / 2,
        (ui->display_passwd->height() + ui->display_confirm->height()) / 2
    );

    /*set default image for registeration page*/
    Tools::setQLableImage(ui->display_passwd, "invisiable_password.png");
    Tools::setQLableImage(ui->display_confirm, "invisiable_password.png");
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

    this->ui->display_passwd->setCursor(Qt::PointingHandCursor);
    this->ui->display_confirm->setCursor(Qt::PointingHandCursor);
}

void registerinterface::registerEditFinishedEvent()
{
    /* when user finished editing username info */
    connect(ui->newuser_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkUsername(ui->newuser_edit, ui->status_label);
    });

    /* when user finished editing password info */
    connect(ui->newpassed_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkPassword(ui->newpassed_edit, ui->status_label);
    });

    /* when user finished editing email info */
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkEmail(ui->email_edit, ui->status_label);
    });

    connect(ui->confirmpasswd_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkSimilarity(ui->newpassed_edit, ui->confirmpasswd_edit, ui->status_label);
    });

    connect(ui->verification_edit, &QLineEdit::editingFinished, this, [this](){
        [[maybe_unused]] auto ret = Tools::checkCaptcha(ui->verification_edit, ui->status_label);
    });

    connect(ui->display_passwd, &PasswordDisplaySwitching::clicked, this, [this](){
        auto state = ui->display_passwd->getState();
        if(state.visiable == LabelState::VisiableStatus::ENABLED){
            this->ui->newpassed_edit->setEchoMode(QLineEdit::Normal);
            Tools::setQLableImage(ui->display_passwd, "show_password.png");
        }
        else{
            this->ui->newpassed_edit->setEchoMode(QLineEdit::Password);
            Tools::setQLableImage(ui->display_passwd, "invisiable_password.png");
        }
    });

    connect(ui->display_confirm, &PasswordDisplaySwitching::clicked, this, [this](){
        auto state = ui->display_confirm->getState();
        if(state.visiable == LabelState::VisiableStatus::ENABLED){
            this->ui->confirmpasswd_edit->setEchoMode(QLineEdit::Normal);
            Tools::setQLableImage(ui->display_confirm, "show_password.png");
        }
        else{
            this->ui->confirmpasswd_edit->setEchoMode(QLineEdit::Password);
            Tools::setQLableImage(ui->display_confirm, "invisiable_password.png");
        }
    });
}

void registerinterface::regisrerCallBackFunctions()
{
    m_callbacks.insert(std::pair<ServiceType, CallBackFunc>(
        ServiceType::SERVICE_VERIFICATION, [this](QJsonObject &&json){
            auto error = json["error"].toInt();

            if(error != static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS)){
                Tools::setWidgetAttribute(
                        this->ui->status_label,
                        QString("Service Error!"),
                    false
                );
                return;
            }

            QString email = json["email"].toString();
            qDebug() << "E-mail = " << email << '\n';

            Tools::setWidgetAttribute(
                this->ui->status_label,
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
                    this->ui->status_label,
                    QString("Register Failed! Internel Error"),
                    false
                );
                return;
            }

            Tools::setWidgetAttribute(
                this->ui->status_label,
                QString("Register Successful"),
                true
            );

            qDebug() << "email = "<<json["email"].toString();
            qDebug() << "uuid = " << json["uuid"].toString();

            /*switch to successful page!*/
            switchRegSucessfulPage();
        })
    );
}

/*
 * switch to registeration successful page
 * by using stackedWidget switch to successful_page
 */
void registerinterface::switchRegInfoPage()
{
    ui->stackedWidget->setCurrentWidget(ui->reg_info);
}

void registerinterface::switchRegSucessfulPage()
{
    ui->stackedWidget->setCurrentWidget(ui->successful_page);
    m_timer->start(1000/*default time interval = 1000ms(1s)*/);
}

void registerinterface::on_confirm_button_clicked()
{
    [[maybe_unused]] QString username = this->ui->newuser_edit->text();
    [[maybe_unused]] QString passwd = this->ui->newpassed_edit->text();
    [[maybe_unused]] QString confirm_passwd = this->ui->confirmpasswd_edit->text();
    [[maybe_unused]] QString email_text = this->ui->email_edit->text();
    [[maybe_unused]] QString cpatcha = this->ui->verification_edit->text();

    if(!Tools::checkUsername(ui->newuser_edit, ui->status_label)){
        return;
    }

    if(!Tools::checkEmail(ui->email_edit, ui->status_label)){
        return;
    }

    if(!Tools::checkPassword(ui->newpassed_edit, ui->status_label)){
        return;
    }

    if(!Tools::checkSimilarity(ui->newpassed_edit, ui->confirmpasswd_edit, ui->status_label)){
        return;
    }

    if(!Tools::checkCaptcha(ui->verification_edit, ui->status_label)){
        return;
    }

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

void registerinterface::registerSignal()
{
    connect(ui->return_to_login, &QPushButton::clicked, this, &registerinterface::switchToLogin);
}

/*
 * control back to login timeout setting
 * returning back to login page within 5s
 */
void registerinterface::registerTimeoutSetting()
{
    m_counter = m_countdown;

    connect(m_timer, &QTimer::timeout, [this](){
        ui->timeout_label->setText(QString("returning back to login page within ") + QString::number(m_counter) + 's');
        --m_counter;

        /*reset data display*/
        if(m_counter <= 0){
            m_timer->stop();
            emit switchToLogin();
            return;
        }
    });
}

void registerinterface::on_verification_button_clicked()
{
    if(!Tools::checkEmail(ui->email_edit, ui->status_label)){
        return;
    }

    /*Sending e-mail verification code*/
    QJsonObject json;
    json["email"] = ui->email_edit->text();
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
        Tools::setWidgetAttribute(this->ui->status_label, QString("Network Error!"), false);
        return;
    }

    QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
    if(json_obj.isNull()){  //converting failed
        Tools::setWidgetAttribute(this->ui->status_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json object is null!\n";
        return;
    }

    if(!json_obj.isObject()){
        Tools::setWidgetAttribute(this->ui->status_label, QString("Retrieve Data Error!"), false);
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
        Tools::setWidgetAttribute(this->ui->status_label, QString("Network Error!"), false);
        return;
    }

    //json_data
    QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
    if(json_obj.isNull()){  //converting failed
        Tools::setWidgetAttribute(this->ui->status_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json object is null!\n";
        return;
    }

    if(!json_obj.isObject()){
        Tools::setWidgetAttribute(this->ui->status_label, QString("Retrieve Data Error!"), false);
        //journal log system
        qDebug() << "[FATAL ERROR]: json can not be converted to an object!\n";
        return;
    }

    m_callbacks[srv_type](std::move(json_obj.object()));
}

void registerinterface::on_cancel_button_clicked()
{
    emit switchToLogin();
    return;
}

