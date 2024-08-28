#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

#include "logininterface.h"
#include "passworddisplayswitching.h"
#include "ui_logininterface.h"

LoginInterface::LoginInterface(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginInterface) {
  ui->setupUi(this);

  /*register pushbutton signal for page swiping*/
  registerSignal();

  /*set login password*/
  setLoginAttribute();

  registerNetworkEvent();
  regisrerCallBackFunctions();

  /*load registeration interface's image*/
  Tools::loadImgResources(
      {"show_password.png", "show_passwd_selected.png",
       "invisiable_password.png", "invisiable_passwd_selected.png"},
      ui->passwd_display->width(), ui->passwd_display->height());

  /*set default image for registeration page*/
  Tools::setQLableImage(ui->passwd_display, "invisiable_password.png");
}

LoginInterface::~LoginInterface() { delete ui; }

void LoginInterface::registerSignal() {
  connect(this->ui->register_button, &QPushButton::clicked, this,
          &LoginInterface::switchWindow);
  connect(this->ui->forgot_passwd_label, &ForgotPassword::clicked, this,
          &LoginInterface::slot_forgot_passwd);
  connect(this->ui->passwd_display, &PasswordDisplaySwitching::clicked, this,
          [this]() {
            auto state = ui->passwd_display->getState();
            if (state.visiable == LabelState::VisiableStatus::ENABLED) {
              this->ui->passwd_edit->setEchoMode(QLineEdit::Normal);
              Tools::setQLableImage(ui->passwd_display, "show_password.png");
            } else {
              this->ui->passwd_edit->setEchoMode(QLineEdit::Password);
              Tools::setQLableImage(ui->passwd_display,
                                    "invisiable_password.png");
            }
          });
}

void LoginInterface::setLoginAttribute() {
  /*set password editing attribute*/
  this->ui->passwd_edit->setEchoMode(QLineEdit::Password);
}

void LoginInterface::registerNetworkEvent() {
  connect(HttpNetworkConnection::get_instance().get(),
          &HttpNetworkConnection::signal_login_finished, this,
          &LoginInterface::slot_login_finished);
}

void LoginInterface::regisrerCallBackFunctions() {
  m_callbacks.insert(std::pair<ServiceType, CallBackFunc>(
      ServiceType::SERVICE_LOGINSERVER, [this](QJsonObject &&json) {
        auto error = json["error"].toInt();

        if (error != static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS)) {
          Tools::setWidgetAttribute(this->ui->status_label_3,
                                    QString("Service Error!"), false);
          return;
        }

        TCPNetworkConnection::ChattingServerInfo info;
        info.uuid = json["uuid"].toInt();
        info.host = json["host"].toString();
        info.port = json["port"].toString();
        info.token = json["token"].toString();

        emit signal_establish_long_connnection(info);
      }));
}

void LoginInterface::slot_login_finished(ServiceType srv_type,
                                         QString json_data,
                                         ServiceStatus srv_status) {
  /*handle network error*/
  if (!json_data.length() && srv_status == ServiceStatus::NETWORK_ERROR) {
    Tools::setWidgetAttribute(this->ui->status_label_3,
                              QString("Network Error!"), false);
    return;
  }

  // json_data
  QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
  if (json_obj.isNull()) { // converting failed
    Tools::setWidgetAttribute(this->ui->status_label_3,
                              QString("Retrieve Data Error!"), false);
    // journal log system
    qDebug() << "[FATAL ERROR]: json object is null!\n";
    return;
  }

  if (!json_obj.isObject()) {
    Tools::setWidgetAttribute(this->ui->status_label_3,
                              QString("Retrieve Data Error!"), false);
    // journal log system
    qDebug() << "[FATAL ERROR]: json can not be converted to an object!\n";
    return;
  }

  /*to prevent app crash due to callback is not exists*/
  try {
    m_callbacks[srv_type](std::move(json_obj.object()));
  } catch (const std::exception &e) {
    qDebug() << e.what();
  }
}

void LoginInterface::slot_forgot_passwd() {
  emit switchReset();
  return;
}

void LoginInterface::on_login_button_clicked() {
  QString username = this->ui->username_edit->text();
  QString passwd = this->ui->passwd_edit->text();

  QJsonObject json;
  json["username"] = username;
  json["password"] = passwd;

  HttpNetworkConnection::get_instance()->postHttpRequest(
      Tools::getTargetUrl("/trylogin_server"), json,
      ServiceType::SERVICE_LOGINSERVER);
}
