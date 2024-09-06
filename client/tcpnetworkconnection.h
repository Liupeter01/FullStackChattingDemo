#ifndef TCPNETWORKCONNECTION_H
#define TCPNETWORKCONNECTION_H

#include "def.hpp"
#include <MsgNode.hpp>
#include <QJsonObject>
#include <QObject> //connect
#include <QString>
#include <QTcpSocket>
#include <QUrl>
#include <functional>
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
    ChattingServerInfo() : uuid(0) {}
    std::size_t uuid;
    QString host;
    QString port;
    QString token;
  };

public:
  ~TCPNetworkConnection();

  /*use signal to trigger data sending*/
  void
  send_data(SendNode<QByteArray, std::function<uint16_t(uint16_t)>> &&data);

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

signals:
  void signal_establish_long_connnection(
      TCPNetworkConnection::ChattingServerInfo info);

  /*return connection status to login class*/
  void signal_connection_status(bool status);

  /*login to server failed*/
  void signal_login_failed(ServiceStatus status);

  /*if login success, then switch to chatting dialog*/
  void signal_switch_chatting_dialog();

private:
  /*establish tcp socket with server*/
  QTcpSocket m_socket;

  /*save server connection info*/
  ChattingServerInfo m_server;

  struct RecvInfo {
    uint16_t _id = 0;
    uint16_t _length = 0;
    QByteArray _msg;
  } m_received;

  /*create a connection buffer to store the data transfer from server*/
  RecvNode<QByteArray, std::function<uint16_t(uint16_t)>> m_buffer;

  /*according to service type to execute callback*/
  std::map<ServiceType, Callbackfunction> m_callbacks;
};

#endif // TCPNETWORKCONNECTION_H
