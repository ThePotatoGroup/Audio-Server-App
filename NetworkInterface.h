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

class NetworkInterface : public QObject
{
    Q_OBJECT

public:
    NetworkInterface(QHostAddress address, quint16 port);

    bool hasStreamConnection;
    bool hasControlConnection;

    public slots:

    void establishStreamSocket();
    void establishControlSocket();

    void sendStreamSamples(int samplesCount, SAMPLE* samplesBuffer);

signals:

    void sentStreamData();
    void sentControlData();

protected:
    AudioSource* audioSource;

    QTcpServer*streamServer;
    QTcpServer*controlServer;

    QTcpSocket* streamSocket;
    QTcpSocket* controlSocket;

private:
};


#endif //AUDIO_SERVER_APP_NETWORKINTERFACE_H
