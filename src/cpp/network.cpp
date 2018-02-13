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
    qDebug() << "Starting the server...";
    m_server.reset(new QTcpServer);
    connect(m_server.data(), &QTcpServer::newConnection, [this](){
        if(m_socket.isNull())
        {
            m_socket.reset(m_server->nextPendingConnection());
            prepareSocket();
        }
    });
    m_server->listen(QHostAddress::Any, m_port);
    m_networkState = NetworkState::Server;
}
void Network::stopServer()
{
    m_server->close();
    m_server.take()->deleteLater();
    m_socket->close();
    m_socket.take()->deleteLater();
    m_networkState = NetworkState::None;
}

void Network::connectToServer(QHostAddress a_address)
{
    qDebug() << "Connecting to the server...";
    m_socket.reset(new QTcpSocket);
    prepareSocket();
    m_socket->connectToHost(a_address, m_port);
    m_networkState = NetworkState::Client;
}

void Network::disconnectFromServer()
{
    m_socket->close();
    m_socket.take()->deleteLater();
    m_networkState = NetworkState::None;

}

void Network::readyRead()
{
    //there could be more than 1 message at time - so while
    while(m_socket->bytesAvailable())
    {
        QDataStream dataStream(m_socket.data());
        if( m_messageSize == 0 )
        {
            //if not enough bytes, wait for more

            if(m_socket->bytesAvailable() < sizeof(quint16))
                return;
            qDebug() << "Bytes before: " << m_socket->bytesAvailable();
            dataStream >> m_messageSize;
            qDebug() << "Bytes after: " << m_socket->bytesAvailable() << "Mess length: " << m_messageSize;
        }

        //if not enough bytes, wait for more
        if( m_socket->bytesAvailable() < m_messageSize )
            return;
        QByteArray mess;
        dataStream >> mess;
        qDebug() << "messageReceived: " << mess << "size: " << mess.length();
        emit messageReceived(mess);
        m_messageSize = 0; //ready for next message
    }
}




void Network::sendMessage(const QByteArray &a_message)
{
    qDebug() << "Sending message: " << a_message << "size: " << a_message.length();
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);
    dataStream << (quint16)0;
    qDebug() << "Po sizie" << data.length();
    dataStream << a_message;
    qDebug() << "Po wpisaniu message: " << data.length();
    dataStream.device()->seek(0);
    dataStream << (quint16)( data.length() - sizeof(quint16) );
    //data: (quint16)a_message.length()+a_message
    //dataLength:  sizeof(quint16) + a_message.length();
    qDebug() << "Sending stream: " << data << "Meslength: " << (quint16)(quint16)( data.length() - sizeof(quint16)) << "absolute length: " << data.length();
    m_socket->write(data);
}

bool Network::isConnected() const
{
    return m_networkState != NetworkState::None && m_socket->state() == QAbstractSocket::ConnectedState;
}

Network::NetworkState Network::getNetworkState() const
{
    return m_networkState;
}

void Network::prepareSocket()
{
    connect(m_socket.data(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_socket.data(), &QTcpSocket::disconnected, [this](){
        m_socket.take()->deleteLater();
    });
    emit connected();
}
