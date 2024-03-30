#ifndef CTRL_LAYER_H
#define CTRL_LAYER_H

#include "Layer.h"
#include "Sound.h"
#include <mutex>

class ControlLayer : public Layer
{
protected:
    virtual void OnAttach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnDetach() override;

private:
    Sound *m_sound;

    std::mutex m_mux;
    std::vector<double> m_data;
    std::vector<std::vector<double>> m_data_list;
    void OnSpectrumUpdateCallback(std::vector<double> &&data);

    bool m_isFirst;
    double x_values[SIMPLE_LEN / 2];
};

#endif