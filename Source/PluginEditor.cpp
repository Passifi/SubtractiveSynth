/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundMangerAudioProcessorEditor::SoundMangerAudioProcessorEditor(SoundMangerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), cutOff("cutoff", p.getParameters(),juce::Slider::Rotary)
{

    addAndMakeVisible(cutOff.slider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
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
    cutOff.slider.setBounds(0, 0, 300, 50);

}

void SoundMangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
