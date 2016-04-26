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
    if (this->hasStreamConnection)
    {
        LOG(ERROR) << "WARNING: A new control connection request came in, but we are already connected.";
    }
        this->streamSocket = this->streamServer->nextPendingConnection();
        connect(this->streamSocket, &QTcpSocket::bytesWritten, [=](){
                VLOG(4) << "Stream socket bytesWritten signal received.";
                emit this->sentStreamData();});

        connect(this->streamSocket, &QTcpSocket::disconnected, [=](){
            VLOG(2) << "Stream socket has been disconnected.";
            this->hasStreamConnection = false;
            emit this->connectionStatusChanged();
            emit this->streamSocketDisconnected();});

        this->hasStreamConnection = true;
    emit this->connectionStatusChanged();
        VLOG(2) << "Connected to stream socket.";
}

void NetworkInterface::establishControlSocket()
{
    VLOG(2) << "New control socket request received.";
    if (this->hasControlConnection)
    {
        LOG(ERROR) << "WARNING: A new control connection request came in, but we are already connected.";
    }
        this->controlSocket = this->controlServer->nextPendingConnection();
        connect(this->controlSocket, &QTcpSocket::bytesWritten, [=](){
                    VLOG(4) << "Control socket bytesWritten signal received.";
                    emit this->sentControlData();});

        connect(this->controlSocket, &QTcpSocket::disconnected, [=](){
            VLOG(2) << "Control socket has been disconnected.";
            this->hasControlConnection = false;
            emit this->connectionStatusChanged();
            emit this->controlSocketDisconnected();});

        this->hasControlConnection = true;
    emit this->connectionStatusChanged();
        VLOG(2) << "Connected to control socket.";

}


void NetworkInterface::sendStreamSamples(int samplesCount, SAMPLE* samplesBuffer)
{
    this->streamSocket->write((char *)samplesBuffer, (qint64)(samplesCount* sizeof(SAMPLE)/ sizeof(char)));

}

void NetworkInterface::sendCommand(control_command_t command)
{
    VLOG(2) << "Sending command to client.";

    this->controlSocket->write((char*)&command, (qint64) sizeof(command));
    this->controlSocket->flush();

}

#pragma clang diagnostic pop

