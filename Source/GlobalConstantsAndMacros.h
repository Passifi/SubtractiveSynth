/*
  ==============================================================================

    GlobalConstantsAndMacros.h
    Created: 22 Apr 2025 11:19:54am
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

struct VTSSetup {
    std::string name;
    juce::Range<float> range;
    float startingValue;
};


extern std::unordered_map<std::string, VTSSetup> stateMap;
 