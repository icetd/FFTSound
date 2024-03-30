#ifndef SOUND_H
#define SOUND_H

#include <alsa/asoundlib.h>
#include "MThread.h"
#include <vector>
#include <functional>

#define SIMPLE_RATE     44100
#define AUDIO_CHANNEL   1
const int SIMPLE_LEN =  1024;

class Sound : public MThread
{
public:
    Sound();
    ~Sound();

    void Init();
    void setOnSpectrumUpdate(std::function<void (std::vector<double>&&)> callback);

    virtual void run() override;
 
    // 定义窗函数类型
    enum WindowType {
        Hanning,
        Hamming,
        Blackman
    };

private:
    snd_pcm_t *m_snd_pcm;
    snd_pcm_uframes_t m_frames;   
    int16_t *m_pcm_buf;   //SND_PCM_FORMAT_S16_LE  
    double m_spectrum[SIMPLE_LEN / 2];

    //void ApplyWindow(complex<double>* signal, int length, WindowType windowType);
    std::function<void (std::vector<double> &&)> OnSpectrumUpdate;
};

#endif