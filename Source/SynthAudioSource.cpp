/*
  ==============================================================================

    SynthAudioSource.cpp
    Created: 17 Apr 2025 2:22:23pm
    Author:  Pascal

  ==============================================================================
*/

#include "SynthAudioSource.h"

SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState) : keyboardState(keyState)
{

}


void SynthAudioSource::prepareToPlay(int, double sampleRate)
{


}

void SynthAudioSource::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    synth.setCurrentPlaybackSampleRate(spec.sampleRate);
    for (int i = 0; i < MAX_VOICES; i++) {
        Voice* currentV = new Voice();
        currentV->prepare(spec);
        synth.addVoice(currentV);
        synth.addSound(new SineWaveSound());
    }
}

void SynthAudioSource::releaseResources()
{
}

void SynthAudioSource::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
}

void SynthAudioSource::getNextAudioBlock(juce::AudioBuffer<float>& buffer,juce::MidiBuffer midi)
{
    synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());
}

void SynthAudioSource::setVoiceParameters(VoiceParameters param)
{
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto v = dynamic_cast<Voice*>(synth.getVoice(i))) {
            v->setParameters(param);
        }
    }
}

void SynthAudioSource::setVoiceAttack(float newValue)
{
    for (size_t i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto v = dynamic_cast<Voice*>(synth.getVoice(i)))
        {
            v->setAttack(newValue);
        }
    }
}

void SynthAudioSource::setVoiceDecay(float newValue)
{
    for (size_t i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto v = dynamic_cast<Voice*>(synth.getVoice(i)))
        {
            v->setDecay(newValue);
        }
    }
}

void SynthAudioSource::setVoiceSustain(float newValue)
{
    for (size_t i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto v = dynamic_cast<Voice*>(synth.getVoice(i)))
        {
            v->setSustain(newValue);
        }

    }
}

void SynthAudioSource::setVoiceRelease(float newValue)
{
    for (size_t i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto v = dynamic_cast<Voice*>(synth.getVoice(i)))
        {
            v->setRelease(newValue);
        }
    }
}
