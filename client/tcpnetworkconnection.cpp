#include "tcpnetworkconnection.h"
#include <QDataStream>

TCPNetworkConnection::TCPNetworkConnection()
{

}

TCPNetworkConnection::~TCPNetworkConnection()
{

}

void TCPNetworkConnection::registerSignal()
{

}

void TCPNetworkConnection::establish_long_connnection(TCPNetworkConnection::ChattingServerInfo info)
{
    m_server = std::move(info);
}
