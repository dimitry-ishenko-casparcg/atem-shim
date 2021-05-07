////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "server.hpp"

#include <QCoreApplication>
#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QStringList>

#include <csignal>
#include <iostream>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
bool contains(QStringList& args, const QString& arg, QString& value)
{
    int index = args.indexOf(arg);
    if(index < 0) return false;

    value = (index < args.size() - 1) ? args[index + 1] : QString();

    args.removeAt(index);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    QCoreApplication app(argc, argv);

    auto signal_handler = [](int signal)
    {
        std::cout << "Received signal " << signal << ", exiting" << std::endl;
        QCoreApplication::quit();
    };
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "Reading command line arguments" << std::endl;
    QStringList args = QCoreApplication::arguments();

    QString value;

    // check --atem-address option (mandatory)
    QHostAddress atem_address;
    if(contains(args, "--atem-address", value))
    {
        atem_address = QHostAddress(value);
        if(atem_address.isNull()) throw std::invalid_argument("Invalid ATEM address");
    }
    else throw std::invalid_argument("Missing --atem-address option");

    // check --recv-address option (optional)
    QHostAddress recv_address = QHostAddress::Any;
    if(contains(args, "--recv-address", value))
    {
        recv_address = QHostAddress(value);
        if(recv_address.isNull()) throw std::invalid_argument("Invalid recv address");
    }

    // check --recv-port option (optional)
    quint16 recv_port = 1230;
    if(contains(args, "--recv-port", value))
    {
        bool ok = false;
        recv_port = value.toUInt(&ok);
        if(!ok) throw std::invalid_argument("Invalid recv port");
    }

    Server server(atem_address, recv_address, recv_port);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &server, &Server::close);

    std::cout << "Entering event loop" << std::endl;
    return app.exec();
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
