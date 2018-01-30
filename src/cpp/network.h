#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
//#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>





class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    ~Network();

    void startServer();
    void stopServer();

    void connectToServer(QHostAddress a_address);
    void disconnectFromServer();


    void sendMessage(QByteArray &a_message);


signals:
    void messageReceived(QByteArray a_message);


//private slots:
//    void newConnection();

private:

    static const quint16 m_port = 6969;

    enum NetworkState
    {
        None = 0,
        Client,
        Server
    };

    NetworkState m_networkState;

    QScopedPointer<QTcpServer> m_server;
    QScopedPointer<QTcpSocket> m_client;



};

#endif // NETWORK_H
