//
// Created by Bryce Cater on 4/23/16.
//

#include "audioServerApp.h"

#include <sys/socket.h>
#include <qimage.h>
#include <qgraphicsanchorlayout.h>
INITIALIZE_EASYLOGGINGPP;

QFont statusFont = QFont("Ubuntu-light", 50, QFont::Bold, false);
QFont volumeFont = QFont("Ubuntu", 50, QFont::Bold, true);
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

#define SKIP_STEP 3000000;

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

	rewindButton = new QPushButton();
	playPauseButton = new QPushButton();
	fastForwardButton = new QPushButton();

	muteButton = new QPushButton();
	volumeDownButton = new QPushButton();
	volumePercentLabel = new QLabel();
	volumeUpButton = new QPushButton();
	maxVolumeButton = new QPushButton();

	streamConnectionStatusLabel = new QLabel("Stream Status");
    streamConnectionStatusIndicator = new QPushButton();

    controlConnectionStatusLabel = new QLabel("Control Status");
    controlConnectionStatusIndicator = new QPushButton();

	layout = new QVBoxLayout();
	soundFileLayout = new QHBoxLayout();
	playbackControlLayout = new QHBoxLayout();
	volumeControlLayout = new QHBoxLayout();
    connectionStatusLayout = new QHBoxLayout();


    QString playFilename = "play.png";
    this->playImage = new QImage(playFilename, "png");

    QString pauseFilename = "pause.png";
    this->pauseImage = new QImage(pauseFilename, "png");

    QString rewindFilename = "rewind.png";
    this->rewindImage = new QImage(rewindFilename, "png");

    QString fastForwardFilename = "fastForward.png";
    this->fastForwardImage = new QImage(fastForwardFilename, "png");

    QString muteFilename = "mute.png";
    this->muteImage = new QImage(muteFilename, "png");

    QString volumeDownFilename = "volumeDown.png";
    this->volumeDownImage = new QImage(volumeDownFilename, "png");

    QString volumeUpFilename = "volumeUp.png";
    this->volumeUpImage = new QImage(volumeUpFilename, "png");

    QString maxVolumeFilename = "maxVolume.png";
    this->maxVolumeImage = new QImage(maxVolumeFilename, "png");

    // =============== CONFIG ===============

    this->titleLabel->setFont(statusFont);

    this->soundFileTextEdit->setFixedWidth(250);

    this->volumePercentLabel->setFont(volumeFont);
    this->setVolume(50);

    this->playPauseButton->setIcon(QIcon("play.png"));
    this->playPauseButton->setFixedSize(80,80);
    this->playPauseButton->setIconSize(QSize(60,60));

    this->rewindButton->setIcon(QIcon("rewind.png"));
    this->rewindButton->setFixedSize(80,80);
    this->rewindButton->setIconSize(QSize(60,60));

    this->fastForwardButton->setIcon(QIcon("fastForward.png"));
    this->fastForwardButton->setFixedSize(80,80);
    this->fastForwardButton->setIconSize(QSize(60,60));

    this->muteButton->setIcon(QIcon("mute.png"));
    this->muteButton->setFixedSize(80,80);
    this->muteButton->setIconSize(QSize(60,60));

    this->volumeDownButton->setIcon(QIcon("volumeDown.png"));
    this->volumeDownButton->setFixedSize(80,80);
    this->volumeDownButton->setIconSize(QSize(60,60));

    this->volumeUpButton->setIcon(QIcon("volumeUp.png"));
    this->volumeUpButton->setFixedSize(80,80);
    this->volumeUpButton->setIconSize(QSize(60,60));

    this->maxVolumeButton->setIcon(QIcon("maxVolume.png"));
    this->maxVolumeButton->setFixedSize(80,80);
    this->maxVolumeButton->setIconSize(QSize(60,60));

    this->streamConnectionStatusIndicator->setIcon(QIcon("statusBad.png"));
    this->streamConnectionStatusIndicator->setFixedSize(80,80);
    this->streamConnectionStatusIndicator->setIconSize(QSize(60,60));

    this->controlConnectionStatusIndicator->setIcon(QIcon("statusBad.png"));
    this->controlConnectionStatusIndicator->setFixedSize(80,80);
    this->controlConnectionStatusIndicator->setIconSize(QSize(60,60));

    this->soundProgressBar->setValue(0);
    this->soundProgressBar->setFixedWidth(350);
    this->soundProgressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // ===============  LAYOUT ==============

    this->setLayout(this->layout);

    this->layout->addWidget(this->titleLabel, 0, Qt::AlignCenter);

    this->layout->addLayout(this->soundFileLayout);
    this->soundFileLayout->addWidget(this->soundFileLabel, 0, Qt::AlignCenter);
    this->soundFileLayout->addWidget(this->soundFileTextEdit, 1, Qt::AlignCenter);
    this->soundFileLayout->addWidget(this->soundFileButton, 0, Qt::AlignCenter);

    this->layout->addWidget(this->soundProgressBar, 0 , Qt::AlignCenter);

    this->layout->addLayout(this->playbackControlLayout);
    this->playbackControlLayout->addWidget(this->rewindButton, 0, Qt::AlignCenter);
    this->playbackControlLayout->addWidget(this->playPauseButton, 0, Qt::AlignCenter);
    this->playbackControlLayout->addWidget(this->fastForwardButton, 0, Qt::AlignCenter);

    this->layout->addLayout(this->volumeControlLayout);
    this->volumeControlLayout->addWidget(this->muteButton, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->volumeDownButton, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->volumePercentLabel, 1, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->volumeUpButton, 0, Qt::AlignCenter);
    this->volumeControlLayout->addWidget(this->maxVolumeButton, 0, Qt::AlignCenter);

    this->layout->addLayout(this->connectionStatusLayout);
    this->connectionStatusLayout->addWidget(this->streamConnectionStatusLabel, 0, Qt::AlignCenter);
    this->connectionStatusLayout->addWidget(this->streamConnectionStatusIndicator, 0, Qt::AlignCenter);
    this->connectionStatusLayout->addWidget(this->controlConnectionStatusLabel, 0, Qt::AlignCenter);
    this->connectionStatusLayout->addWidget(this->controlConnectionStatusIndicator, 0, Qt::AlignCenter);

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
    connect(this->networkInterface, SIGNAL(connectionStatusChanged()), this, SLOT(updateConnectionInidcators()));
    connect(this->networkInterface, SIGNAL(sentStreamData()), this, SLOT(sendStreamData()));
    connect(this->networkInterface, SIGNAL(sentStreamData()), this, SLOT(sendControlData()));
#pragma clang diagnostic pop
    this->updateConnectionInidcators();

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
    control_command_t command;
    command.type = FLUSH_BUFFER;
    command.value = 0;
    this->networkInterface->sendCommand(command);

    int newPos = this->audioSource->getPosition()-SKIP_STEP;
    this->audioSource->setPostion(newPos);

    VLOG(2) << "Rewind pressed.";
}

void audioAppMainView::togglePlayPause()
{
    control_command_t command;
    command.type = PLAY;
    command.value = 0;
    this->networkInterface->sendCommand(command);

    VLOG(2) << "Toggle play pause pressed.";
    this->sendStreamData();
}

void audioAppMainView::fastForward()
{
    control_command_t command;
    command.type = FLUSH_BUFFER;
    command.value = 0;
    this->networkInterface->sendCommand(command);

    int newPos = this->audioSource->getPosition()+SKIP_STEP;
    this->audioSource->setPostion(newPos);

    VLOG(2) << "Fast forward pressed.";
}

void audioAppMainView::muteVolume()
{
    this->setVolume(0);
    control_command_t command;
    command.type = MUTE;
    command.value = 0;
    this->networkInterface->sendCommand(command);
    VLOG(2) << "Mute pressed.";
}

void audioAppMainView::decreaseVolume()
{
    if (this->currentvVolumePercent > 5)
    {
        this->setVolume(this->currentvVolumePercent-5);
    }
    else
    {
        this->setVolume(0);
    }

    VLOG(2) << "Decrease volume pressed.";
}

void audioAppMainView::increaseVolume()
{
    this->setVolume(this->currentvVolumePercent+5);
    VLOG(2) << "Increase volume pressed.";
}

void audioAppMainView::maxVolume()
{
    this->setVolume(100);
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

    int samplesCount = this->audioSource->getSamples(BUFFER_SIZE, this->samplesBuffer);
    this->networkInterface->sendStreamSamples(samplesCount, this->samplesBuffer);

    this->soundProgressBar->setValue(this->audioSource->getPercentDone());
}

void audioAppMainView::sendControlData()
{

}


void audioAppMainView::setVolume(unsigned int volumePercent)
{
    if (this->currentvVolumePercent != volumePercent)
    {
        if (volumePercent > 100)
        {
            this->currentvVolumePercent = 100;
        }
        else if (volumePercent < 0)
        {
            this->currentvVolumePercent = 0;
        }
        else
        {
            this->currentvVolumePercent = volumePercent;
        }

        this->volumePercentLabel->setText(QString::number(this->currentvVolumePercent)+"%");

        if (this->networkInterface->hasControlConnection)
        {
            control_command_t command;
            command.type = SET_VOLUME;
            command.value = this->currentvVolumePercent;
            this->networkInterface->sendCommand(command);
        }

    }
}


void audioAppMainView::updateConnectionInidcators()
{
    if (this->networkInterface->hasStreamConnection)
    {
        this->streamConnectionStatusIndicator->setIcon(QIcon("statusGood.png"));
    }
    else
    {
        this->streamConnectionStatusIndicator->setIcon(QIcon("statusBad.png"));
    }
    if (this->networkInterface->hasControlConnection)
    {
        this->controlConnectionStatusIndicator->setIcon(QIcon("statusGood.png"));
    }
    else
    {
        this->controlConnectionStatusIndicator->setIcon(QIcon("statusBad.png"));
    }
    if (this->networkInterface->hasControlConnection && this->networkInterface->hasStreamConnection)
    {
        this->rewindButton->setEnabled(true);
        this->playPauseButton->setEnabled(true);
        this->fastForwardButton->setEnabled(true);
        this->muteButton->setEnabled(true);
        this->volumeDownButton->setEnabled(true);
        this->volumeUpButton->setEnabled(true);
        this->maxVolumeButton->setEnabled(true);


        this->readyToPlay = true;
    }
    else
    {
        this->rewindButton->setEnabled(false);
        this->playPauseButton->setEnabled(false);
        this->fastForwardButton->setEnabled(false);
        this->muteButton->setEnabled(false);
        this->volumeDownButton->setEnabled(false);
        this->volumeUpButton->setEnabled(false);
        this->maxVolumeButton->setEnabled(false);
        
        this->readyToPlay = false;
    }

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

