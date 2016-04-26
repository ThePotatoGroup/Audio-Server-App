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
    if (postion > this->decoder->numSamples())
    {
        this->decoder->seek(this->decoder->numSamples());
        return 1;
    }
    if (postion < 0)
    {
        this->decoder->seek(0);
        return 2;
    }
    this->decoder->seek(postion);
    return 0; // TODO implement errors
}

float AudioSource::getPercentDone()
{
    return (float)this->decoder->positionInSamples()*100/(float)decoder->numSamples();
}

int AudioSource::getPosition()
{
    return this->decoder->positionInSamples();
}
