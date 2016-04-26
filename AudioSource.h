//
// Created by Bryce Cater on 4/23/16.
//

#ifndef AUDIO_SERVER_APP_AUDIOSOURCE_H
#define AUDIO_SERVER_APP_AUDIOSOURCE_H

#include <audiodecoder/audiodecoder.h>
#include <string>
#include "easylogging++.h"

class AudioSource
{
private:
    AudioDecoder* decoder;

    int playbackPosition;
    bool fileSelected;

public:
    AudioSource();

    int setSoundFile(std::string path);
    int getSamples(int size, const SAMPLE* buffer); // Returns number of samples put in buffer
    int setPostion(int postion);
    float getPercentDone();
    int getPosition();


};

#endif //AUDIO_SERVER_APP_AUDIOSOURCE_H
