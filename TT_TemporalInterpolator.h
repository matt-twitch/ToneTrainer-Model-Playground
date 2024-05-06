/*
  ==============================================================================

    TT_TemporalInterpolator.h
    Created: 30 Mar 2024 3:06:41pm
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

class TT_TemporalInterpolator
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
    std::vector<float> filterAttack;
    std::vector<float> filterDecay;
    std::vector<float> filterSustain;
    std::vector<float> filterRelease;
    std::vector<float> vcaAttack;
    std::vector<float> vcaDecay;
    std::vector<float> vcaSustain;
    std::vector<float> vcaRelease;
    std::vector<float> filterContour;
    
    std::vector<float> newFilterAttack;
    std::vector<float> newFilterDecay;
    std::vector<float> newFilterSustain;
    std::vector<float> newFilterRelease;
    std::vector<float> newVcaAttack;
    std::vector<float> newVcaDecay;
    std::vector<float> newVcaSustain;
    std::vector<float> newVcaRelease;
    std::vector<float> newFilterContour;
    
    TagVector expandedTag;
};
