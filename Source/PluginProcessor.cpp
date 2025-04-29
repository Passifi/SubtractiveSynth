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
                           createParameters())
#endif
{

    cutoff = parameters.getRawParameterValue("cutoff");
    voiceParameters.filterCutoff = 2000.0f;
    voiceParameters.filterResonance = 0.3;
    voiceParameters.osc1Tuning = 1.0;
    voiceParameters.osc2Tuning = 1.01;
    
    auto& osc = processorChain.get<oscIndex>();
    osc.initialise([](float x) {return x < 0.0 ? -1.0 : 1.0; });
    osc.setFrequency(440.0);
    auto& gain = processorChain.get<gainIndex>();
    gain.setGainDecibels(10.0);

    parameters.addParameterListener("cutoff", this);
    parameters.addParameterListener("resonance", this);
    parameters.addParameterListener("osc1Tuner", this);
    parameters.addParameterListener("osc2Tuner", this);
    parameters.addParameterListener("attack", this);
    parameters.addParameterListener("decay", this);
    parameters.addParameterListener("sustain", this);
    parameters.addParameterListener("release", this);


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
    synthAudio.setVoiceParameters(voiceParameters);
    synthAudio.setVoiceAttack(*parameters.getRawParameterValue("attack"));
    synthAudio.setVoiceDecay(*parameters.getRawParameterValue("decay"));
    synthAudio.setVoiceSustain(*parameters.getRawParameterValue("sustain"));
    synthAudio.setVoiceRelease(*parameters.getRawParameterValue("release"));
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
        }
        // ..do something to the data...
    }
    synthAudio.getNextAudioBlock(buffer,midiMessages);
    
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
    // check whether VTS Parameters are setup
    // save those ... is there a convenient function for this? 
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SoundMangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(parameters.state.getType())) {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout SoundMangerAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //auto cutoffParam = std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 0.0f, 20000.0, 3000.9);
    //layout.add(std::move(cutoffParam));
    //auto par1 = std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", 0.0f, 1.0, 0.1);
    //layout.add(std::move(par1));
    //auto par2 = std::make_unique<juce::AudioParameterFloat>("osc1Tuner", "Osc1Tuner", 0.0f, 1.0f, 0.5);
    //layout.add(std::move(par2));
    //auto par3 = std::make_unique<juce::AudioParameterFloat>("osc2Tuner", "Osc2Tuner", 0.0f, 1.0f, 0.5);
    //layout.add(std::move(par3));

    for (auto& el : stateMap) {
        auto param = std::make_unique<juce::AudioParameterFloat>(el.first, el.second.name, el.second.range, el.second.startingValue);
        layout.add(std::move(param));
    }
    return layout;
}

void SoundMangerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "cutoff")
    {
        voiceParameters.filterCutoff = newValue;
        synthAudio.setVoiceParameters(voiceParameters);
    }
    else if (parameterID == "resonance") {
        voiceParameters.filterResonance = newValue;
        synthAudio.setVoiceParameters(voiceParameters);
    }
    else if (parameterID == "osc1Tuner") {
        
        voiceParameters.osc1Tuning = std::pow(2.0, newValue/24.0f);
        synthAudio.setVoiceParameters(voiceParameters);
    }
    else if (parameterID == "osc2Tuner") {
        voiceParameters.osc2Tuning = std::pow(2.0, newValue/24.0f);
        synthAudio.setVoiceParameters(voiceParameters);
    }
    else if (parameterID == "attack") { 
        synthAudio.setVoiceAttack(newValue);
    }
    else if (parameterID == "decay") { 
        synthAudio.setVoiceDecay(newValue);
    }
    else if (parameterID == "sustain") { 
        synthAudio.setVoiceSustain(newValue);
    }
    else if (parameterID == "release") { 
        synthAudio.setVoiceRelease(newValue);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundMangerAudioProcessor();
}
