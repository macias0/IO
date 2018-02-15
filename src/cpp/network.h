#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>


class Network : public QObject
{
    Q_OBJECT
public:

    enum NetworkState
    {
        None = 0,
        Broadcast,
        Client,
        Server
    };


    static Network& getSingleton()
    {
        static Network singleton;
        return singleton;
    }
    ~Network();


    void startServer();
    void stop();
    void connectToServer();
    void sendMessage(const QByteArray &a_message);
    bool isConnected() const;
    NetworkState getNetworkState() const;


signals:
    void messageReceived(const QByteArray &a_message);
    void connected();


private slots:
    void readyRead();
    void sendBroadcastMessage();

private:
    //for singleton
    explicit Network(QObject *a_parent = nullptr);
    Network(const Network&) = delete;
    void operator=(const Network&) = delete;


    static const quint16 m_connectionPort = 6969;
    static const quint16 m_broadcastPort = 6968;
    static const int m_broadcastInterval = 1000; //ms


    NetworkState m_networkState;
    QHostAddress m_localAddress;
    QScopedPointer<QTcpServer> m_server;
    QScopedPointer<QTcpSocket> m_connectionSocket;
    QScopedPointer<QUdpSocket> m_broadcastSocket;
    QTimer m_broadcastTimer;
    quint16 m_messageSize = 0;


    void prepareSocket();

};

#endif // NETWORK_H
