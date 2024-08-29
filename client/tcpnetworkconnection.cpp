#include "tcpnetworkconnection.h"
#include <QDataStream>

TCPNetworkConnection::TCPNetworkConnection()
    :m_buffer()
{
  /*callbacks should be registered at first(before signal)*/
  registerCallback();

  /*register socket connect & disconnect & data ready signals */
  registerSocketSignal();

  /*setup socket error handling slot*/
  registerErrorHandling();
}

TCPNetworkConnection::~TCPNetworkConnection() {}

void TCPNetworkConnection::registerSocketSignal() {
  /*connected to server successfully*/
  connect(&m_socket, &QTcpSocket::connected, [this]() {
    qDebug() << "connected to server successfully";
    emit connection_status(true);
  });

  /*server disconnected*/
  connect(&m_socket, &QTcpSocket::disconnected, [this]() {
    qDebug() << "server disconnected";
    emit connection_status(false);
  });

  /*receive data from server*/
  connect(&m_socket, &QTcpSocket::readyRead, [this]() {
    QByteArray array = m_socket.readAll();
    if (m_buffer.getMessageNode().has_value()) {
      auto msg = m_buffer.getMessageNode().value();
      qDebug() << "msg_id = " << msg._msg_id
               << "msg_length = " << msg._msg_length
               << "msg_data = " << msg._msg_data;

      /*to prevent app crash due to callback is not exists*/
      try {
        m_callbacks[static_cast<ServiceType>(msg._msg_id)](
            std::move(msg._msg_data));
      } catch (const std::exception &e) {
        qDebug() << e.what();
      }
    }
  });
}

void TCPNetworkConnection::registerErrorHandling() {
  connect(
      &m_socket,
      QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
      [this]([[maybe_unused]] QTcpSocket::SocketError socketErr) {
        qDebug() << "Connection To Tcp error: " << m_socket.errorString();
      });
}

void TCPNetworkConnection::registerCallback() {
  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_1, [this](QByteArray &&container) {
        // container.data();
      }));

  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_2, [this](QByteArray &&container) {

      }));

  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_3, [this](QByteArray &&container) {

      }));

  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_4, [this](QByteArray &&container) {

      }));
}

void TCPNetworkConnection::establish_long_connnection(
    TCPNetworkConnection::ChattingServerInfo info) {
  qDebug() << "Connecting to Server";
  m_server = std::move(info);
  m_socket.connectToHost(m_server.host, m_server.port);
}
