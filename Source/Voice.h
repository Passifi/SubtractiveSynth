/*
  ==============================================================================

    Voice.h
    Created: 11 Apr 2025 2:50:47pm
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct SineWaveSound final : public juce::SynthesiserSound
{
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

class Voice : public juce::SynthesiserVoice
{
public:
    Voice();
    void prepare(const juce::dsp::ProcessSpec& spec);
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override;
    void stopNote(float, bool allowTailOff) override;
    void pitchWheelMoved(int) override;
    void controllerMoved(int,int) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
private:
    float tailOff;
    void setFrequency(int midiNote);
    void setFrequency(float freq);
    void setGain(float gain);
    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::Oscillator<float>> processorChain;
    juce::ADSR adsr;
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> tempBuffer;
    enum {
        GainIndex,
        OscIndex
    };

};