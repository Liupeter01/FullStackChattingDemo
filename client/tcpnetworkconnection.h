#ifndef TCPNETWORKCONNECTION_H
#define TCPNETWORKCONNECTION_H

#include "singleton.hpp"
#include <QTcpSocket>
#include <QObject>               //connect
#include <QString>
#include <QUrl>

class TCPNetworkConnection
    : public QObject
    , public Singleton<TCPNetworkConnection>
    , public std::enable_shared_from_this<TCPNetworkConnection>
{

    Q_OBJECT
    friend class Singleton<TCPNetworkConnection>;

public:
    struct ChattingServerInfo{
        std::size_t uuid;
        QString host;
        QString port;
        QString token;
    };

    TCPNetworkConnection();

private:
    ~TCPNetworkConnection();
    void registerSignal();

private slots:
    void establish_long_connnection(TCPNetworkConnection::ChattingServerInfo info);

private:
    QTcpSocket m_socket;
    ChattingServerInfo m_server;
};

#endif // TCPNETWORKCONNECTION_H
