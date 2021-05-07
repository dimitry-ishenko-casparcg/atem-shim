////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <qatemconnection.h>
#include <qatemmixeffect.h>

#include <QHostAddress>
#include <QMetaObject>
#include <QObject>
#include <QTcpSocket>

////////////////////////////////////////////////////////////////////////////////
class Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection(const QHostAddress& atem_address, QTcpSocket* socket, QObject* parent = nullptr);
    ~Connection();

signals:
    void aboutToClose();

public slots:
    void close();

private slots:
    void socket_read();

    void atem_connected();
    void atem_disconnected();

    void program_changed(quint8 me, quint16 from, quint16 to);
    void preview_changed(quint8 me, quint16 from, quint16 to);

private:
    QHostAddress atem_address_;
    QTcpSocket* socket_;

    QAtemConnection atem_;
    QAtemMixEffect* me_ = nullptr;

    void atem_connect();
};

////////////////////////////////////////////////////////////////////////////////
#endif
