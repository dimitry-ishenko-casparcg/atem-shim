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
int main(int argc, char* argv[])
try
{
    QCoreApplication app{ argc, argv };

    auto signal_handler = [](int signal)
    {
        std::cout << "Received signal " << signal << ", exiting" << std::endl;
        QCoreApplication::quit();
    };
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "Reading command line arguments" << std::endl;

    QHostAddress atem_address;
    QHostAddress recv_address = QHostAddress::Any;
    quint16 recv_port = 1230;

    auto args = app.arguments();
    args.pop_front();

    for(auto const& arg : args)
    {
        if(arg.startsWith("--recv-address="))
        {
            recv_address = QHostAddress{ arg.mid(sizeof("--recv-address=")) };
            if(recv_address.isNull()) throw std::invalid_argument{ "Invalid recv address" };
        }
        else if(arg.startsWith("--recv-port="))
        {
            bool ok = false;
            recv_port = arg.mid(sizeof("--recv-port=")).toUInt(&ok);
            if(!ok) throw std::invalid_argument{ "Invalid recv port" };
        }
        else // atem-address
        {
            atem_address = QHostAddress{ arg };
            if(atem_address.isNull()) throw std::invalid_argument("Invalid ATEM address");
        }
    }

    Server server{ atem_address, recv_address, recv_port };
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &server, &Server::close);

    std::cout << "Entering event loop" << std::endl;
    return app.exec();
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
