#include "Sound.h"
#include "log.h"
#include "fft.h"

Sound::Sound()
{
}

Sound::~Sound()
{
    free(m_pcm_buf);
    snd_pcm_drain(m_snd_pcm);
    snd_pcm_close(m_snd_pcm);
}

void Sound::Init()
{
    snd_pcm_open(&m_snd_pcm, "default", SND_PCM_STREAM_CAPTURE, 0);

    snd_pcm_hw_params_t *param;
    snd_pcm_hw_params_malloc(&param);
    snd_pcm_hw_params_any(m_snd_pcm, param);

    snd_pcm_hw_params_set_access(m_snd_pcm, param, SND_PCM_ACCESS_RW_INTERLEAVED);
    
    snd_pcm_hw_params_set_format(m_snd_pcm, param, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(m_snd_pcm, param, AUDIO_CHANNEL);
    
    uint32_t sample_rate = SIMPLE_RATE; int dir;
    snd_pcm_hw_params_set_rate_near(m_snd_pcm, param, &sample_rate, &dir);

    unsigned int buffer_time;
    snd_pcm_hw_params_get_buffer_time_max(param, &buffer_time, 0);
    LOG(INFO, "max buffer_time:%d", buffer_time);
    if (buffer_time > 500000) 
        buffer_time = 500000;
    snd_pcm_hw_params_set_buffer_time_near(m_snd_pcm, param, &buffer_time, 0);

    unsigned int period_time = (float)SIMPLE_LEN / SIMPLE_RATE * 1000000 + 5000;
    snd_pcm_hw_params_set_period_time_near(m_snd_pcm, param, &period_time, 0);

    // 3. 将设置初始化到pcm结构
    snd_pcm_hw_params(m_snd_pcm, param);


    // 4. 初始化一个帧缓冲区
    snd_pcm_hw_params_get_period_size(param, &m_frames, &dir);

    m_pcm_buf = (int16_t*) malloc(m_frames * 1 * AUDIO_CHANNEL);
    LOG(INFO, "frames %d", m_frames);
}


void Sound::setOnSpectrumUpdate(std::function<void (std::vector<double> &&)> callback)
{
    OnSpectrumUpdate = std::move(callback);
}

void Sound::run()
{
    static complex<double> in[SIMPLE_LEN];
    static complex<double> out[SIMPLE_LEN];

    while(!this->isStoped()) {
        int ret = snd_pcm_readi(m_snd_pcm, m_pcm_buf, m_frames);
        if (ret == -EPIPE) {
            printf("overrun occurred\n");
            ret = snd_pcm_prepare(m_snd_pcm);
            if (ret < 0) {
                printf("failed to recover form overrun\n");
            }
        }
        else if (ret < 0) {
            printf("error from read:%s\n", snd_strerror(ret));
        }
        else if (ret != (int)m_frames) {
            printf("short read, read %d samples\n", ret);
        }

        for(int i = 0; i < SIMPLE_LEN; ++i) {
            in[i] = complex<double>(m_pcm_buf[i], 0.0f);
        }

        memset(m_pcm_buf, 0, SIMPLE_LEN);


        bool re = TFFT<double>::Forward(in, out, SIMPLE_LEN);
        
        for(int i = 0; i < SIMPLE_LEN / 2; ++i) {
            m_spectrum[i] = sqrt(out[i].norm()) / SIMPLE_LEN; 
           // m_spectrum[i] = std::log10(out[i].norm());
        }
        
        if (re)
            OnSpectrumUpdate(std::vector<double>(m_spectrum, m_spectrum + SIMPLE_LEN));
        memset(m_spectrum, 0, SIMPLE_LEN / 2);
    }
}
