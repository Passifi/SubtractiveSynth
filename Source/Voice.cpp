/*
  ==============================================================================

    Voice.cpp
    Created: 11 Apr 2025 2:50:47pm
    Author:  Pascal

  ==============================================================================
*/

#include "Voice.h"

Voice::Voice()
{
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
    processorChain.prepare(spec);
    auto& osc = processorChain.get<OscIndex>();
    auto& gain = processorChain.get<GainIndex>();
    osc.initialise([](float x) {return x < 0.0 ? -1.0 : 1.0; });
    osc.prepare(spec);
    gain.prepare(spec);
    gain.setGainDecibels(1.0);
    juce::ADSR::Parameters par;
    par.attack = 0.1;
    par.decay = 0.2;
    par.sustain = 1.0;
    par.decay = 0.3;
    adsr.setParameters(par);
    adsr.setSampleRate(spec.sampleRate);
    tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
}


bool Voice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    // seems like a somewhat contrieved way to check whether this sound is the right one
    // I suppose dymanic cast will fail and return a null ptr if sound isn't of type SineWaveSound
}

void Voice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int)
{
    adsr.noteOn();
    setFrequency(midiNoteNumber);

}

void Voice::stopNote(float, bool allowTailOff)
{
    adsr.noteOff();
    clearCurrentNote();
}

void Voice::pitchWheelMoved(int)
{
}

void Voice::controllerMoved(int, int)
{
}


void Voice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

    if (!isVoiceActive()) {
        return;
    }

    juce::Random rng;
        float* channel = outputBuffer.getWritePointer(0);
        auto tempBlock = juce::dsp::AudioBlock<float>(tempBuffer).getSubBlock(0, (size_t)numSamples);
        tempBuffer.clear();
    juce::dsp::AudioBlock<float> audioBlock(outputBuffer);
    juce::dsp::ProcessContextReplacing context(tempBlock);
    processorChain.process(context);
    //adsr.applyEnvelopeToBuffer(tempBuffer,0,numSamples);
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        // Check if the source buffer (tempBuffer) has this channel
        if (channel < tempBuffer.getNumChannels()) {
            outputBuffer.addFrom(
                channel,           // Destination channel index
                startSample,       // Destination start sample index
                tempBuffer,        // Source buffer
                channel,           // Source channel index
                0,                 // Source start sample index (always 0 for temp buffer)
                numSamples         // Number of samples to add
            );
        }
        // Optional: Handle cases where output has more channels than tempBuffer (e.g., copy mono to stereo)
        // else if (channel > 0 && tempBuffer.getNumChannels() == 1) {
        //    outputBuffer.addFrom(channel, startSample, tempBuffer, 0, 0, numSamples); // Copy mono channel 0 to other output channels
        // }
    }

}

void Voice::setFrequency(int midiNote)
{
    auto& osc = processorChain.get<OscIndex>();
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNote));

}

void Voice::setFrequency(float freq)
{
    auto& osc = processorChain.get<OscIndex>();
    osc.setFrequency(freq);
}

void Voice::setGain(float gainValue)
{
    auto& gain = processorChain.get<GainIndex>();
    gain.setGainLinear(gainValue);
}
 