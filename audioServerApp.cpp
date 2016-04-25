//
// Created by Bryce Cater on 4/23/16.
//

#include "audioServerApp.h"

#include <sys/socket.h>
#include <qimage.h>
#include <qgraphicsanchorlayout.h>
INITIALIZE_EASYLOGGINGPP;

QFont statusFont = QFont("Lucida Grande", 50, QFont::Bold, false);
QString buttonStyle = QString::fromUtf8("QPushButton{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                "stop: 0 #FFFFFF, stop: 1 #ECECEC);"
                                                "border-style: solid;"
                                                "border-color: #EFA756;"
                                                "border-width: 2px;"
                                                "border-radius: 10px;}");

QString controlButtonStyle = QString::fromUtf8("QPushButton{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                       "stop: 0 #FFFFFF, stop: 1 #ECECEC);"
                                                       "border-style: solid;"
                                                       "border-color: #EFA756;"
                                                       "border-width: 2px;"
                                                       "border-radius: 10px;}");

audioAppMainView::audioAppMainView(QWidget *parent) : QWidget(parent)
{

    // ================ INIT ===================
//    this->hostInputWindow = new HostInputWindow();
//    connect(this->hostInputWindow, &HostInputWindow::beginHosting, this, &audioAppMainView::initializeNetworkInterface);
//    this->hostInputWindow->setWindowModality(Qt::WindowModality::ApplicationModal);
//    this->hostInputWindow->show();
    this->initializeNetworkInterface(QHostAddress::Any, TCP_BASE_PORT);
    this->samplesBuffer = new SAMPLE[BUFFER_SIZE];

    this->audioSource = new AudioSource();
    readyToPlay = false;

    // INTERFACE
	titleLabel = new QLabel("Audio Server App");

	soundFileLabel = new QLabel("Sound File");
	soundFileTextEdit = new QLineEdit();
	soundFileButton = new QPushButton("Browse");
	soundFileBrowseDiaglog = new QFileDialog();

	soundProgressBar = new QProgressBar();

	rewindButton = new QPushButton("<-");
	playPauseButton = new QPushButton(">");
	fastForwardButton = new QPushButton("->");

	muteButton = new QPushButton("mute");
	volumeDownButton = new QPushButton("-");
	volumePercentLabel = new QLabel("50%");
	volumeUpButton = new QPushButton("+");
	maxVolumeButton = new QPushButton("max");

	connectionStatusLabel = new QLabel("Status");
    connectionStatusIndicator = new QPushButton("discon");

	layout = new QVBoxLayout();
	soundFileLayout = new QHBoxLayout();
	playbackControlLayout = new QHBoxLayout();
	volumeControlLayout = new QHBoxLayout();
    connectionStatusLayout = new QHBoxLayout();

    // =============== CONFIG ===============

    this->soundProgressBar->setValue(0);

    // ===============  LAYOUT ==============

    this->setLayout(this->layout);

    this->layout->addWidget(this->titleLabel, 0, Qt::AlignCenter);

    this->layout->addLayout(this->soundFileLayout);
    this->soundFileLayout->addWidget(this->soundFileLabel, 0, Qt::AlignCenter);
    this->soundFileLayout->addWidget(this->soundFileTextEdit, 0, Qt::AlignCenter);
    this->soundFileLayout->addWidget(this->soundFileButton, 0, Qt::AlignCenter);

    this->layout->addWidget(this->soundProgressBar, 0 , Qt::AlignCenter);

    this->layout->addLayout(this->playbackControlLayout);
    this->playbackControlLayout->addWidget(this->rewindButton, 0, Qt::AlignCenter);
    this->playbackControlLayout->addWidget(this->playPauseButton, 0, Qt::AlignCenter);
    this->playbackControlLayout->addWidget(this->fastForwardButton, 0, Qt::AlignCenter);

    this->layout->addLayout(this->volumeControlLayout);
    this->volumeControlLayout->addWidget(this->muteButton, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->volumeDownButton, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->volumePercentLabel, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->volumeUpButton, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->maxVolumeButton, 0, Qt::AlignCenter);

    this->layout->addLayout(this->connectionStatusLayout);
    this->connectionStatusLayout->addWidget(this->connectionStatusLabel, 0, Qt::AlignCenter);
    this->connectionStatusLayout->addWidget(this->connectionStatusIndicator, 0, Qt::AlignCenter);

    // =============== BINDING ==============
    connect(this->soundFileButton, SIGNAL(clicked()), this, SLOT(browseFiles()));
    connect(this->rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));
    connect(this->playPauseButton, SIGNAL(clicked()), this, SLOT(togglePlayPause()));
    connect(this->fastForwardButton, SIGNAL(clicked()), this, SLOT(fastForward()));
    connect(this->muteButton, SIGNAL(clicked()), this, SLOT(muteVolume()));
    connect(this->volumeDownButton, SIGNAL(clicked()), this, SLOT(decreaseVolume()));
    connect(this->volumeUpButton, SIGNAL(clicked()), this, SLOT(increaseVolume()));
    connect(this->maxVolumeButton, SIGNAL(clicked()), this, SLOT(maxVolume()));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types"
    connect(this->networkInterface, SIGNAL(sentStreamData()), this, SLOT(sendStreamData()));
    connect(this->networkInterface, SIGNAL(sentStreamData()), this, SLOT(sendControlData()));
#pragma clang diagnostic pop


}

void audioAppMainView::browseFiles()
{
    VLOG(2) << "Browse files pressed.";

    QString newSoundFilePath = this->soundFileBrowseDiaglog->getOpenFileName();
    if (!this->audioSource->setSoundFile(newSoundFilePath.toUtf8().constData()))
    {
        readyToPlay = true;

        this->soundFileTextEdit->setText(newSoundFilePath);
    }
    else
    {
        this->soundFileTextEdit->setText("[FILE NOT VALID]");
        readyToPlay = false;
    }
}

void audioAppMainView::loadSoundFile()
{

}

void audioAppMainView::rewind()
{
    VLOG(2) << "Rewind pressed.";
}

void audioAppMainView::togglePlayPause()
{
    VLOG(2) << "Toggle play pause pressed.";
    this->sendStreamData();
}

void audioAppMainView::fastForward()
{
    VLOG(2) << "Fast forward pressed.";
}

void audioAppMainView::muteVolume()
{
    VLOG(2) << "Mute pressed.";
}

void audioAppMainView::decreaseVolume()
{
    VLOG(2) << "Decrease volume pressed.";
}

void audioAppMainView::increaseVolume()
{
    VLOG(2) << "Increase volume pressed.";
}

void audioAppMainView::maxVolume()
{
    VLOG(2) << "Max volume pressed.";
}


void audioAppMainView::initializeNetworkInterface(QHostAddress address, quint16 port)
{
    LOG(DEBUG) << "Network interface init started.";
    this->networkInterface = new NetworkInterface(address, port);
    LOG(DEBUG) << "Network interface started.";
}


void audioAppMainView::sendStreamData()
{
    VLOG(4) << "Sending stream data.";
//    int samplesCount = this->audioSource->getSamples(BUFFER_SIZE, this->samplesBuffer);
//    this->networkInterface->sendStreamSamples(samplesCount, this->samplesBuffer);

    int samplesCount = this->audioSource->getSamples(BUFFER_SIZE, this->samplesBuffer);
    this->networkInterface->sendStreamSamples(samplesCount, this->samplesBuffer);
}

void audioAppMainView::sendControlData()
{

}



int main(int argc, char *argv[])
{
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations conf("easylogging++.conf");
    el::Loggers::reconfigureAllLoggers(conf);

    QApplication app(argc, argv);

    audioAppMainView window;

    window.resize(400, 200);
    window.setWindowTitle("Audio Server App");
    window.show();

    return app.exec();
}