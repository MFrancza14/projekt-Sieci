#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    enum Mode { None, Server, Client };

    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    Mode currentMode() const;
    void connectToServer(const QString &host, quint16 port);
    void disconnect();
    void startServer(quint16 port);

    void sendMessage(const QString &message);

signals:
    void connected(const QString &peerAddress);
    void disconnected();
    void messageReceived(const QString &message);
    void errorOccurred(const QString &error);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    Mode mode;

    void setupSocket(QTcpSocket *sock);
};

#endif // NETWORKMANAGER_H

