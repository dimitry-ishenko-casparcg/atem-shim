////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SERVER_HPP
#define SERVER_HPP

#include <QHostAddress>
#include <QObject>
#include <QTcpServer>

////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
#endif
