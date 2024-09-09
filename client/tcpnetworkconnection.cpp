#include "tcpnetworkconnection.h"
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include "useraccountmanager.hpp"

TCPNetworkConnection::TCPNetworkConnection()
    : m_buffer([](auto x) { return qToLittleEndian(x); }) {
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

    /*make sure header is full*/
    if (m_buffer.check_header_remaining()) {
      [[maybe_unused]] auto res = m_buffer.insert_header(array);
      return;
    }

    /*insert body until it meets the requirement of the length*/
    if (!m_buffer.insert_body(array)) {
      return;
    }

    /*record them temporarily*/
    m_received._id = m_buffer.get_id().value();
    m_received._length = m_buffer.get_length().value();
    m_received._msg = m_buffer.get_msg_body().value();

    qDebug() << "msg_id = " << m_received._id
             << "msg_length = " << m_received._length
             << "msg_data = " << m_received._msg;

    /*erase current packet(include header and body)*/
    m_buffer.clear();

    /*parse it as json*/
    QJsonDocument json_obj = QJsonDocument::fromJson(m_received._msg);
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
      m_callbacks[static_cast<ServiceType>(m_received._id)](
          std::move(json_obj.object()));
    } catch (const std::exception &e) {
      qDebug() << e.what();
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

void TCPNetworkConnection::slot_establish_long_connnection() {

  qDebug() << "Connecting to Server"
             << "\nuuid = " << UserAccountManager::get_instance()->get_uuid()
             << "\nhost = " << UserAccountManager::get_instance()->get_host()
             << "\nport = " << UserAccountManager::get_instance()->get_port()
           << "\ntoken = " << UserAccountManager::get_instance()->get_token() << '\n';

  m_socket.connectToHost(
      UserAccountManager::get_instance()->get_host(),
        UserAccountManager::get_instance()->get_port().toUShort()
    );
}

void TCPNetworkConnection::send_data(
    SendNode<QByteArray, std::function<uint16_t(uint16_t)>> &&data) {
  m_socket.write(data.get_buffer());
}
