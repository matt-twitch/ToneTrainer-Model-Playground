/*
  ==============================================================================

    TT_TemporalInterpolator.cpp
    Created: 30 Mar 2024 3:06:41pm
    Author:  Matt Twitchen

  ==============================================================================
*/

#include "TT_TemporalInterpolator.h"

#define spectralParams DataNodes::ParameterNodes::spectralParams
#define temporalParams DataNodes::ParameterNodes::temporalParams

using namespace mlinterp;

TagVector TT_TemporalInterpolator::interpolateTag(TagVector tag, std::vector<float> mags, int sf)
{
    reset();
    
    scaleFactor = sf;
    
    tagVector = tag;
    magnitudes = mags;
    filterTagVector(tag);
    
    interpolateParameter(filterAttack, temporalParams[1]);
    interpolateParameter(filterDecay, temporalParams[2]);
    interpolateParameter(filterSustain, temporalParams[3]);
    interpolateParameter(filterRelease, temporalParams[4]);
    interpolateParameter(vcaAttack, temporalParams[5]);
    interpolateParameter(vcaDecay, temporalParams[6]);
    interpolateParameter(vcaSustain, temporalParams[7]);
    interpolateParameter(vcaRelease, temporalParams[8]);
    interpolateParameter(filterContour, temporalParams[9]);
    
    reformatData();
    
    return expandedTag;
}

void TT_TemporalInterpolator::filterTagVector(TagVector toSort)
{
    for(int i = 0 ; i < toSort.size() ; i++)
    {
        std::vector<float> patch = toSort[i];
        for(int j = 0 ; j < patch.size() ; j++)
        {
            switch(j)
            {
                case 0:
                    filterAttack.push_back(patch[j]);
                break;
                case 1:
                    filterDecay.push_back(patch[j]);
                break;
                case 2:
                    filterSustain.push_back(patch[j]);
                break;
                case 3:
                    filterRelease.push_back(patch[j]);
                break;
                case 4:
                    vcaAttack.push_back(patch[j]);
                break;
                case 5:
                    vcaDecay.push_back(patch[j]);
                break;
                case 6:
                    vcaSustain.push_back(patch[j]);
                break;
                case 7:
                    vcaRelease.push_back(patch[j]);
                break;
                case 8:
                    filterContour.push_back(patch[j]);
                break;
            }
        }
    }
}

void TT_TemporalInterpolator::interpolateParameter(std::vector<float> param, juce::Identifier paramID)
{
    auto [min, max] = std::minmax_element(param.begin(), param.end());
    
    DBG("interpolating " << paramID.toString());
    //DBG("min = " << *min);
    //DBG("max = " << *max);
    
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

void TT_TemporalInterpolator::storeInterpolatedParameter(float value, juce::Identifier paramID)
{
    if(paramID == temporalParams[1])
    {
        newFilterAttack.push_back(value);
    } else if(paramID == temporalParams[2])
    {
        newFilterDecay.push_back(value);
    } else if(paramID == temporalParams[3])
    {
        newFilterSustain.push_back(value);
    } else if(paramID == temporalParams[4])
    {
        newFilterRelease.push_back(value);
    } else if(paramID == temporalParams[5])
    {
        newVcaAttack.push_back(value);
    } else if(paramID == temporalParams[6])
    {
        newVcaDecay.push_back(value);
    } else if(paramID == temporalParams[7])
    {
        newVcaSustain.push_back(value);
    } else if(paramID == temporalParams[8])
    {
        newVcaRelease.push_back(value);
    } else if(paramID == temporalParams[9])
    {
        newFilterContour.push_back(value);
    }
}

void TT_TemporalInterpolator::reformatData()
{
    patchCount = (int)tagVector.size() * scaleFactor;
    
    for(int i = 0 ; i < patchCount ; i++)
    {
        PATCH newPatch;
    
        newPatch.push_back(newFilterAttack[i]);
        newPatch.push_back(newFilterDecay[i]);
        newPatch.push_back(newFilterSustain[i]);
        newPatch.push_back(newFilterRelease[i]);
        newPatch.push_back(newVcaAttack[i]);
        newPatch.push_back(newVcaDecay[i]);
        newPatch.push_back(newVcaSustain[i]);
        newPatch.push_back(newVcaRelease[i]);
        newPatch.push_back(newFilterContour[i]);
        
        expandedTag.push_back(newPatch);
    }
}

void TT_TemporalInterpolator::reset()
{
    patchCount = 0;
    
    magnitudes.clear();
    filterAttack.clear();
    filterDecay.clear();
    filterSustain.clear();
    filterRelease.clear();
    vcaAttack.clear();
    vcaDecay.clear();
    vcaSustain.clear();
    vcaRelease.clear();
    filterContour.clear();
    
    newFilterAttack.clear();
    newFilterDecay.clear();
    newFilterSustain.clear();
    newFilterRelease.clear();
    newVcaAttack.clear();
    newVcaDecay.clear();
    newVcaSustain.clear();
    newVcaRelease.clear();
    newFilterContour.clear();
}
