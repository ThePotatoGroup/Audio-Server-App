//
// Created by Bryce Cater on 4/23/16.
//

#include "HostInputWindow.h"


HostInputWindow::HostInputWindow(QWidget *parent)
{
    // ================ INIT ===================

    this->titleLabel = new QLabel("Hosting Config", this);
    this->addressLabel = new QLabel("Address", this);
    this->portLabel = new QLabel("Port", this);

    this->addressBits1Edit = new QLineEdit();
    this->addressBits2Edit = new QLineEdit();
    this->addressBits3Edit = new QLineEdit();
    this->addressBits4Edit = new QLineEdit();
    this->portEdit = new QLineEdit();

    this->hostButton = new QPushButton("Begin Hosting");

    // =============== CONFIG ===============
    this->setFixedWidth(400);
    this->setFixedHeight(200);


    // ===============  LAYOUT ==============
    this->layout = new QVBoxLayout(this);
    layout->setSpacing(1);

    layout->addWidget(titleLabel, 0 ,Qt::AlignHCenter);
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->addressBitsLayout = new QHBoxLayout(this);
    addressBitsLayout->addWidget(this->addressLabel);
    addressBitsLayout->addWidget(this->addressBits1Edit);
    addressBitsLayout->addWidget(this->addressBits2Edit);
    addressBitsLayout->addWidget(this->addressBits3Edit);
    addressBitsLayout->addWidget(this->addressBits4Edit);
    layout->addLayout(addressBitsLayout);

    this->portLayout = new QHBoxLayout(this);
    portLayout->addWidget(this->portLabel);
    portLayout->addWidget(this->portEdit);
    layout->addLayout(portLayout);

    layout->addWidget(hostButton);

    this->setLayout(layout);
    // =============== BINDING ==============

    connect(this->hostButton, SIGNAL(clicked()), this, SLOT(beginHostingButtonPressed()));
}

void HostInputWindow::beginHostingButtonPressed()
{
    QHostAddress address = QHostAddress(QString(this->addressBits1Edit->text()+"."+this->addressBits2Edit->text()+"."+this->addressBits3Edit->text()+"."+this->addressBits4Edit->text()));
    quint16 port = 5000;
    emit beginHosting(address, port);
    this->close();
}

