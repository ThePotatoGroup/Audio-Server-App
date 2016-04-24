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


class audioAppMainView : public QWidget
{
    Q_OBJECT // Must be used in classes that define their own signals and slots
private:

    AudioDecoder *decoder;

    // VARS
    bool playing = false;
    QString soundFilePath;
    int playbackPosition = 0;
    bool fileSelected = false;


    // NETWORK
    NetworkInterface *networkInterface;

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

    void browseFiles();
    void loadSoundFile();
    void rewind();
    void togglePlayPause();
    void fastForward();
    void muteVolume();
    void decreaseVolume();
    void increaseVolume();
    void maxVolume();

    void sendAudioFileBuffer();

public:
    void initializeNetworkInterface(QHostAddress address, quint16 port);
    audioAppMainView(QWidget *parent = 0);

};

#endif //AUDIO_SERVER_APP_AUDOSERVERAPP_H
