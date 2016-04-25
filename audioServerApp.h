//
// Created by Bryce Cater on 4/23/16.
//

#ifndef AUDIO_SERVER_APP_AUDOSERVERAPP_H
#define AUDIO_SERVER_APP_AUDOSERVERAPP_H

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
#include <QFileDialog>
#include <QTimer>
#include <QAudioFormat>
#include <QAudioDecoder>
#include "NetworkInterface.h"
#include "HostInputWindow.h"
#include <audiodecoder/audiodecoder.h>


const int BUFFER_SIZE = 100;

const int TCP_BASE_PORT = 5000;


class audioAppMainView : public QWidget
{
    Q_OBJECT // Must be used in classes that define their own signals and slots
private:
    // VARS
    bool readyToPlay;

    SAMPLE* samplesBuffer;

    // NETWORK
    NetworkInterface *networkInterface;
    AudioSource *audioSource;

    // INTERFACE
    QLabel *titleLabel;

    QLabel *soundFileLabel;
    QLineEdit *soundFileTextEdit;
    QPushButton *soundFileButton;
    QFileDialog *soundFileBrowseDiaglog;

    QProgressBar *soundProgressBar;

    QPushButton *rewindButton;
    QPushButton *playPauseButton;
    QPushButton *fastForwardButton;

    QPushButton *muteButton;
    QPushButton *volumeDownButton;
    QLabel *volumePercentLabel;
    QPushButton *volumeUpButton;
    QPushButton *maxVolumeButton;

    QLabel *connectionStatusLabel;
    QPushButton *connectionStatusIndicator;

    // LAYOUT
    QVBoxLayout *layout;
    QHBoxLayout *soundFileLayout;
    QHBoxLayout *playbackControlLayout;
    QHBoxLayout *volumeControlLayout;
    QHBoxLayout *connectionStatusLayout;

    HostInputWindow* hostInputWindow;

private slots:

    void loadSoundFile();


    // BUTTON SLOTS
    void browseFiles();
    void rewind();
    void togglePlayPause();
    void fastForward();
    void muteVolume();
    void decreaseVolume();
    void increaseVolume();
    void maxVolume();


    // NETWORK SLOTS
    void sendControlData();
    void sendStreamData();

public:
    void initializeNetworkInterface(QHostAddress address, quint16 port);
    audioAppMainView(QWidget *parent = 0);

};

#endif //AUDIO_SERVER_APP_AUDOSERVERAPP_H
