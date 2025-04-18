/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundMangerAudioProcessor::SoundMangerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, juce::Identifier("ToneManger"),
                           createParameters()), synthAudio(keyState)
#endif
{

    cutoff = parameters.getRawParameterValue("cutoff");
    auto& osc = processorChain.get<oscIndex>();
    osc.initialise([](float x) {return x < 0.0 ? -1.0 : 1.0; });
    osc.setFrequency(440.0);
    auto& gain = processorChain.get<gainIndex>();
    gain.setGainDecibels(10.0);

}

SoundMangerAudioProcessor::~SoundMangerAudioProcessor()
{
}

//==============================================================================
const juce::String SoundMangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoundMangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoundMangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoundMangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoundMangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoundMangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoundMangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoundMangerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SoundMangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoundMangerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

juce::AudioProcessorValueTreeState& SoundMangerAudioProcessor::getParameters()
{
    return parameters;
    // TODO: hier return-Anweisung eingeben
}

//==============================================================================
void SoundMangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getNumInputChannels();
    processorChain.prepare(spec);
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < 4; i++) {
        Voice* currentV = new Voice();
        currentV->prepare(spec);
        synth.addVoice(currentV);
        synth.addSound(new SineWaveSound());
    }
    synthAudio.prepareToPlay(spec);
}

void SoundMangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoundMangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SoundMangerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    juce::Random rng;
    auto& gain = processorChain.get<gainIndex>();
    gain.setGainDecibels(*cutoff*10.0f);
    auto& freq = processorChain.get<oscIndex>();
    freq.setFrequency(*cutoff * 440.0f);
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
        }
        // ..do something to the data...
    }
    synthAudio.getNextAudioBlock(buffer,midiMessages);
    //synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    //juce::dsp::AudioBlock<float> block(buffer);
    //juce::dsp::ProcessContextReplacing context(block);
    //processorChain.process(context);
    
}

//==============================================================================
bool SoundMangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SoundMangerAudioProcessor::createEditor()
{
    return new SoundMangerAudioProcessorEditor (*this);
}

//==============================================================================
void SoundMangerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SoundMangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout SoundMangerAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    auto cutoffParam = std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 0.0f, 1.0, 0.9);
    layout.add(std::move(cutoffParam));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundMangerAudioProcessor();
}
