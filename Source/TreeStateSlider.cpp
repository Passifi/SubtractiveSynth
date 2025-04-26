/*
  ==============================================================================

    TreeStateSlider.cpp
    Created: 7 Apr 2025 2:08:41pm
    Author:  Pascal

  ==============================================================================
*/

#include "TreeStateSlider.h"


TreeStateSlider::TreeStateSlider(std::string name, juce::AudioProcessorValueTreeState& vts, Slider::SliderStyle style) : label(name+"label",name)
{

    this->name = name;
    sliderAttachment.reset(new SliderAttachment(vts, name, slider));
    slider.setSliderStyle(style);
    slider.setTextBoxStyle(Slider::NoTextBox,false,0,0);
    addAndMakeVisible(label);
    addAndMakeVisible(slider);
}

void TreeStateSlider::paint(juce::Graphics& g)
{
    
}

void TreeStateSlider::resized()
{
    label.setBounds(0, 2, getWidth(), 20); // need some way to calulate the width of the label
    slider.setBounds(2, 22, getWidth(), getHeight() - 20);
}

