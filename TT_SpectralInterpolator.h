/*
  ==============================================================================

    TT_SpectralInterpolator.h
    Created: 8 Mar 2024 10:44:55am
    Author:  Matt Twitchen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>
#include <vector>
#include "../../mlinterp/mlinterp/mlinterp.hpp"
#include "../TT_DataNodes.h"

#define PATCH std::vector<float>
typedef std::vector<PATCH> TagVector;

class TT_SpectralInterpolator
{
public:
    
    // top level function called in TT_Augmenter
    TagVector interpolateTag(TagVector tag, std::vector<float> mags, int sf);
    // filters tag vector into parameter vectors
    void filterTagVector(TagVector toSort);
    // called for each parameter, calculates new values
    void interpolateParameter(std::vector<float> param, juce::Identifier paramID);
    // stores inteprolated parameter in correct array
    void storeInterpolatedParameter(float value, juce::Identifier paramID);
    // rebuilds the now expanded tag vector
    void reformatData();
    // resets internal arrays and patch counter
    void reset();
    
private:
    
    TagVector tagVector;
    
    int scaleFactor = 4;
    int patchCount = 0;
    
    std::vector<float> magnitudes;
    std::vector<float> filterFrequency;
    std::vector<float> filterEmphasis;
    std::vector<float> filterContour;
    std::vector<float> oscModMix;
    
    std::vector<float> newFreq;
    std::vector<float> newEmph;
    std::vector<float> newContour;
    std::vector<float> newMix;
    
    TagVector expandedTag;
};
