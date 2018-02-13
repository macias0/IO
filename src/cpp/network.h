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

    enum NetworkState
    {
        None = 0,
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
    void stopServer();

    void connectToServer(QHostAddress a_address);
    void disconnectFromServer();


    void sendMessage(const QByteArray &a_message);

    bool isConnected() const;

    NetworkState getNetworkState() const;


signals:
    void messageReceived(const QByteArray &a_message);
    void connected();


private slots:
    void readyRead();

private:
    //for singleton
    explicit Network(QObject *parent = nullptr);
    Network(const Network&) = delete;
    void operator=(const Network&) = delete;

    static const quint16 m_port = 6969;



    NetworkState m_networkState;

    QScopedPointer<QTcpServer> m_server;
    QScopedPointer<QTcpSocket> m_socket;

    quint16 m_messageSize = 0;


    void prepareSocket();

};

#endif // NETWORK_H
