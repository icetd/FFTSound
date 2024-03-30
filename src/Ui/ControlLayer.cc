#include <vector>
#include "ControlLayer.h"

void ControlLayer::OnAttach()
{
    m_sound = new Sound();
    m_sound->Init();
    m_sound->setOnSpectrumUpdate(std::bind(&ControlLayer::OnSpectrumUpdateCallback, this, std::placeholders::_1));
    for (int i = 0; i < SIMPLE_LEN / 2; i++) {
        x_values[i] = (float)(44100.0 / SIMPLE_LEN) * i;
    }
    m_isFirst = true;
}

void ControlLayer::OnDetach()
{
    m_sound->stop();
}

void ControlLayer::OnUpdate(float ts)
{       
		ImGuiContext& g = *GImGui;
		if (!(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize))
			ImGui::SetNextWindowSize(ImVec2(0.0f, ImGui::GetFontSize() * 12.0f), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(u8"Control") || ImGui::GetCurrentWindow()->BeginCount > 1) {
			ImGui::End();
			return;
		}

        if (m_isFirst)
            m_sound->start();
        m_isFirst = false;

        if (!m_data_list.empty()) {
            m_mux.lock();
            m_data = m_data_list.back();
            m_data_list.pop_back();
                // 开始ImPlot绘图
            if (ImPlot::BeginPlot("Spectrum", "Frequency", "Amplitude", ImVec2(1200, 600))) {
                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_LockMax | ImPlotAxisFlags_LockMin, ImPlotAxisFlags_LockMin);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, SIMPLE_RATE / 2, ImGuiCond_Always);
                ImPlot::PlotLine("Spectrum", x_values, m_data.data(), SIMPLE_LEN / 2);
                ImPlot::EndPlot();
            }
            m_mux.unlock();
       } else if (!m_data.empty()){
            if (ImPlot::BeginPlot("Spectrum", "Frequency", "Amplitude", ImVec2(1200, 600))) {
                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_LockMax | ImPlotAxisFlags_LockMin, ImPlotAxisFlags_LockMin);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, SIMPLE_RATE / 2, ImGuiCond_Always);
                ImPlot::PlotLine("Spectrum", x_values, m_data.data(), SIMPLE_LEN / 2);
                ImPlot::EndPlot();
            }
       }

        ImGui::End();
}

void ControlLayer::OnSpectrumUpdateCallback(std::vector<double> &&data)
{
    m_mux.lock();
    m_data_list.emplace_back(std::move(data));
    if (m_data_list.size() > 3) 
        m_data_list.pop_back();
    m_mux.unlock();
}