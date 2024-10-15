#ifndef TCPNETWORKCONNECTION_H
#define TCPNETWORKCONNECTION_H

#include "def.hpp"
#include <MsgNode.hpp>
#include <QJsonObject>
#include <QObject> //connect
#include <QString>
#include <QTcpSocket>
#include <QUrl>
#include <QtEndian>
#include <functional>
#include <optional>
#include <singleton.hpp>

struct UserNameCard;

class TCPNetworkConnection
    : public QObject,
      public Singleton<TCPNetworkConnection>,
      public std::enable_shared_from_this<TCPNetworkConnection> {

  Q_OBJECT
  friend class Singleton<TCPNetworkConnection>;
  using Callbackfunction = std::function<void(QJsonObject &&)>;

public:
  virtual ~TCPNetworkConnection();

  /*use signal to trigger data sending*/
  void
  send_data(SendNode<QByteArray, std::function<uint16_t(uint16_t)>> &&data);

private:
  TCPNetworkConnection();

  void registerNetworkEvent();
  void registerSocketSignal();
  void registerCallback();
  void registerErrorHandling();

private slots:
  void slot_establish_long_connnection();

signals:
  void signal_establish_long_connnection();

  /*return connection status to login class*/
  void signal_connection_status(bool status);

  /*login to server failed*/
  void signal_login_failed(ServiceStatus status);

  /*if login success, then switch to chatting dialog*/
  void signal_switch_chatting_dialog();

  /*
   * client sent search username request to server
   * server return result back to client
   */
  void signal_search_username(std::optional<std::shared_ptr<UserNameCard>>, ServiceStatus status);

private:
  /*establish tcp socket with server*/
  QTcpSocket m_socket;

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
