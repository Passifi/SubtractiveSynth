/*
  ==============================================================================

    SynthAudioSource.h
    Created: 17 Apr 2025 2:22:23pm
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Voice.h"
#include <vector>

class SynthAudioSource : public juce::AudioSource {

public:
    SynthAudioSource(juce::MidiKeyboardState& keyState);
    void prepareToPlay(int, double sampleRate) override;
    void prepareToPlay(juce::dsp::ProcessSpec spec);
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void getNextAudioBlock(juce::AudioBuffer<float>& buffer,juce::MidiBuffer midi);
    void setVoiceParameters(VoiceParameters param);
    void setVoiceAttack(float newValue);
    void setVoiceDecay(float newValue);
    void setVoiceSustain(float newValue);
    void setVoiceRelease(float newValue);

private:
    const int MAX_VOICES = 32;
    juce::Synthesiser synth; 
    juce::MidiKeyboardState& keyboardState;
    juce::ADSR adsr;
};
