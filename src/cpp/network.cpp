#include "network.h"
#include <QDataStream>
#include <QNetworkInterface>

Network::Network(QObject *parent) : QObject(parent)
{
    connect(&m_broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcastMessage()));
    m_broadcastTimer.setInterval(m_broadcastInterval);
    m_broadcastTimer.setSingleShot(false);



    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             m_localAddress = address;
    }
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

    //configuring broadcast socket
    m_broadcastSocket.reset(new QUdpSocket);
    m_broadcastTimer.start();

    //configuring connection socket
    m_server.reset(new QTcpServer);
    connect(m_server.data(), &QTcpServer::newConnection, [this](){
        if(m_connectionSocket.isNull())
        {
            m_connectionSocket.reset(m_server->nextPendingConnection());
            m_broadcastTimer.stop();
            prepareSocket();
            m_networkState = NetworkState::Server;
        }
    });
    m_server->listen(QHostAddress::Any, m_connectionPort);
    m_networkState = NetworkState::Broadcast;


}
void Network::stopServer()
{
    m_broadcastTimer.stop();
    m_broadcastSocket->close();
    m_broadcastSocket.take()->deleteLater();
    m_server->close();
    m_server.take()->deleteLater();
    m_connectionSocket->close();
    m_connectionSocket.take()->deleteLater();
    m_networkState = NetworkState::None;
}

void Network::connectToServer()
{
    qDebug() << "Connecting to the server...";

    //configuring broadcast socket
    m_broadcastSocket.reset(new QUdpSocket);
    connect(m_broadcastSocket.data(), &QUdpSocket::readyRead, [this]()
    {
        QByteArray datagram;
        datagram.resize(m_broadcastSocket->pendingDatagramSize());
        if(m_broadcastSocket->readDatagram(datagram.data(), datagram.size()) != -1)
        {
            qDebug() << "Received datagram: " << datagram;
            m_connectionSocket.reset(new QTcpSocket);
            prepareSocket();
            m_connectionSocket->connectToHost(QHostAddress(QString(datagram)), m_connectionPort);
            m_networkState = NetworkState::Client;
        }

    });
    m_broadcastSocket->bind(m_broadcastPort);

    //configuring connection socket

    m_networkState = NetworkState::Broadcast;

}

void Network::disconnectFromServer()
{
    m_broadcastSocket->close();
    m_broadcastSocket.take()->deleteLater();
    m_connectionSocket->close();
    m_connectionSocket.take()->deleteLater();
    m_networkState = NetworkState::None;

}

void Network::readyRead()
{
    //there could be more than 1 message at time - so while
    while(m_connectionSocket->bytesAvailable())
    {
        QDataStream dataStream(m_connectionSocket.data());
        if( m_messageSize == 0 )
        {
            //if not enough bytes, wait for more

            if(m_connectionSocket->bytesAvailable() < sizeof(quint16))
                return;
            qDebug() << "Bytes before: " << m_connectionSocket->bytesAvailable();
            dataStream >> m_messageSize;
            qDebug() << "Bytes after: " << m_connectionSocket->bytesAvailable() << "Mess length: " << m_messageSize;
        }

        //if not enough bytes, wait for more
        if( m_connectionSocket->bytesAvailable() < m_messageSize )
            return;
        QByteArray mess;
        dataStream >> mess;
        qDebug() << "messageReceived: " << mess << "size: " << mess.length();
        emit messageReceived(mess);
        m_messageSize = 0; //ready for next message
    }
}


void Network::sendBroadcastMessage()
{
    qDebug() << "Sending broadcast: " << m_localAddress.toString();
    m_broadcastSocket->writeDatagram(m_localAddress.toString().toUtf8(), QHostAddress::Broadcast, m_broadcastPort );
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
    m_connectionSocket->write(data);
}

bool Network::isConnected() const
{
    return m_networkState > NetworkState::Broadcast && m_connectionSocket->state() == QAbstractSocket::ConnectedState;
}

Network::NetworkState Network::getNetworkState() const
{
    return m_networkState;
}

void Network::prepareSocket()
{

    m_broadcastSocket->close();

    connect(m_connectionSocket.data(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_connectionSocket.data(), &QTcpSocket::disconnected, [this](){
        m_connectionSocket.take()->deleteLater();
    });
    emit connected();
}
