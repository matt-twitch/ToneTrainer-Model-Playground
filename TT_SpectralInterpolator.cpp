/*
  ==============================================================================

    TT_SpectralInterpolator.cpp
    Created: 14 Mar 2024 7:12:44pm
    Author:  Matt Twitchen

  ==============================================================================
*/

#include "TT_SpectralInterpolator.h"

#define spectralParams DataNodes::ParameterNodes::spectralParams
#define temporalParams DataNodes::ParameterNodes::temporalParams

using namespace mlinterp;

TagVector TT_SpectralInterpolator::interpolateTag(TagVector tag, std::vector<float> mags, int sf)
{
    reset();
    
    scaleFactor = sf;
    
    tagVector = tag;
    magnitudes = mags;
    filterTagVector(tag);
    
    interpolateParameter(filterFrequency, spectralParams[0]);
    interpolateParameter(filterEmphasis, spectralParams[1]);
    interpolateParameter(filterContour, spectralParams[2]);
    interpolateParameter(oscModMix, spectralParams[3]);
    
    reformatData();
    
    return expandedTag;
}

void TT_SpectralInterpolator::filterTagVector(TagVector toSort)
{
    for(int i = 0 ; i < toSort.size() ; i++)
    {
        std::vector<float> patch = toSort[i];
        for(int j = 0 ; j < patch.size() ; j++)
        {
            switch(j)
            {
                case 0:
                    filterFrequency.push_back(patch[j]);
                break;
                case 1:
                    filterEmphasis.push_back(patch[j]);
                break;
                case 2:
                    filterContour.push_back(patch[j]);
                break;
                case 3:
                    oscModMix.push_back(patch[j]);
                break;
            }
        }
    }
}

void TT_SpectralInterpolator::interpolateParameter(std::vector<float> param, juce::Identifier paramID)
{
    auto [min, max] = std::minmax_element(param.begin(), param.end());
    
    DBG("interpolating " << paramID.toString());
    DBG("min = " << *min);
    DBG("max = " << *max);
    
    for(int i = 0 ; i < magnitudes.size() ; i++)
    {
        constexpr int nd[] = { 2 };
        int ni = scaleFactor;
        
        float xd[2] = {magnitudes[i], magnitudes[i+1]};
        float yd[2] = {param[i], param[i+1]};
        
        float xi[scaleFactor];
        for(int i = 0 ; i < scaleFactor ; i++)
            xi[i] = xd[0] + (xd[1] - xd[0]) / (scaleFactor - 1) * i;
        
        float yi[scaleFactor];
        interp
        (
            nd, ni, // Number of points
            yd, yi, // Output axis (y)
            xd, xi  // Input axis (x)
        );

        for(int i = 0 ; i < scaleFactor ; i++)
        {
            if(yi[i] < *min)
                yi[i] = *min;
            if(yi[i] > *max)
                yi[i] = *max;
             
            storeInterpolatedParameter(yi[i], paramID);
        }
    }
}

void TT_SpectralInterpolator::storeInterpolatedParameter(float value, juce::Identifier paramID)
{
    if(paramID == spectralParams[0])
    {
        newFreq.push_back(value);
    } else if(paramID == spectralParams[1])
    {
        newEmph.push_back(value);
    } else if(paramID == spectralParams[2])
    {
        newContour.push_back(value);
    } else if(paramID == spectralParams[3])
    {
        newMix.push_back(value);
    }
}

void TT_SpectralInterpolator::reformatData()
{
    patchCount = (int)tagVector.size() * scaleFactor;
    
    for(int i = 0 ; i < patchCount ; i++)
    {
        PATCH newPatch;
        //DBG("===== NEW PATCH " << i << " =====");
        newPatch.push_back(newFreq[i]);
        //DBG("FilterFrequency = " << newFreq[i]);
        newPatch.push_back(newEmph[i]);
        //DBG("FilterEmphasis = " << newEmph[i]);
        newPatch.push_back(newContour[i]);
        //DBG("OscModAmount = " << newAmount[i]);
        newPatch.push_back(newMix[i]);
        //DBG("OscModMix = " << newMix[i]);
        
        expandedTag.push_back(newPatch);
    }
}

void TT_SpectralInterpolator::reset()
{
    patchCount = 0;
    
    magnitudes.clear();
    filterFrequency.clear();
    filterEmphasis.clear();
    filterContour.clear();
    oscModMix.clear();
    
    newFreq.clear();
    newEmph.clear();
    newContour.clear();
    newMix.clear();
}
