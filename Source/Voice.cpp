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
    // refactor as initilize ProccesorChain
    // Put Processorchain in a dedicated class to completly abstract away the process? 
    // Overhead?
    processorChain.prepare(spec);
    auto& osc = processorChain.get<OscIndex>();
    auto& osc2 = processorChain.get<Osc2Index>();
    auto& gain = processorChain.get<GainIndex>();
    //osc.setOscillatorForm(Square);
    //osc.setGain(0.8);
    osc.prepare(spec);
    osc.initialise([](float x) {return x < 0.0f ? -1.0 : 1.0; }, 128);
    //osc2.setOscillatorForm(Saw);
    
    osc2.initialise([](float x) {return x < 0.0f ? -1.0 : 1.0; }, 128);
    osc2.prepare(spec);
    //osc2.setGain(0.0);
    gain.prepare(spec);
    gain.setGainLinear(0.3);
    auto& filter = processorChain.get<FilterIndex>();
    filter.prepare(spec);
    filter.setCutoffFrequencyHz(parameters.filterCutoff);
    filter.setResonance(parameters.filterResonance);

    juce::ADSR::Parameters par;
    par.attack = 0.1;
    par.decay = 0.1;
    par.sustain = 1.0;
    par.release = 0.3;
    adsr.setSampleRate(spec.sampleRate);
    adsr.setParameters(par);
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
    tempBuffer.clear();
        auto tempBlock = juce::dsp::AudioBlock<float>(tempBuffer).getSubBlock(0, (size_t)numSamples);
        auto outBlock = juce::dsp::AudioBlock<float>(outputBuffer).getSubBlock(startSample,numSamples);
    juce::dsp::ProcessContextReplacing context(tempBlock);
    processorChain.process(context);
    adsr.applyEnvelopeToBuffer(tempBuffer,0,numSamples);
    if (!adsr.isActive()) {
        clearCurrentNote();

    }
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
    }
}

void Voice::setFrequency(int midiNote)
{
    setFrequency((float)juce::MidiMessage::getMidiNoteInHertz(midiNote));
}

void Voice::setFrequency(float freq)
{
    auto& osc = processorChain.get<OscIndex>();
    osc.setFrequency(freq*parameters.osc1Tuning);
    auto& osc2 = processorChain.get<Osc2Index>();
    osc2.setFrequency(freq * parameters.osc2Tuning);
    baseFrequency = freq;
}

void Voice::setGain(float gainValue)
{
    auto& gain = processorChain.get<GainIndex>();
    gain.setGainLinear(gainValue);
}

void Voice::setFilter()
{
    auto& filter = processorChain.get<FilterIndex>();
    filter.setCutoffFrequencyHz(parameters.filterCutoff);
    filter.setResonance(parameters.filterResonance);
}

void Voice::updateFrequencies()
{
    auto& osc1 = processorChain.get<OscIndex>();
    auto& osc2 = processorChain.get<Osc2Index>();
    osc1.setFrequency(baseFrequency * parameters.osc1Tuning);
    osc2.setFrequency(baseFrequency * parameters.osc2Tuning);
}

void Voice::setParameters(VoiceParameters param)
{
    parameters = param;
    setFilter();
    updateFrequencies();
}

void Voice::setAttack(float newValue)
{
    auto par = adsr.getParameters();
    par.attack = newValue;
    adsr.setParameters(par);
}

void Voice::setDecay(float newValue)
{
    auto par = adsr.getParameters();
    par.decay = newValue;
    adsr.setParameters(par);

}

void Voice::setSustain(float newValue) {
    auto par = adsr.getParameters();
    par.sustain = newValue;
    adsr.setParameters(par);
}

void Voice::setRelease(float newValue) {
     auto par = adsr.getParameters();
    par.release = newValue;
    adsr.setParameters(par);
}
 