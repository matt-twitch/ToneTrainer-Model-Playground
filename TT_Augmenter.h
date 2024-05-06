/*
  ==============================================================================

    TT_Augmenter.h
    Created: 26 Jan 2024 11:04:26am
    Author:  Matt Twitchen

  ==============================================================================
*/

#pragma once
#include <random>
#include "TT_SpectralInterpolator.h"
#include "TT_TemporalInterpolator.h"
#include "TT_DataNodes.h"
#include "TT_Fetcher.h"
#include "TT_Thresholds.h"

enum Mode
{
    INTERP = 0,
    NOISE
};

class TT_Augmenter
{
public:
    
    TT_Augmenter(TT_Fetcher* ttf);
    ~TT_Augmenter();
    
    void setMode(Mode newMode) { mode = newMode; }
    
    void fetchSpectralData();
    void fetchBrightData();
    void fetchDarkData();
    void fetchResonantData();
    void fetchSoftData();
    
    void fetchTemporalData();
    void fetchPluckData();
    void fetchLongReleaseData();
    void fetchSwellData();
    void fetchShortData();
    
    void augmentSpectralTags();
    void augmentTemporalTags();
    
    void interpolateSpectralTags();
    void interpolateTemporalTags();
    
    void injectNoiseSpectralTags();
    void injectNoiseTemporalTags();
    
    void addAugmentedSpectralTags(TagVector toAdd, juce::Identifier parent, juce::Identifier child);
    void addAugmentedTemporalTags(TagVector toAdd, juce::Identifier parent, juce::Identifier child);

    void sortTagVector(TagVector input);
    void sortMagVector(std::vector<float> input);
    
    std::shared_ptr<juce::ValueTree> const& getDataTree() { return dataTree; }
    
    void errorCheck(TagVector original, TagVector expanded);
    
private:
    
    float calculateMagnitude(PATCH input)
    {
        float magnitudeSqr = 0.f;
        for(int i = 0 ; i < input.size() - 2 ; i++)
        {
            float param = input[i];
            magnitudeSqr += param * param;
        }
        float magnitude = std::sqrt(magnitudeSqr);
        
        return magnitude;
    }

    Mode mode;
    
    std::shared_ptr<juce::ValueTree> dataTree;
    juce::ValueTree spectralTree;
    juce::ValueTree temporalTree;
    
    TT_SpectralInterpolator spectralInterpolator;
    TT_TemporalInterpolator temporalInterpolator;
    TT_Fetcher* fetcher;
    
    // Data vectors - Index of parameters in patch vectors correspond to index of parameter in ParameterNodes arrays
    std::vector<TagVector> spectralTags;
    std::vector<TagVector> temporalTags;
    
    std::vector<std::vector<float>> spectralMags;
    std::vector<std::vector<float>> temporalMags;
    
    int scaleFactor = 3;
};
