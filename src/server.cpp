////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "connection.hpp"
#include "server.hpp"

#include <QTcpSocket>

#include <iostream>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
Server::Server(const QHostAddress& atem_address, const QHostAddress& recv_address, quint16 recv_port, QObject* parent) :
    QObject(parent),
    atem_address_(atem_address)
{
    std::cout << "Creating Server" << std::endl;

    connect(&server_, &QTcpServer::newConnection, this, &Server::connection);

    std::cout << "Listening on " << recv_address.toString().toStdString() << ':' << recv_port << std::endl;
    server_.listen(recv_address, recv_port);

    if(!server_.isListening()) throw std::runtime_error("Failed to set up listen socket");
}

////////////////////////////////////////////////////////////////////////////////
Server::~Server()
{
    std::cout << "Destroying Server" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void Server::connection()
{
    while(server_.hasPendingConnections())
    {
        auto socket = server_.nextPendingConnection();
        if(socket)
        {
            std::cout << "Accepted connection from " << socket->peerAddress().toString().toStdString() << ':' << socket->peerPort() << std::endl;

            auto connection = new Connection(atem_address_, socket, this);
            connect(connection, &Connection::aboutToClose, connection, &Connection::deleteLater);
        }
    }
}
