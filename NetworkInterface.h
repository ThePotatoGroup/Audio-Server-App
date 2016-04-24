//
// Created by Bryce Cater on 4/23/16.
//

#ifndef AUDIO_SERVER_APP_NETWORKINTERFACE_H
#define AUDIO_SERVER_APP_NETWORKINTERFACE_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QThread>
#include "AudioSource.h"
#include "easylogging++.h"

class NetworkInterface : public QTcpServer
{
    Q_OBJECT

public:
    NetworkInterface(QObject *parent, QHostAddress address, quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:
};



class AudioStreamThread : public QThread
{
    Q_OBJECT

public:
    AudioStreamThread(int socketDescriptor, AudioSource &source, QObject *parent);

    void run() Q_DECL_OVERRIDE;

    signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    AudioSource source;
};

class AudioControlThread : public QThread
{
Q_OBJECT

public:
    AudioControlThread(int socketDescriptor, const QString &fortune, QObject *parent);

    void run() Q_DECL_OVERRIDE;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    AudioSource source;
};

#endif //AUDIO_SERVER_APP_NETWORKINTERFACE_H
