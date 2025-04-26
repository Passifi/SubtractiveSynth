/*
  ==============================================================================

    CustomOscilattor.cpp
    Created: 20 Apr 2025 10:53:02am
    Author:  Pascal

  ==============================================================================
*/

#include "CustomOscilattor.h"

void CustomOscillator::prepare(const juce::dsp::ProcessSpec& spec)
{
    processorChain.prepare(spec);
    auto& osc = processorChain.get<OscIndex>();
    osc.prepare(spec);
    auto& gain = processorChain.get<GainIndex>();
    gain.prepare(spec);
}

void CustomOscillator::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    processorChain.process(context);
}

void CustomOscillator::reset()
{
    processorChain.reset();
}

void CustomOscillator::setOscillatorForm(OscillatorType form)
{
    auto& osc = processorChain.get<OscIndex>();

    switch (form) {
    case Sine:
        osc.initialise(
            [](float x) {
                return std::sin(x);
            }, 128
        );
        break;
    case Square:
        osc.initialise(
            [](float x) {
                return x <= 0.0 ? -1.0 : 1.0;
            }, 128
        );
        break;
    case Saw:
        osc.initialise(
            [](float x) {
                return juce::jmap(x, -juce::MathConstants<float>::pi, juce::MathConstants<float>::pi);
            }, 128
        );
        break;
    case Triangle:
        osc.initialise([](float x) {
            return std::asin(sin(x));
            }, 128);
        break;
    }
}

void CustomOscillator::setFrequency(float frequency)
{
    auto& osc = processorChain.get<OscIndex>();
    osc.setFrequency(frequency);
}

void CustomOscillator::setGain(float gainValue)
{
    auto& gain = processorChain.get<GainIndex>();
    gain.setGainLinear(gainValue);
}

void CustomOscillator::setGainDecibels(float gainValue)
{
    auto& gain = processorChain.get<GainIndex>();
    gain.setGainDecibels(gainValue);
}
