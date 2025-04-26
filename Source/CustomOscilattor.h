/*
  ==============================================================================

    CustomOscilattor.h
    Created: 20 Apr 2025 10:53:02am
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
enum OscillatorType {
    Square,
    Sine,
    Saw,
    Triangle
};


class CustomOscillator : juce::dsp::ProcessorBase {
public:
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
    void reset() override;
    void setOscillatorForm(OscillatorType form);
    void setFrequency(float frequency);
    void setGain(float gainValue);
    void setGainDecibels(float gainValue);
private:
    enum
    {
        OscIndex,
        GainIndex,
    };
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> processorChain;

};