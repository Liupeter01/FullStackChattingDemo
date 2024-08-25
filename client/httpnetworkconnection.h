#ifndef HTTPNETWORKCONNECTION_H
#define HTTPNETWORKCONNECTION_H

#include "singleton.hpp"
#include <QJsonObject>
#include <QNetworkAccessManager> //HTTP
#include <QObject>               //connect
#include <QString>
#include <QUrl>

enum class ServiceType : uint8_t {
  SERVICE_VERIFICATION,  // get verification code
  SERVICE_REGISTERATION, // user registeration
  SERVICE_CHECKEEXISTS,  // check account existance
  SERVICE_RESETPASSWD    // password reset
};

enum class ServiceStatus : uint8_t {
  SERVICE_SUCCESS,         // SUCCESS
  JSONPARSE_ERROR,         // json parsing error
  NETWORK_ERROR,           // network failed
  GRPC_ERROR,              // grpc error
  REDIS_UNKOWN_ERROR,      // redis server error code
  REDIS_CPATCHA_NOT_FOUND, // redis no cpatcha
  MYSQL_INTERNAL_ERROR,    // mysql error
  MYSQL_MISSING_INFO,      // mysql missing account info
  MYSQL_ACCOUNT_NOT_EXISTS // mysql account not exists
};

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

private:
  QNetworkAccessManager m_network;
};

#endif // HTTPNETWORKCONNECTION_H
