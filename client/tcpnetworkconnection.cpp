#include "tcpnetworkconnection.h"
#include "UserNameCard.h"
#include "UserFriendRequest.hpp"
#include "useraccountmanager.hpp"
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>

TCPNetworkConnection::TCPNetworkConnection()
    : m_buffer([](auto x) { return qFromBigEndian(x); }) {
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
    while (m_socket.bytesAvailable() > 0) {
      QByteArray array = m_socket.readAll(); // Read all available data

      /*
       * Ensure the received data is large enough to include the header
       * if no enough data, then continue waiting
       */
      while (array.size() >= m_buffer.get_header_length()) {

        // Check if we are still receiving the header
        if (m_buffer.check_header_remaining()) {

          /*
           * Take the necessary portion from the array for the header
           * Insert the header data into the buffer
           */
          m_buffer._buffer = array.left(m_buffer.get_header_length());
          m_buffer.update_pointer_pos(m_buffer.get_header_length());

          m_received._id = m_buffer.get_id().value();
          m_received._length = m_buffer.get_length().value();

          // Clear the header part from the array
          array.remove(0, m_buffer.get_header_length());
        }

        if (array.size() < m_received._length) {
          return;
        }

        // If we have remaining data in array, treat it as body
        if (m_buffer.check_body_remaining()) {

          std::memcpy(m_buffer.get_body_base(), array.data(),
                      m_received._length);

          m_buffer.update_pointer_pos(m_received._length);

          /*
           * Clear the body part from the array
           * Maybe there are some other data inside
           */
          array.remove(0, m_received._length);
        }
      }

      // Now, both the header and body are fully received
      m_received._msg = m_buffer.get_msg_body().value();

      // Debug output to show the received message
      qDebug() << "msg_id = " << m_received._id << "\n"
               << "msg_length = " << m_received._length << "\n"
               << "msg_data = " << m_received._msg << "\n";

      // Clear the buffer for the next message
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
      ServiceType::SERVICE_LOGINRESPONSE, [this](QJsonObject &&json) {
        /*error occured!*/
        if (!json.contains("error")) {
          qDebug() << "Json Parse Error!";
          emit signal_login_failed(ServiceStatus::JSONPARSE_ERROR);
          return;
        }
        if (json["error"].toInt() !=
            static_cast<int>(ServiceStatus::SERVICE_SUCCESS)) {
          qDebug() << "Login Server Error!";
          emit signal_login_failed(
              static_cast<ServiceStatus>(json["error"].toInt()));
          return;
        }

        emit signal_switch_chatting_dialog();
      }));

  /*Client search username and server return result back*/
  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_SEARCHUSERNAMERESPONSE, [this](QJsonObject &&json) {
        /*error occured!*/
        if (!json.contains("error")) {
          qDebug() << "Json Parse Error!";

          emit signal_search_username(std::nullopt,
                                      ServiceStatus::JSONPARSE_ERROR);
          return;
        } else if (json["error"].toInt() !=
                   static_cast<int>(ServiceStatus::SERVICE_SUCCESS)) {
          qDebug() << "Login Server Error!";

          emit signal_search_username(
              std::nullopt, static_cast<ServiceStatus>(json["error"].toInt()));
          return;
        } else {
          auto uuid = json["uuid"].toString();
          auto username = json["username"].toString();
          auto nickname = json["nickname"].toString();
          auto avator = json["avator"].toString();
          auto description = json["description"].toString();
          auto sex = static_cast<Sex>(json["sex"].toInt());

          qDebug() << "Retrieve Data From Server of uuid = " << uuid << ":"
                   << "username = " << username << '\n'
                   << "nickname = " << nickname << '\n'
                   << "avator = " << avator << '\n'
                   << "description = " << description << '\n';

          emit signal_search_username(
              std::make_shared<UserNameCard>(uuid, avator, username, nickname,
                                             description, sex),
              ServiceStatus::SERVICE_SUCCESS);
        }
      }));

  /*Client send friend request to other*/
  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_FRIENDREQUESTRESPONSE, [this](QJsonObject &&json) {
        /*error occured!*/
        if (!json.contains("error")) {
          qDebug() << "Json Parse Error!";
          // emit
          return;

        } else if (json["error"].toInt() !=
                   static_cast<int>(ServiceStatus::SERVICE_SUCCESS)) {
          qDebug() << "Friend Request Send Failed!";
          // emit
          return;
        }

        qDebug() << "Friend Request Send Successfully!";

        json["error"].toString();
        json["src_uuid"].toString();
        json["dst_uuid"].toString();
      }));

  /*the person who is going to receive friend request*/
  m_callbacks.insert(std::pair<ServiceType, Callbackfunction>(
      ServiceType::SERVICE_FRIENDREINCOMINGREQUEST, [this](QJsonObject &&json) {
          /*error occured!*/
          if (!json.contains("error")) {
              qDebug() << "Json Parse Error!";

              // emit
              return;
          } else if (json["error"].toInt() !=
                     static_cast<int>(ServiceStatus::SERVICE_SUCCESS)) {
              qDebug() << "Receive Friend Request Send Failed!";

              // emit
              return;
          }

          qDebug() << "Receive Friend Request Send Successfully!";

          QString src = json["src_uuid"].toString();
          QString dst = json["dst_uuid"].toString();
          QString nickname = json["nickname"].toString();
          QString req_msg = json["message"].toString();

          auto request =std::make_unique<UserFriendRequest>(src,dst,nickname,req_msg);

      }));
}

void TCPNetworkConnection::slot_establish_long_connnection() {

  qDebug() << "Connecting to Server"
           << "\nuuid = " << UserAccountManager::get_instance()->get_uuid()
           << "\nhost = " << UserAccountManager::get_instance()->get_host()
           << "\nport = " << UserAccountManager::get_instance()->get_port()
           << "\ntoken = " << UserAccountManager::get_instance()->get_token()
           << '\n';

  /*the successful or unsuccessful signal is going to generate in
   * signal<->slot*/
  m_socket.connectToHost(
      UserAccountManager::get_instance()->get_host(),
      UserAccountManager::get_instance()->get_port().toUShort());
}

void TCPNetworkConnection::send_data(
    SendNode<QByteArray, std::function<uint16_t(uint16_t)>> &&data) {
  m_socket.write(data.get_buffer());
}
