#include "networkmanager.h"
#include <QHostAddress>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), server(nullptr), socket(nullptr), mode(None)
{
}

NetworkManager::~NetworkManager()
{
    disconnect();
    if (server) {
        server->close();
        delete server;
    }
}

void NetworkManager::startServer(quint16 port)
{
    disconnect();

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);

    if (server->listen(QHostAddress::Any, port)) {
        mode = Server;
    } else {
        emit errorOccurred("Nie można uruchomić serwera: " + server->errorString());
    }
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    disconnect();

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);

    socket->connectToHost(host, port);
    if (!socket->waitForConnected(3000)) {
        emit errorOccurred("Nie udało się połączyć: " + socket->errorString());
        return;
    }

    mode = Client;
    emit connected(socket->peerAddress().toString());
}

void NetworkManager::disconnect()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }
    if (server) {
        server->close();
        server->deleteLater();
        server = nullptr;
    }
    mode = None;
    emit disconnected();
}

NetworkManager::Mode NetworkManager::currentMode() const
{
    return mode;
}

void NetworkManager::sendMessage(const QString &message)
{
    if (socket && socket->isOpen()) {
        socket->write(message.toUtf8());
    }
}

void NetworkManager::onNewConnection()
{
    if (socket) return;

    socket = server->nextPendingConnection();
    setupSocket(socket);
    emit connected(socket->peerAddress().toString());
}

void NetworkManager::setupSocket(QTcpSocket *sock)
{
    connect(sock, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(sock, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
}

void NetworkManager::onReadyRead()
{
    if (!socket) return;

    while (socket->bytesAvailable()) {
        QString message = QString::fromUtf8(socket->readAll());
        emit messageReceived(message);
    }
}

void NetworkManager::onDisconnected()
{
    emit disconnected();
    if (socket) {
        socket->deleteLater();
        socket = nullptr;
    }
    mode = None;
}
