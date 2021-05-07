#ifndef SERVER_HPP
#define SERVER_HPP

#include <QHostAddress>
#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(const QHostAddress& atem_address, const QHostAddress& recv_address, quint16 recv_port, QObject* parent = nullptr);
    ~Server();

public slots:
    void close() { server_.close(); }

private slots:
    void connection();

private:
    QTcpServer server_;
    QHostAddress atem_address_;
};

#endif
