//
// Created by Bryce Cater on 4/23/16.
//

#ifndef AUDIO_SERVER_APP_HOSTINPUTWINDOW_H
#define AUDIO_SERVER_APP_HOSTINPUTWINDOW_H


#include "easylogging++.h"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QProgressBar>
#include <QKeyEvent>
#include <QEvent>
//#include <QSizePolicy>
#include <QObject>
#include <Qt>
#include <QtGui>
#include <QPainter>
#include <QTimer>
#include <QLineEdit>
#include <QTcpServer>

class HostInputWindow : public QWidget
{
Q_OBJECT // Must be used in classes that define their own signals and slots
private:
    QLabel* titleLabel;
    QLabel* addressLabel;
    QLabel* portLabel;

    QLineEdit* addressBits1Edit;
    QLineEdit* addressBits2Edit;
    QLineEdit* addressBits3Edit;
    QLineEdit* addressBits4Edit;

    QLineEdit* portEdit;

    QPushButton* hostButton;

    // LAYOUTS
    QVBoxLayout* layout;
    QHBoxLayout* addressBitsLayout;
    QHBoxLayout* portLayout;

private slots:

    void beginHostingButtonPressed();



public:
    HostInputWindow(QWidget *parent = 0);

    signals:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
    void beginHosting(QHostAddress address, quint16 port);
#pragma clang diagnostic pop

};

#endif //AUDIO_SERVER_APP_HOSTINPUTWINDOW_H
