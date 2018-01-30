#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}


Network::~Network()
{
    if(m_networkState == Client)
        disconnectFromServer();
    else if(m_networkState == Server)
        stopServer();
}

void Network::startServer()
{
    m_server.reset(new QTcpServer);
    connect(m_server.data(), &QTcpServer::newConnection, [this](){
        if(m_client.isNull())
        {
            m_client.reset(m_server->nextPendingConnection());
            connect(m_client.data(), &QTcpSocket::disconnected, [this](){
                m_client.take()->deleteLater();
            });
        }
    });

    m_server->listen(QHostAddress::Any, m_port);
}
void Network::stopServer()
{
    m_server->close();
    m_server.take()->deleteLater();
    m_client->close();
    m_client.take()->deleteLater();
}

void Network::connectToServer(QHostAddress a_address)
{
    m_client.reset(new QTcpSocket);
    m_client->connectToHost(a_address, m_port);
}

void Network::disconnectFromServer()
{
    m_client->close();
    m_client.take()->deleteLater();

}


void Network::sendMessage(QByteArray &a_message)
{

}
