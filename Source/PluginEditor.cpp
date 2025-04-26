/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundMangerAudioProcessorEditor::SoundMangerAudioProcessorEditor(SoundMangerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), cutOff("cutoff", p.getParameters(),juce::Slider::Rotary), resonance("resonance",p.getParameters(),juce::Slider::Rotary),
    osc1Tuner("osc1Tuner", p.getParameters(), juce::Slider::Rotary),osc2Tuner("osc2Tuner", p.getParameters(), juce::Slider::Rotary)
{

    adsrSlider.push_back(std::make_unique<TreeStateSlider>("attack", p.getParameters(), juce::Slider::LinearBarVertical));
    adsrSlider.push_back(std::make_unique<TreeStateSlider>("decay", p.getParameters(), juce::Slider::LinearBarVertical));
    adsrSlider.push_back(std::make_unique<TreeStateSlider>("sustain", p.getParameters(), juce::Slider::LinearBarVertical));
    adsrSlider.push_back(std::make_unique<TreeStateSlider>("release", p.getParameters(), juce::Slider::LinearBarVertical));
    
    for (auto& s : adsrSlider) {
        addAndMakeVisible(*s);
    }

    addAndMakeVisible(cutOff);
    addAndMakeVisible(resonance);
    addAndMakeVisible(osc1Tuner);
    addAndMakeVisible(osc2Tuner);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 800);
}

SoundMangerAudioProcessorEditor::~SoundMangerAudioProcessorEditor()
{
}

//==============================================================================
void SoundMangerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    cutOff.setBounds(0, 0, 100, 100);
    resonance.setBounds(0, 100, 100, 100);
    osc1Tuner.setBounds(110, 0, 100, 100);
    osc2Tuner.setBounds(110, 100, 100, 100);
    int x = 300;
    int y = 0;
    for (auto& el : adsrSlider) {
        el->setBounds(x, y, 20 , 200);
        x += 25;
        
    }

}

void SoundMangerAudioProcessorEditor::resized()
{

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
