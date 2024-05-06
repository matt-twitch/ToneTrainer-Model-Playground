/*
  ==============================================================================

    TT_Formater.h
    Created: 26 Jan 2024 11:04:38am
    Author:  Matt Twitchen

  ==============================================================================
*/

/*
 DATA FORMATTING:
 
 Tag labels are one-hot encoded:
 
    Bright / Pluck -> {1, 0, 0, 0}
    Dark / Long release -> {0, 1, 0, 0}
    Resonant / Swell -> {0, 0, 1, 0}
    Soft / Short -> {0, 0, 0, 1}
 
 Parameter data is encoded with these indexes:
 
    Spectral -
        1. FilterFrequency
        2. FilterEmphasis
        3. FilterContour
        4. OscModMix
 
    Temporal -
        1. EnvType
        2. FilterAttack
        3. FilterDecay
        4. FilterSustain
        5. FilterRelease
        6. VcaAttack
        7. VcaDecay
        8. VcaSustain
        9. VcaRelease
        10. FilterContour
 */

#pragma once
#include "../tiny-dnn-master/tiny_dnn/tiny_dnn.h"
#include "TT_Augmenter.h"

using namespace tiny_dnn;

struct ParameterData
{
    tensor_t data;
    tensor_t labels;
};

class TT_Formatter
{
public:
    
    TT_Formatter(TT_Augmenter* tta); // must be called after TT_Fetcher has parsed the patch library
    ~TT_Formatter();
    
    void formatSpectralData();
    void formatTemporalData();
    
    void scrambleSpectralData();
    void scrambleTemporalData();
    
    ParameterData getSpectralData() { return spectralData; }
    ParameterData getTemporalData() { return temporalData; }
    
private:
    
    std::shared_ptr<juce::ValueTree> dataTree;
    
    juce::ValueTree spectralTree;
    juce::ValueTree temporalTree;
    TT_Augmenter* augmenter;
    
    ParameterData spectralData;
    ParameterData temporalData;
};
