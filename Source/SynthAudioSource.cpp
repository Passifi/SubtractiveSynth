/*
  ==============================================================================

    SynthAudioSource.cpp
    Created: 17 Apr 2025 2:22:23pm
    Author:  Pascal

  ==============================================================================
*/

#include "SynthAudioSource.h"
#include "Voice.h"

SynthAudioSource::SynthAudioSource() 
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
    //juce::MidiBuffer incomingMidi;
    //keyboardState.processNextMidiBuffer(incomingMidi, 0, buffer.getNumSamples(), true);

    synth.renderNextBlock(buffer, midi, 0, buffer.getNumSamples());

}
