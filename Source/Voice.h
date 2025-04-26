/*
  ==============================================================================

    Voice.h
    Created: 11 Apr 2025 2:50:47pm
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomOscilattor.h"

enum FilterType {
    Moog,
    Bandpass,
    Highpass
};

struct SineWaveSound final : public juce::SynthesiserSound
{
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

struct VoiceParameters {
    float osc1Tuning;
    float osc2Tuning;
    float filterCutoff;
    float filterResonance;
    float osc1Gain;
    float osc2Gain;
    OscillatorType oscType;
    FilterType filtertype;
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
    void setParameters(VoiceParameters param);
    void setAttack(float newValue);
    void setDecay(float newValue);
    void setSustain(float newValue);
    void setRelease(float newValue);
private:
    float tailOff;
    VoiceParameters parameters{ 1.0,1.02,2000.0f,0.2,Moog };
    void setFrequency(int midiNote);
    void setFrequency(float freq);
    void setGain(float gain);
    void setFilter();
    void updateFrequencies();
    juce::dsp::ProcessorChain<juce::dsp::Gain<float>,juce::dsp::Oscillator<float>,juce::dsp::Oscillator<float>,juce::dsp::LadderFilter<float>> processorChain;
    juce::ADSR adsr;
    double sampleRate = 44100.0;
    float baseFrequency = 440.0f;
    juce::AudioBuffer<float> tempBuffer;
    enum {
        GainIndex,
        OscIndex,
        Osc2Index,
        FilterIndex
    };

};