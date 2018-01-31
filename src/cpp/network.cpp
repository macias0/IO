#include "network.h"
#include <QDataStream>

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
            connect(m_client.data(), SIGNAL(readyRead()), this, SLOT(readyRead()));
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

void Network::readyRead()
{
    //there could be more than 1 message at time - so while
    while(m_client->bytesAvailable())
    {
        QDataStream dataStream(m_client.data());
        if( m_messageSize == 0 )
        {
            //if not enough bytes, wait for more
            if(m_client->bytesAvailable() < sizeof(quint16))
                return;
            dataStream >> m_messageSize;
        }

        //if not enough bytes, wait for more
        if( m_client->bytesAvailable() < m_messageSize )
            return;
        emit messageReceived(m_client->read(m_messageSize));
        m_messageSize = 0; //ready for next message
    }
}


void Network::sendMessage(QByteArray &a_message)
{
    QByteArray data;
    QDataStream dataStream(data);
    dataStream << (quint16) 0;
    dataStream << a_message;
    dataStream.device()->seek(0);
    dataStream << ( data.length() - sizeof(quint16) );
    //data: (quint16)a_message.length()+a_message
    //dataLength:  sizeof(quint16) + a_message.length();
    m_client->write(data);
}
