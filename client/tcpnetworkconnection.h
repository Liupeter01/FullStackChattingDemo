#ifndef TCPNETWORKCONNECTION_H
#define TCPNETWORKCONNECTION_H

#include "def.hpp"
#include <QByteArray>
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
  using Callbackfunction = std::function<void(QByteArray &&)>;

public:
  struct ChattingServerInfo {
    QString uuid;
    QString host;
    std::size_t port;
    QString token;
  };

  TCPNetworkConnection();

private:
  ~TCPNetworkConnection();
  void registerSocketSignal();
  void registerCallback();
  void registerErrorHandling();

private slots:
  void
  establish_long_connnection(TCPNetworkConnection::ChattingServerInfo info);

signals:
  /*return connection status to login class*/
  void connection_status(bool status);

private:
  /*establish tcp socket with server*/
  QTcpSocket m_socket;

  /*save server connection info*/
  ChattingServerInfo m_server;

  /*create a connection buffer to store the data transfer from server*/
  RecvNode<QByteArray> m_buffer;

  std::map<ServiceType, Callbackfunction> m_callbacks;
};

#endif // TCPNETWORKCONNECTION_H
