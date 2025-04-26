/*
  ==============================================================================

    GlobalConstantsAndMacros.cpp
    Created: 22 Apr 2025 12:07:01pm
    Author:  Pascal

  ==============================================================================
*/

#include "GlobalConstantsAndMacros.h"

std::unordered_map<std::string, VTSSetup> stateMap = {
    {"cutoff", VTSSetup{"Cutoff", {100.0, 20000.0}, 2000.0}},
    {"resonance", VTSSetup{"Resonance", {0.0, 1.0}, 1.0}},
    {"osc1Tuner", VTSSetup{"Osc1Tuner", {-24.0, 24.0}, 0.0}},
    {"osc2Tuner", VTSSetup{"Osc2Tuner", {-24.0, 24.0}, 0.0}},
    {"attack", VTSSetup{"Attack", {0.001, 5.0}, 0.1}},
    {"decay", VTSSetup{"Decay", {0.001, 5.0}, 0.5}},
    {"sustain", VTSSetup{"Sustain", {0.0, 1.0}, 0.5}},
    {"release", VTSSetup{"Release", {0.001, 5.0}, 0.5}},
};

