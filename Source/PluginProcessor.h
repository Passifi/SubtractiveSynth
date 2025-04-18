/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MangerSynth.h"
#include "SynthAudioSource.h"

//==============================================================================
/**
*/
class SoundMangerAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SoundMangerAudioProcessor();
    ~SoundMangerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    juce::AudioProcessorValueTreeState& getParameters();
    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
private:
    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float>* cutoff = nullptr;
    juce::Synthesiser synth;
    SynthAudioSource synthAudio;

    enum {
        oscIndex,
        filterIndex,
        gainIndex
    };
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>,juce::dsp::LadderFilter<float>, juce::dsp::Gain<float>> processorChain;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundMangerAudioProcessor)
};
