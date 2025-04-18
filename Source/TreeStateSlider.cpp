/*
  ==============================================================================

    TreeStateSlider.cpp
    Created: 7 Apr 2025 2:08:41pm
    Author:  Pascal

  ==============================================================================
*/

#include "TreeStateSlider.h"


TreeStateSlider::TreeStateSlider(std::string name, juce::AudioProcessorValueTreeState& vts, Slider::SliderStyle style)
{

    this->name = name;
    sliderAttachment.reset(new SliderAttachment(vts, name, slider));
    slider.setSliderStyle(style);
    slider.setTextBoxStyle(Slider::NoTextBox,false,0,0);

}
