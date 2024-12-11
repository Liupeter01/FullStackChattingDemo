#ifndef HTTPNETWORKCONNECTION_H
#define HTTPNETWORKCONNECTION_H

#include "def.hpp"
#include "singleton.hpp"
#include <QJsonObject>
#include <QNetworkAccessManager> //HTTP
#include <QObject>               //connect
#include <QString>
#include <QUrl>

class HttpNetworkConnection
    : public QObject,
      public Singleton<HttpNetworkConnection>,
      public std::enable_shared_from_this<HttpNetworkConnection> {
  Q_OBJECT
  friend class Singleton<HttpNetworkConnection>;

public:
  /*Singleton class will deploy dtor function*/
  ~HttpNetworkConnection();

  void postHttpRequest(QUrl url, QJsonObject json, ServiceType srv_type);

private:
  /*Singleton class could have access to deprived class's ctor function*/
  HttpNetworkConnection();

  void registerNetworkEvent();

private slots:
  void slot_http_finished(ServiceType srv_type, QString json_data,
                          ServiceStatus srv_status);

signals:
  void signal_http_finished(ServiceType srv_type, QString json_data,
                            ServiceStatus srv_status);

  /*connect to registerinterface!*/
  void signal_registeration_finished(ServiceType srv_type, QString json_data,
                                     ServiceStatus srv_status);
  void signal_verification_finished(ServiceType srv_type, QString json_data,
                                    ServiceStatus srv_status);
  void signal_accountValidating_finished(ServiceType srv_type,
                                         QString json_data,
                                         ServiceStatus srv_status);
  void signal_alterPassword_finished(ServiceType srv_type, QString json_data,
                                     ServiceStatus srv_status);

  void signal_login_finished(ServiceType srv_type, QString json_data,
                             ServiceStatus srv_status);

private:
  QNetworkAccessManager m_network;
};

#endif // HTTPNETWORKCONNECTION_H
