//
// Created by Bryce Cater on 4/23/16.
//

#include "AudioSource.h"


AudioSource::AudioSource()
{
    this->decoder = new AudioDecoder("");
    this->playbackPosition = -1;
    this->fileSelected = false;
}

int AudioSource::setSoundFile(std::string path)
{
    *this->decoder = AudioDecoder(path);
    if (this->decoder->open() == AUDIODECODER_OK)
    {
        this->fileSelected = true;
        this->playbackPosition = this->decoder->positionInSamples();

        VLOG(3) << "Audio file loaded:\n"
        << "    Channels: " << this->decoder->channels() << "\n"
        << "    Samples: " << this->decoder->numSamples() << "\n"
        << "    Rate: " << this->decoder->sampleRate() << "\n";
        return 0;
    }
    else
    {
        this->fileSelected = false;
        this->playbackPosition = -1;
        return 1;
    }
}

int AudioSource::getSamples(int size, const SAMPLE *buffer)
{
    if (this->fileSelected)
    {
        return this->decoder->read(size, buffer);
    }

}

int AudioSource::setPostion(int postion)
{

    this->decoder->seek(0);
    return 0; // TODO implement errors
}
