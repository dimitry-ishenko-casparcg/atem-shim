#include "connection.hpp"
#include <iostream>

Connection::Connection(const QHostAddress& atem_address, QTcpSocket* socket, QObject* parent) :
    QObject(parent),
    atem_address_(atem_address),
    socket_(socket)
{
    std::cout << "Creating Connection" << std::endl;

    socket->setParent(this);
    connect(socket, &QTcpSocket::readyRead, this, &Connection::socket_read);
    connect(socket, &QTcpSocket::disconnected, this, &Connection::close);

    connect(&atem_, &QAtemConnection::connected, this, &Connection::atem_connected);
    connect(&atem_, &QAtemConnection::disconnected, this, &Connection::atem_disconnected);

    atem_connect();
}

Connection::~Connection()
{
    std::cout << "Destroying Connection" << std::endl;
}

void Connection::socket_read()
{
    while(socket_->canReadLine())
    {
        auto read = socket_->readLine();
        read.chop(1);

        if(read == "rc") atem_connect();
        else if(me_)
        {
                 if(read == "tr") me_->autoTransition();
            else if(read == "ct") me_->cut();
            else if(read == "fb") me_->toggleFadeToBlack();
            else if(read == "pg=?")
                socket_->write("pg=" + QByteArray::number(me_->programInput()) + '\n');
            else if(read == "pv=?")
                socket_->write("pv=" + QByteArray::number(me_->previewInput()) + '\n');
            else if(read.startsWith("pg_"))
            {
                bool ok = false;
                auto num = read.mid(3).toUInt(&ok);
                if(ok) me_->changeProgramInput(num);
            }
            else if(read.startsWith("pv_"))
            {
                bool ok = false;
                auto num = read.mid(3).toUInt(&ok);
                if(ok) me_->changePreviewInput(num);
            }
        }
    }
}

void Connection::close()
{
    std::cout << "Closing" << std::endl;
    emit aboutToClose();

    socket_->close();
    atem_.disconnectFromSwitcher();
}

void Connection::atem_connect()
{
    std::cout << "Connecting to ATEM on " << atem_address_.toString().toStdString() << std::endl;
    atem_.connectToSwitcher(atem_address_);
}

void Connection::atem_connected()
{
    std::cout << "Connected to ATEM" << std::endl;

    me_ = atem_.mixEffect(0);
    if(me_)
    {
        std::cout << "Found M/E 0" << std::endl;
        connect(me_, &QAtemMixEffect::previewInputChanged, this, &Connection::preview_changed);
        connect(me_, &QAtemMixEffect::programInputChanged, this, &Connection::program_changed);
    }

    socket_->write("cn\n");
}

void Connection::atem_disconnected()
{
    std::cout << "Disconnected from ATEM" << std::endl;

    me_ = nullptr;
    socket_->write("dc\n");
}

void Connection::program_changed(quint8, quint16, quint16 num)
{
    socket_->write("pg=" + QByteArray::number(num) + '\n');
}

void Connection::preview_changed(quint8, quint16, quint16 num)
{
    socket_->write("pv=" + QByteArray::number(num) + '\n');
}
