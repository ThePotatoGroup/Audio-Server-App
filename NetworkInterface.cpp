//
// Created by Bryce Cater on 4/23/16.
//

#include "NetworkInterface.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types"
NetworkInterface::NetworkInterface(QHostAddress address, quint16 port)
{
    // INIT
    this->hasControlConnection = false;
    this->hasStreamConnection = false;

    this->streamServer = new QTcpServer();
    this->controlServer = new QTcpServer();

    this->streamSocket = new QTcpSocket;
    this->controlSocket = new QTcpSocket;


    // CONFIG
    this->streamServer->listen(address, port);
    this->controlServer->listen(address, port + quint16(1));

    // BINDING
    connect(this->streamServer, SIGNAL(newConnection()), this, SLOT(establishStreamSocket()));
    connect(this->controlServer, SIGNAL(newConnection()), this, SLOT(establishControlSocket()));
}

void NetworkInterface::establishStreamSocket()
{
    VLOG(2) << "New stream socket request received.";
    if (!this->hasStreamConnection)
    {
        this->streamSocket = this->streamServer->nextPendingConnection();
        connect(this->streamSocket, &QTcpSocket::bytesWritten, [=](){
                VLOG(4) << "Stream socket bytesWritten signal received.";
                emit this->sentStreamData();});
        this->hasStreamConnection = true;
        VLOG(2) << "Connected to stream socket.";
    }
    else
    {
        LOG(ERROR) << "ERROR: A new stream connection request came in, but we are already connected.";
    }
}

void NetworkInterface::establishControlSocket()
{
    VLOG(2) << "New control socket request received.";
    if (!this->hasControlConnection)
    {
        this->controlSocket = this->controlServer->nextPendingConnection();
        connect(this->controlSocket, &QTcpSocket::bytesWritten, [=](){
                    VLOG(4) << "Control socket bytesWritten signal received.";
                    emit this->sentControlData();});
        this->hasControlConnection = true;
        VLOG(2) << "Connected to control socket.";
    }
    else
    {
        LOG(ERROR) << "ERROR: A new control connection request came in, but we are already connected.";
    }
}


void NetworkInterface::sendStreamSamples(int samplesCount, SAMPLE* samplesBuffer)
{
    this->streamSocket->write((char *)samplesBuffer, (qint64)(samplesCount* sizeof(SAMPLE)/ sizeof(char)));

}

#pragma clang diagnostic pop