#include "tools.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

#include "httpnetworkconnection.h"
#include "resetpasswdinterface.h"
#include "ui_resetpasswdinterface.h"

ResetPasswdInterface::ResetPasswdInterface(QWidget *parent)
    : m_countdown(5 /*seconds*/), m_timer(new QTimer(this)), QDialog(parent),
      ui(new Ui::ResetPasswdInterface) {
  ui->setupUi(this);

  /*set password edit echo mode*/
  setResetAttribute();

  registerSignal();

  /*
   * control back to login timeout setting
   * returning back to login page within 5s
   */
  registerTimeoutSetting();

  /*switch reset info page*/
  switchResetInfoPage();

  /*register lineedit event*/
  registerEditFinishedEvent();

  /*register network response event*/
  registerNetworkEvent();

  regisrerCallBackFunctions();

  /*load registeration interface's image*/
  Tools::loadImgResources(
      {"show_password.png", "show_passwd_selected.png",
       "invisiable_password.png", "invisiable_passwd_selected.png"},
      (ui->newpasswd_show->width() + ui->newconfirm_show->width()) / 2,
      (ui->newpasswd_show->height() + ui->newconfirm_show->height()) / 2);

  /*set default image for registeration page*/
  Tools::setQLableImage(ui->newpasswd_show, "invisiable_password.png");
  Tools::setQLableImage(ui->newconfirm_show, "invisiable_password.png");
}

ResetPasswdInterface::~ResetPasswdInterface() { delete ui; }

void ResetPasswdInterface::registerNetworkEvent() {
  connect(HttpNetworkConnection::get_instance().get(),
          &HttpNetworkConnection::signal_accountValidating_finished, this,
          &ResetPasswdInterface::signal_accountvalidating_finished);

  connect(HttpNetworkConnection::get_instance().get(),
          &HttpNetworkConnection::signal_alterPassword_finished, this,
          &ResetPasswdInterface::signal_alterpassword_finished);
}

void ResetPasswdInterface::regisrerCallBackFunctions() {
  m_callbacks.insert(std::pair<ServiceType, CallBackFunc>(
      ServiceType::SERVICE_CHECKEEXISTS, [this](QJsonObject &&json) {
        auto error = json["error"].toInt();

        if (error != static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS)) {
          Tools::setWidgetAttribute(this->ui->status_label_1,
                                    QString("Service Error!"), false);
          return;
        }
        switchResetPasswordPage();
      }));

  m_callbacks.insert(std::pair<ServiceType, CallBackFunc>(
      ServiceType::SERVICE_RESETPASSWD, [this](QJsonObject &&json) {
        auto error = json["error"].toInt();

        if (error != static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS)) {
          Tools::setWidgetAttribute(
              this->ui->status_label_2,
              QString("Reset Password Failed! Internel Error"), false);
          return;
        }

        Tools::setWidgetAttribute(this->ui->status_label_2,
                                  QString("Reset Password Successful"), true);

        /*switch to successful page!*/
        switchResetSuccessfulPage();
      }));
}

void ResetPasswdInterface::setResetAttribute() {
  /*set password editing attribute*/
  this->ui->newpasswd_edit->setEchoMode(QLineEdit::Password);
  this->ui->newconfirm_edit->setEchoMode(QLineEdit::Password);
}

void ResetPasswdInterface::registerSignal() {
  connect(this, &ResetPasswdInterface::switchToResetFront, this,
          &ResetPasswdInterface::switchResetInfoPage);
}

void ResetPasswdInterface::on_go_back_login_2_clicked() {
  emit switchToResetFront();
  return;
}

void ResetPasswdInterface::on_go_back_login_1_clicked() {
  emit switchToLogin();
  return;
}

void ResetPasswdInterface::on_go_back_to_login3_clicked() {
  emit switchToLogin();
  return;
}

void ResetPasswdInterface::registerTimeoutSetting() {
  m_counter = m_countdown;

  connect(m_timer, &QTimer::timeout, [this]() {
    ui->timeout_label->setText(QString("returning back to login page within ") +
                               QString::number(m_counter) + 's');
    --m_counter;

    /*reset data display*/
    if (m_counter <= 0) {
      m_timer->stop();
      emit switchToLogin();
      return;
    }
  });
}

void ResetPasswdInterface::registerEditFinishedEvent() {
  /* when user finished editing username info */
  connect(ui->username_edit, &QLineEdit::editingFinished, this, [this]() {
    [[maybe_unused]] auto ret =
        Tools::checkUsername(ui->username_edit, ui->status_label_1);
  });

  connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]() {
    [[maybe_unused]] auto ret =
        Tools::checkEmail(ui->email_edit, ui->status_label_1);
  });

  /* when user finished editing password info */
  connect(ui->newpasswd_edit, &QLineEdit::editingFinished, this, [this]() {
    [[maybe_unused]] auto ret =
        Tools::checkPassword(ui->email_edit, ui->status_label_2);
  });

  connect(ui->newconfirm_edit, &QLineEdit::editingFinished, this, [this]() {
    [[maybe_unused]] auto ret = Tools::checkSimilarity(
        ui->newpasswd_edit, ui->newconfirm_edit, ui->status_label_2);
  });
  connect(
      ui->newpasswd_show, &PasswordDisplaySwitching::clicked, this, [this]() {
          auto state = ui->newpasswd_show->getState();
          if (state.visiable == LabelState::VisiableStatus::ENABLED) {
              this->ui->newpasswd_edit->setEchoMode(QLineEdit::Normal);
              Tools::setQLableImage(ui->newpasswd_show, "show_password.png");
          } else {
              this->ui->newpasswd_edit->setEchoMode(QLineEdit::Password);
              Tools::setQLableImage(ui->newpasswd_show, "invisiable_password.png");
          }
      });

  connect(
      ui->newconfirm_show, &PasswordDisplaySwitching::clicked, this, [this]() {
          auto state = ui->newconfirm_show->getState();
          if (state.visiable == LabelState::VisiableStatus::ENABLED) {
              this->ui->newconfirm_edit->setEchoMode(QLineEdit::Normal);
              Tools::setQLableImage(ui->newconfirm_show, "show_password.png");
          } else {
              this->ui->newconfirm_edit->setEchoMode(QLineEdit::Password);
              Tools::setQLableImage(ui->newconfirm_show, "invisiable_password.png");
          }
      });
}

/*
 * switch to registeration successful page
 * by using stackedWidget switch to successful_page
 */
void ResetPasswdInterface::switchResetInfoPage() {
  ui->stackedWidget->setCurrentWidget(ui->reset_page);
}

void ResetPasswdInterface::switchResetPasswordPage() {
  ui->stackedWidget->setCurrentWidget(ui->passwd_page);
}

void ResetPasswdInterface::switchResetSuccessfulPage() {
  ui->stackedWidget->setCurrentWidget(ui->successful_page);
  m_timer->start(1000 /*default time interval = 1000ms(1s)*/);
}

void ResetPasswdInterface::signal_accountvalidating_finished(
    ServiceType srv_type, QString json_data, ServiceStatus srv_status) {
  /*handle network error*/
  if (!json_data.length() && srv_status == ServiceStatus::NETWORK_ERROR) {
    Tools::setWidgetAttribute(this->ui->status_label_1,
                              QString("Network Error!"), false);
    return;
  }

  QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
  if (json_obj.isNull()) { // converting failed
    Tools::setWidgetAttribute(this->ui->status_label_1,
                              QString("Retrieve Data Error!"), false);
    // journal log system
    qDebug() << "[FATAL ERROR]: json object is null!\n";
    return;
  }

  if (!json_obj.isObject()) {
    Tools::setWidgetAttribute(this->ui->status_label_1,
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

void ResetPasswdInterface::signal_alterpassword_finished(
    ServiceType srv_type, QString json_data, ServiceStatus srv_status) {
  /*handle network error*/
  if (!json_data.length() && srv_status == ServiceStatus::NETWORK_ERROR) {
    Tools::setWidgetAttribute(this->ui->status_label_2,
                              QString("Network Error!"), false);
    return;
  }

  QJsonDocument json_obj = QJsonDocument::fromJson(json_data.toUtf8());
  if (json_obj.isNull()) { // converting failed
    Tools::setWidgetAttribute(this->ui->status_label_2,
                              QString("Retrieve Data Error!"), false);
    // journal log system
    qDebug() << "[FATAL ERROR]: json object is null!\n";
    return;
  }

  if (!json_obj.isObject()) {
    Tools::setWidgetAttribute(this->ui->status_label_2,
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

void ResetPasswdInterface::on_verify_account_clicked() {
  if (!Tools::checkUsername(ui->username_edit, ui->status_label_1)) {
    return;
  }
  if (!Tools::checkEmail(ui->email_edit, ui->status_label_1)) {
    return;
  }

  /*Sending e-mail verification code*/
  QJsonObject json;
  json["username"] = ui->username_edit->text();
  json["email"] = ui->email_edit->text();
  HttpNetworkConnection::get_instance()->postHttpRequest(
      Tools::getTargetUrl("/check_accountexists"), json,
      ServiceType::SERVICE_CHECKEEXISTS);
}

void ResetPasswdInterface::on_submit_passwd_clicked() {
  if (!Tools::checkUsername(ui->username_edit, ui->status_label_2)) {
    return;
  }
  if (!Tools::checkEmail(ui->email_edit, ui->status_label_2)) {
    return;
  }
  if (!Tools::checkPassword(ui->newpasswd_edit, ui->status_label_2)) {
    return;
  }
  if (!Tools::checkSimilarity(ui->newpasswd_edit, ui->newconfirm_edit,
                              ui->status_label_2)) {
    return;
  }

  /*Sending e-mail verification code*/
  QJsonObject json;
  json["username"] = ui->username_edit->text();
  json["email"] = ui->email_edit->text();
  json["password"] = ui->newpasswd_edit->text();

  HttpNetworkConnection::get_instance()->postHttpRequest(
      Tools::getTargetUrl("/reset_password"), json,
      ServiceType::SERVICE_RESETPASSWD);
}
