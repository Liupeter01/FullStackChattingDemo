#ifndef TCPNETWORKCONNECTION_H
#define TCPNETWORKCONNECTION_H

#include "singleton.hpp"
#include <QTcpSocket>
#include <QObject>               //connect
#include <QString>
#include <QUrl>
#include "def.hpp"

class TCPNetworkConnection
    : public QObject
    , public Singleton<TCPNetworkConnection>
    , public std::enable_shared_from_this<TCPNetworkConnection>
{

    Q_OBJECT
    friend class Singleton<TCPNetworkConnection>;

public:
    struct ChattingServerInfo{
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
    void establish_long_connnection(TCPNetworkConnection::ChattingServerInfo info);

signals:
    /*return connection status to login class*/
    void connection_status(bool status);

private:
    QTcpSocket m_socket;
    ChattingServerInfo m_server;
};

#endif // TCPNETWORKCONNECTION_H
