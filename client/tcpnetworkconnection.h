#ifndef TCPNETWORKCONNECTION_H
#define TCPNETWORKCONNECTION_H

#include "def.hpp"
#include <QJsonObject>
#include <QObject> //connect
#include <QString>
#include <QTcpSocket>
#include <QUrl>
#include <functional>
#include <messagenode.hpp>
#include <singleton.hpp>

class TCPNetworkConnection
    : public QObject,
      public Singleton<TCPNetworkConnection>,
      public std::enable_shared_from_this<TCPNetworkConnection> {

  Q_OBJECT
  friend class Singleton<TCPNetworkConnection>;
  using Callbackfunction = std::function<void(QJsonObject &&)>;

public:
  struct ChattingServerInfo {
    QString uuid = "";
    QString host = "";
    std::size_t port = 0;
    QString token = "";
  };

public:
  ~TCPNetworkConnection();

private:
  TCPNetworkConnection();

  void registerNetworkEvent();
  void registerSocketSignal();
  void registerCallback();
  void registerErrorHandling();

private:
  bool checkJsonForm(const QJsonObject &json);

private slots:
  void slot_establish_long_connnection(
      TCPNetworkConnection::ChattingServerInfo info);

  /*use it to send data to server*/
  void slot_send_data(SendNode<QByteArray> data);

signals:
  void signal_establish_long_connnection(
      TCPNetworkConnection::ChattingServerInfo info);

  /*return connection status to login class*/
  void signal_connection_status(bool status);

  /*login to server failed*/
  void signal_login_failed(ServiceStatus status);

  /*if login success, then switch to chatting dialog*/
  void signal_switch_chatting_dialog();

  /*use signal to trigger data sending*/
  void signal_send_data(SendNode<QByteArray> data);

private:
  /*establish tcp socket with server*/
  QTcpSocket m_socket;

  /*save server connection info*/
  ChattingServerInfo m_server;

  /*create a connection buffer to store the data transfer from server*/
  RecvNode<QByteArray> m_buffer;

  /*according to service type to execute callback*/
  std::map<ServiceType, Callbackfunction> m_callbacks;
};

#endif // TCPNETWORKCONNECTION_H
