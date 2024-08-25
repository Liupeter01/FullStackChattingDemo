#ifndef RESETPASSWDINTERFACE_H
#define RESETPASSWDINTERFACE_H

#include "httpnetworkconnection.h"
#include "tools.h"
#include <QDialog>
#include <QTimer>
#include <functional>
#include <map>

namespace Ui {
class ResetPasswdInterface;
}

class ResetPasswdInterface : public QDialog {
  Q_OBJECT

  using CallBackFunc = std::function<void(QJsonObject &&json)>;

public:
  explicit ResetPasswdInterface(QWidget *parent = nullptr);
  ~ResetPasswdInterface();

private:
  void setResetAttribute();
  void registerSignal();
  void registerTimeoutSetting();
  void registerEditFinishedEvent();
  void registerNetworkEvent();
  void regisrerCallBackFunctions();

  /*
   * switch to registeration successful page
   * by using stackedWidget switch to
   */
  void switchResetInfoPage();
  void switchResetPasswordPage();
  void switchResetSuccessfulPage();

private slots:
  void on_go_back_login_2_clicked();
  void on_go_back_login_1_clicked();
  void on_go_back_to_login3_clicked();

  void on_verify_account_clicked();
  void on_submit_passwd_clicked();

  void signal_accountvalidating_finished(ServiceType srv_type,
                                         QString json_data,
                                         ServiceStatus srv_status);
  void signal_alterpassword_finished(ServiceType srv_type, QString json_data,
                                     ServiceStatus srv_status);

signals:
  void switchToLogin();
  void switchToResetFront();

private:
  Ui::ResetPasswdInterface *ui;
  std::map<ServiceType, CallBackFunc> m_callbacks;

  QTimer *m_timer;
  const std::size_t m_countdown;
  std::size_t m_counter;
};

#endif // RESETPASSWDINTERFACE_H
