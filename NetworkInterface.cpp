//
// Created by Bryce Cater on 4/23/16.
//

#include "NetworkInterface.h"

NetworkInterface::NetworkInterface(QObject *parent, QHostAddress address, quint16 port) : QTcpServer(parent)
{
    LOG(DEBUG) << this->listen(address, port);
}

void NetworkInterface::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        LOG(WARNING) << "Socket conection failed!";
        return;
    }

//
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out << (quint16)0;
//    out << text;
//    out.device()->seek(0);
//    out << (quint16)(block.size() - sizeof(quint16));
//    QString fortune = fortunes.at(qrand() % fortunes.size());
    AudioStreamThread *thread = new AudioStreamThread(socketDescriptor, fortune, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

AudioStreamThread::AudioStreamThread(int socketDescriptor, AudioSource &source, QObject *parent)
{

}

void AudioStreamThread::run()
{

}

AudioControlThread::AudioControlThread(int socketDescriptor, const QString &fortune, QObject *parent)
{

}

void AudioControlThread::run()
{

}
