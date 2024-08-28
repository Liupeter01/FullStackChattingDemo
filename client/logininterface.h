#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include "tools.h"
#include <QDialog>
#include <functional>
#include <map>
#include "httpnetworkconnection.h"
#include "tcpnetworkconnection.h"

/*declaration of network events*/
enum class ServiceType : uint8_t;
enum class ServiceStatus : uint8_t;

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QDialog {
  Q_OBJECT

    using CallBackFunc = std::function<void(QJsonObject &&json)>;

public:
  explicit LoginInterface(QWidget *parent = nullptr);
  ~LoginInterface();

private:
  void registerSignal();
  void setLoginAttribute();
  void registerNetworkEvent();
  void regisrerCallBackFunctions();
  void slot_forgot_passwd();
  void signal_login_finished(ServiceType srv_type,
                                             QString json_data,
                             ServiceStatus srv_status);

signals:
  /*switch to register interface*/
  void switchWindow();

  /*switch to reset interface*/
  void switchReset();

  void signal_establish_long_connnection(TCPNetworkConnection::ChattingServerInfo info);

  private slots:
  void on_login_button_clicked();

private:
  Ui::LoginInterface *ui;
    std::map<ServiceType, CallBackFunc> m_callbacks;
};

#endif // LOGININTERFACE_H
