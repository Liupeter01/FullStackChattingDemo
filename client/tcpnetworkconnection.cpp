#include "tcpnetworkconnection.h"
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>

TCPNetworkConnection::TCPNetworkConnection() : m_buffer() {
  /*callbacks should be registered at first(before signal)*/
  registerCallback();

  /*register socket connect & disconnect & data ready signals */
  registerSocketSignal();

  /*setup socket error handling slot*/
  registerErrorHandling();

  /*register connection event*/
  registerNetworkEvent();
}

TCPNetworkConnection::~TCPNetworkConnection() {}

void TCPNetworkConnection::registerNetworkEvent() {
  connect(this, &TCPNetworkConnection::signal_establish_long_connnection, this,
          &TCPNetworkConnection::slot_establish_long_connnection);
}

void TCPNetworkConnection::registerSocketSignal() {
  /*connected to server successfully*/
  connect(&m_socket, &QTcpSocket::connected, [this]() {
    qDebug() << "connected to server successfully";
    emit signal_connection_status(true);
  });

  /*server disconnected*/
  connect(&m_socket, &QTcpSocket::disconnected, [this]() {
    qDebug() << "server disconnected";
    emit signal_connection_status(false);
  });

  /*receive data from server*/
  connect(&m_socket, &QTcpSocket::readyRead, [this]() {
    QByteArray array = m_socket.readAll();
    if (m_buffer.getMessageNode().has_value()) {
      auto msg = m_buffer.getMessageNode().value();
      qDebug() << "msg_id = " << msg._msg_id
               << "msg_length = " << msg._msg_length
               << "msg_data = " << msg._msg_data;

      QJsonDocument json_obj = QJsonDocument::fromJson(msg._msg_data);
      if (json_obj.isNull()) { // converting failed
        // journal log system
        qDebug() << __FILE__ << "[FATAL ERROR]: json object is null!\n";
        emit signal_login_failed(ServiceStatus::JSONPARSE_ERROR);
        return;
      }

      if (!json_obj.isObject()) {
        // journal log system
        qDebug() << __FILE__ << "[FATAL ERROR]: json object is null!\n";
        emit signal_login_failed(ServiceStatus::JSONPARSE_ERROR);
        return;
      }

      /*to prevent app crash due to callback is not exists*/
      try {
        m_callbacks[static_cast<ServiceType>(msg._msg_id)](
            std::move(json_obj.object()));
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

bool TCPNetworkConnection::checkJsonForm(const QJsonObject &json) {
  if (!json.contains("error")) {
    qDebug() << "Json Parse Error!";
    emit signal_login_failed(ServiceStatus::JSONPARSE_ERROR);
    return false;
  }
  if (json["error"].toInt() !=
      static_cast<int>(ServiceStatus::SERVICE_SUCCESS)) {
    qDebug() << "Login Server Error!";
    emit signal_login_failed(static_cast<ServiceStatus>(json["error"].toInt()));
    return false;
  }
  return true;
}

void TCPNetworkConnection::registerCallback() {
  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_LOGINRESPONSE, [this](QJsonObject &&json) {
        /*error occured!*/
        if (!checkJsonForm(json)) {
          return;
        }
        emit signal_switch_chatting_dialog();
      }));

  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_2, [this](QJsonObject &&json) {

      }));

  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_3, [this](QJsonObject &&json) {

      }));

  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_RESERVE_4, [this](QJsonObject &&json) {

      }));
}

void TCPNetworkConnection::slot_establish_long_connnection(
    TCPNetworkConnection::ChattingServerInfo info) {
  qDebug() << "Connecting to Server" << "host = \n"
           << info.host << "ip = " << info.port;

  m_server = std::move(info);
  m_socket.connectToHost(m_server.host, m_server.port);
}

void TCPNetworkConnection::send_data(SendNode<QByteArray> &&data) {
    QByteArray send_buffer;
    QDataStream ds(&send_buffer, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::BigEndian);

    ds << data.getMessageID() << data.getTotalLenth();
    send_buffer.append(data.getMessage());
    m_socket.write(send_buffer);
}
