#include "tcpnetworkconnection.h"
#include <QDataStream>

TCPNetworkConnection::TCPNetworkConnection()
{
    /*callbacks should be registered at first(before signal)*/
    registerCallback();

    registerSocketSignal();
    registerErrorHandling();
}

TCPNetworkConnection::~TCPNetworkConnection()
{

}

void TCPNetworkConnection::registerSocketSignal()
{
    /*connected to server successfully*/
    connect(&m_socket, &QTcpSocket::connected, [this](){
        qDebug() << "connected to server successfully";
        emit connection_status(true);
    });

    /*server disconnected*/
    connect(&m_socket, &QTcpSocket::disconnected, [this](){
        qDebug() << "server disconnected";
        emit connection_status(false);
    });

    /*receive data from server*/
    connect(&m_socket, &QTcpSocket::readyRead, [this](){
        QByteArray array = m_socket.readAll();
    });

}

void TCPNetworkConnection::registerErrorHandling()
{
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            [this]([[maybe_unused]] QTcpSocket::SocketError socketErr){
        qDebug() << "Connection To Tcp error: " << m_socket.errorString();
    });
}

void TCPNetworkConnection::registerCallback()
{

}

void TCPNetworkConnection::establish_long_connnection(TCPNetworkConnection::ChattingServerInfo info)
{
    qDebug() << "Connecting to Server";
    m_server = std::move(info);
    m_socket.connectToHost(m_server.host, m_server.port);
}
