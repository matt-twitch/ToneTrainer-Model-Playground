/*
  ==============================================================================

    TT_Augmenter.cpp
    Created: 26 Jan 2024 11:04:26am
    Author:  Matt Twitchen

  ==============================================================================
*/

#include "TT_Augmenter.h"

#define spectralParams DataNodes::ParameterNodes::spectralParams
#define temporalParams DataNodes::ParameterNodes::temporalParams

#define spectralChild DataNodes::TagNodes::Spectral::Patch::patchTags
#define temporalChild DataNodes::TagNodes::Temporal::Patch::patchTags

#define spectralParents DataNodes::TagNodes::Spectral::Parents::parentTags
#define temporalParents DataNodes::TagNodes::Temporal::Parents::parentTags

TT_Augmenter::TT_Augmenter(TT_Fetcher* ttf)
{
    fetcher = ttf;
    jassert(fetcher->getState());
    
    dataTree = fetcher->getDataTree();
    jassert(dataTree != nullptr);
    
    spectralTree = dataTree->getChildWithName(DataNodes::TypeNodes::Spectral);
    temporalTree = dataTree->getChildWithName(DataNodes::TypeNodes::Temporal);
}

TT_Augmenter::~TT_Augmenter()
{
    fetcher = nullptr;
    dataTree = nullptr;
}

void TT_Augmenter::fetchSpectralData()
{
    jassert(spectralTree.isValid());
    
    fetchBrightData();
    fetchDarkData();
    fetchResonantData();
    fetchSoftData();
}

void TT_Augmenter::fetchBrightData()
{
    juce::ValueTree brightParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::BrightParent);
    
    float avgFilterFrequencyBright = 0;
    for(int i = 0 ; i < brightParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = brightParent.getChild(i);
        avgFilterFrequencyBright += (float)child.getProperty(spectralParams[0]);
    }
    avgFilterFrequencyBright /= brightParent.getNumChildren();
    
    TagVector bright;
    std::vector<float> brightMags;
    for(int i = 0 ; i < brightParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = brightParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(spectralParams[0]) <= Thresholds::Spectral::Bright::filterFrequency)
            child.setProperty(spectralParams[0], avgFilterFrequencyBright, nullptr);
        
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            patch.push_back(child.getProperty(spectralParams[j]));
        }
        
        bright.push_back(patch);
        brightMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(bright);
    sortMagVector(brightMags);
    
    spectralTags.push_back(bright);
    spectralMags.push_back(brightMags);
}

void TT_Augmenter::fetchDarkData()
{
    juce::ValueTree darkParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::DarkParent);
    
    float avgFilterFrequencyDark = 0;
    for(int i = 0 ; i < darkParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = darkParent.getChild(i);
        avgFilterFrequencyDark += (float)child.getProperty(spectralParams[0]);
    }
    avgFilterFrequencyDark /= darkParent.getNumChildren();
    
    TagVector dark;
    std::vector<float> darkMags;
    for(int i = 0 ; i < darkParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = darkParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(spectralParams[0]) <= Thresholds::Spectral::Dark::filterFrequency)
            child.setProperty(spectralParams[0], avgFilterFrequencyDark, nullptr);
        
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            patch.push_back(child.getProperty(spectralParams[j]));
        }
        
        dark.push_back(patch);
        darkMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(dark);
    sortMagVector(darkMags);
    
    spectralTags.push_back(dark);
    spectralMags.push_back(darkMags);
}

void TT_Augmenter::fetchResonantData()
{
    juce::ValueTree resonantParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::ResonantParent);
    
    float avgFilterEmphasisReso = 0;
    for(int i = 0 ; i < resonantParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = resonantParent.getChild(i);
        avgFilterEmphasisReso += (float)child.getProperty(spectralParams[1]);
    }
    avgFilterEmphasisReso /= resonantParent.getNumChildren();
    
    TagVector resonant;
    std::vector<float> resoMags;
    for(int i = 0 ; i < resonantParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = resonantParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(spectralParams[1]) <= Thresholds::Spectral::Resonant::filterEmphasis)
            child.setProperty(spectralParams[1], avgFilterEmphasisReso, nullptr);
        
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            patch.push_back(child.getProperty(spectralParams[j]));
        }

        resonant.push_back(patch);
        resoMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(resonant);
    sortMagVector(resoMags);
    
    spectralTags.push_back(resonant);
    spectralMags.push_back(resoMags);
}

void TT_Augmenter::fetchSoftData()
{
    juce::ValueTree softParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::SoftParent);
    
    float avgFilterFrequency = 0;
    for(int i = 0 ; i < softParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = softParent.getChild(i);
        avgFilterFrequency += (float)child.getProperty(spectralParams[0]);
    }
    avgFilterFrequency /= softParent.getNumChildren();
    
    float avgFilterEmphasis = 0;
    for(int i = 0 ; i < softParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = softParent.getChild(i);
        avgFilterEmphasis += (float)child.getProperty(spectralParams[1]);
    }
    avgFilterEmphasis /= softParent.getNumChildren();
    
    TagVector soft;
    std::vector<float> softMags;
    for(int i = 0 ; i < softParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = softParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(spectralParams[0]) <= Thresholds::Spectral::Soft::filterFrequency)
            child.setProperty(spectralParams[0], avgFilterFrequency, nullptr);
        
        if((float)child.getProperty(spectralParams[1]) <= Thresholds::Spectral::Soft::filterEmphasis)
            child.setProperty(spectralParams[1], avgFilterEmphasis, nullptr);
        
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            patch.push_back(child.getProperty(spectralParams[j]));
        }

        soft.push_back(patch);
        softMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(soft);
    sortMagVector(softMags);
    
    spectralTags.push_back(soft);
    spectralMags.push_back(softMags);
}

void TT_Augmenter::fetchTemporalData()
{
    jassert(temporalTree.isValid());
    
    fetchPluckData();
    fetchLongReleaseData();
    fetchSwellData();
    fetchShortData();
}

void TT_Augmenter::fetchPluckData()
{
    juce::ValueTree pluckParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::PluckParent);
    
    float avgFilterAttack = 0;
    for(int i = 0 ; i < pluckParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = pluckParent.getChild(i);
        avgFilterAttack += (float)child.getProperty(temporalParams[1]);
    }
    avgFilterAttack /= pluckParent.getNumChildren();
    
    float avgVcaAttack = 0;
    for(int i = 0 ; i < pluckParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = pluckParent.getChild(i);
        avgVcaAttack += (float)child.getProperty(temporalParams[5]);
    }
    avgVcaAttack /= pluckParent.getNumChildren();
    
    TagVector pluck;
    std::vector<float> pluckMags;
    for(int i = 0 ; i < pluckParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = pluckParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(temporalParams[1]) >= Thresholds::Temporal::Pluck::filterAttack)
            child.setProperty(temporalParams[1], avgFilterAttack, nullptr);
        
        if((float)child.getProperty(temporalParams[5]) >= Thresholds::Temporal::Pluck::vcaAttack)
            child.setProperty(temporalParams[5], avgVcaAttack, nullptr);
        
        for(int j = 1 ; j < temporalParams.size() ; j++) // start at 1 to miss env type
        {
            patch.push_back(child.getProperty(temporalParams[j]));
        }
        
        if((float)child.getProperty(temporalParams[0]) == 0.f)
        {
            patch.at(5) = 0.f; // filter release
            patch.at(8) = 0.f; // vca release
        }
        
        child.removeProperty(temporalParams[0], nullptr);
        pluck.push_back(patch);
        pluckMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(pluck);
    sortMagVector(pluckMags);
    
    temporalTags.push_back(pluck);
    temporalMags.push_back(pluckMags);
}

void TT_Augmenter::fetchLongReleaseData()
{
    juce::ValueTree longReleaseParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::LongReleaseParent);
    
    float avgFilterRelease = 0;
    for(int i = 0 ; i < longReleaseParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = longReleaseParent.getChild(i);
        avgFilterRelease += (float)child.getProperty(temporalParams[4]);
    }
    avgFilterRelease /= longReleaseParent.getNumChildren();
    
    float avgVcaRelease = 0;
    for(int i = 0 ; i < longReleaseParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = longReleaseParent.getChild(i);
        avgVcaRelease += (float)child.getProperty(temporalParams[8]);
    }
    avgVcaRelease /= longReleaseParent.getNumChildren();
    
    TagVector longRelease;
    std::vector<float> longReleaseMags;
    for(int i = 0 ; i < longReleaseParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = longReleaseParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(temporalParams[4]) <= Thresholds::Temporal::LongRelease::filterRelease)
            child.setProperty(temporalParams[4], avgFilterRelease, nullptr);
        
        if((float)child.getProperty(temporalParams[8]) <= Thresholds::Temporal::LongRelease::vcaRelease)
            child.setProperty(temporalParams[8], avgVcaRelease, nullptr);
        
        for(int j = 1 ; j < temporalParams.size() ; j++) // start at 1 to miss env type
        {
            patch.push_back(child.getProperty(temporalParams[j]));
        }
        
        child.removeProperty(temporalParams[0], nullptr);
        longRelease.push_back(patch);
        longReleaseMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(longRelease);
    sortMagVector(longReleaseMags);
    
    temporalTags.push_back(longRelease);
    temporalMags.push_back(longReleaseMags);
}

void TT_Augmenter::fetchSwellData()
{
    juce::ValueTree swellParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::SwellParent);
    
    float avgFilterAttack = 0;
    for(int i = 0 ; i < swellParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = swellParent.getChild(i);
        avgFilterAttack += (float)child.getProperty(temporalParams[1]);
    }
    avgFilterAttack /= swellParent.getNumChildren();
    
    float avgVcaAttack = 0;
    for(int i = 0 ; i < swellParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = swellParent.getChild(i);
        avgVcaAttack += (float)child.getProperty(temporalParams[5]);
    }
    avgVcaAttack /= swellParent.getNumChildren();
    
    TagVector swell;
    std::vector<float> swellMags;
    for(int i = 0 ; i < swellParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = swellParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(temporalParams[1]) <= Thresholds::Temporal::Swell::filterAttack)
            child.setProperty(temporalParams[1], avgFilterAttack, nullptr);
        
        if((float)child.getProperty(temporalParams[5]) <= Thresholds::Temporal::Swell::vcaAttack)
            child.setProperty(temporalParams[5], avgVcaAttack, nullptr);
        
        for(int j = 1 ; j < temporalParams.size() ; j++) // start at 1 to miss env type
        {
            patch.push_back(child.getProperty(temporalParams[j]));
        }
        
        if((float)child.getProperty(temporalParams[0]) == 0.f)
        {
            patch.at(5) = 0.f; // filter release
            patch.at(8) = 0.f; // vca release
        }
        
        child.removeProperty(temporalParams[0], nullptr);
        swell.push_back(patch);
        swellMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(swell);
    sortMagVector(swellMags);
    
    temporalTags.push_back(swell);
    temporalMags.push_back(swellMags);
}

void TT_Augmenter::fetchShortData()
{
    juce::ValueTree shortParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::ShortParent);
    
    float avgVcaAttack = 0;
    for(int i = 0 ; i < shortParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = shortParent.getChild(i);
        avgVcaAttack += (float)child.getProperty(temporalParams[5]);
    }
    avgVcaAttack /= shortParent.getNumChildren();
    
    float avgVcaSustain = 0;
    for(int i = 0 ; i < shortParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = shortParent.getChild(i);
        avgVcaSustain += (float)child.getProperty(temporalParams[7]);
    }
    avgVcaSustain /= shortParent.getNumChildren();
    
    TagVector shortPatches;
    std::vector<float> shortMags;
    for(int i = 0 ; i < shortParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = shortParent.getChild(i);
        PATCH patch;
        
        if((float)child.getProperty(temporalParams[5]) >= Thresholds::Temporal::Short::vcaAttack)
            child.setProperty(temporalParams[5], avgVcaAttack, nullptr);
        if((float)child.getProperty(temporalParams[7]) >= Thresholds::Temporal::Short::vcaSustain)
            child.setProperty(temporalParams[7], avgVcaSustain, nullptr);
        
        for(int j = 1 ; j < temporalParams.size() ; j++) // start at 1 to miss env type
        {
            patch.push_back(child.getProperty(temporalParams[j]));
        }
        
        if((float)child.getProperty(temporalParams[0]) == 0.f)
        {
            patch.at(5) = 0.f; // filter release
            patch.at(8) = 0.f; // vca release
        }
        
        child.removeProperty(temporalParams[0], nullptr);
        shortPatches.push_back(patch);
        shortMags.push_back(calculateMagnitude(patch));
    }
    
    sortTagVector(shortPatches);
    sortMagVector(shortMags);
    
    temporalTags.push_back(shortPatches);
    temporalMags.push_back(shortMags);
}

void TT_Augmenter::augmentSpectralTags()
{
    if(mode == INTERP)
        interpolateSpectralTags();
    else if(mode == NOISE)
        injectNoiseSpectralTags();
}

void TT_Augmenter::augmentTemporalTags()
{
    if(mode == INTERP)
        interpolateTemporalTags();
    else if(mode == NOISE)
        injectNoiseTemporalTags();
}

void TT_Augmenter::interpolateSpectralTags()
{
    TagVector bright = spectralInterpolator.interpolateTag(spectralTags[0], spectralMags[0], scaleFactor);
    TagVector dark = spectralInterpolator.interpolateTag(spectralTags[1], spectralMags[1], scaleFactor);
    TagVector resonant = spectralInterpolator.interpolateTag(spectralTags[2], spectralMags[2], scaleFactor);
    TagVector soft = spectralInterpolator.interpolateTag(spectralTags[3], spectralMags[3], scaleFactor);
    
    DBG("===== Bright Error =====");
    errorCheck(spectralTags[0], bright);
    DBG("===== Dark Error =====");
    errorCheck(spectralTags[1], dark);
    DBG("===== Resonant Error =====");
    errorCheck(spectralTags[2], resonant);
    DBG("===== Soft Error =====");
    errorCheck(spectralTags[3], soft);
    
    addAugmentedSpectralTags(bright, spectralParents[0], spectralChild[0]);
    addAugmentedSpectralTags(dark, spectralParents[1], spectralChild[1]);
    addAugmentedSpectralTags(resonant, spectralParents[2], spectralChild[2]);
    addAugmentedSpectralTags(soft, spectralParents[3], spectralChild[3]);
}

void TT_Augmenter::interpolateTemporalTags()
{
    TagVector pluck = temporalInterpolator.interpolateTag(temporalTags[0], temporalMags[0], scaleFactor);
    TagVector longRelease = temporalInterpolator.interpolateTag(temporalTags[1], temporalMags[1], scaleFactor);
    TagVector swell = temporalInterpolator.interpolateTag(temporalTags[2], temporalMags[2], scaleFactor);
    TagVector shortTag = temporalInterpolator.interpolateTag(temporalTags[3], temporalMags[3], scaleFactor);
    
    DBG("===== Pluck Error =====");
    errorCheck(temporalTags[0], pluck);
    DBG("===== Long Release Error =====");
    errorCheck(temporalTags[1], longRelease);
    DBG("===== Swell Error =====");
    errorCheck(temporalTags[2], swell);
    DBG("===== Short Error =====");
    errorCheck(temporalTags[3], shortTag);
    
    addAugmentedTemporalTags(pluck, temporalParents[0], temporalChild[0]);
    addAugmentedTemporalTags(longRelease, temporalParents[1], temporalChild[1]);
    addAugmentedTemporalTags(swell, temporalParents[2], temporalChild[2]);
    addAugmentedTemporalTags(shortTag, temporalParents[3], temporalChild[3]);
}

void TT_Augmenter::injectNoiseSpectralTags()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-0.15, 0.15);
    
    for(int i = 0 ; i < scaleFactor ; i++)
    {
        TagVector bright = spectralTags[0];
        for(int j = 0 ; j < bright.size() ; j++)
        {
            PATCH patch = bright[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Bright Error =====");
        errorCheck(spectralTags[0], bright);
        addAugmentedSpectralTags(bright, spectralParents[0], spectralChild[0]);
        
        TagVector dark = spectralTags[1];
        for(int j = 0 ; j < dark.size() ; j++)
        {
            PATCH patch = dark[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Dark Error =====");
        errorCheck(spectralTags[1], dark);
        addAugmentedSpectralTags(dark, spectralParents[1], spectralChild[1]);
        
        TagVector resonant = spectralTags[2];
        for(int j = 0 ; j < resonant.size() ; j++)
        {
            PATCH patch = resonant[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Resonant Error =====");
        errorCheck(spectralTags[2], resonant);
        addAugmentedSpectralTags(resonant, spectralParents[2], spectralChild[2]);
        
        TagVector soft = spectralTags[3];
        for(int j = 0 ; j < soft.size() ; j++)
        {
            PATCH patch = soft[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Soft Error =====");
        errorCheck(spectralTags[3], soft);
        addAugmentedSpectralTags(soft, spectralParents[3], spectralChild[3]);
    }
}

void TT_Augmenter::injectNoiseTemporalTags()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-0.15, 0.15);
    
    for(int i = 0 ; i < scaleFactor ; i++)
    {
        TagVector pluck = temporalTags[0];
        for(int j = 0 ; j < pluck.size() ; j++)
        {
            PATCH patch = pluck[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Pluck Error =====");
        errorCheck(temporalTags[0], pluck);
        addAugmentedTemporalTags(pluck, temporalParents[0], temporalChild[0]);
        
        TagVector longRelease = temporalTags[1];
        for(int j = 0 ; j < longRelease.size() ; j++)
        {
            PATCH patch = longRelease[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Long Release Error =====");
        errorCheck(temporalTags[1], longRelease);
        addAugmentedTemporalTags(longRelease, temporalParents[1], temporalChild[1]);
        
        TagVector swell = temporalTags[2];
        for(int j = 0 ; j < swell.size() ; j++)
        {
            PATCH patch = swell[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Swell Error =====");
        errorCheck(temporalTags[2], swell);
        addAugmentedTemporalTags(swell, temporalParents[2], temporalChild[2]);
        
        TagVector shortTag = temporalTags[3];
        for(int j = 0 ; j < swell.size() ; j++)
        {
            PATCH patch = shortTag[j];
            for(int k = 0 ; k < patch.size() ; k++)
            {
                patch[k] += distribution(gen);
            }
        }
        
        DBG("===== Short Error =====");
        errorCheck(temporalTags[3], shortTag);
        addAugmentedTemporalTags(shortTag, temporalParents[3], temporalChild[3]);
    }
}

void TT_Augmenter::addAugmentedSpectralTags(TagVector toAdd, juce::Identifier parent, juce::Identifier child)
{
    juce::ValueTree parentTree = spectralTree.getChildWithName(parent);
    
    for(int i = 0 ; i < toAdd.size() ; i++)
    {
        juce::ValueTree newChild {child};
        for(int j = 0 ; j < toAdd[i].size() ; j++)
        {
            newChild.setProperty(spectralParams[j], toAdd[i][j], nullptr);
        }
        parentTree.appendChild(newChild, nullptr);
    }
}

void TT_Augmenter::addAugmentedTemporalTags(TagVector toAdd, juce::Identifier parent, juce::Identifier child)
{
    juce::ValueTree parentTree = temporalTree.getChildWithName(parent);
    
    for(int i = 0 ; i < toAdd.size() ; i++)
    {
        juce::ValueTree newChild {child};
        for(int j = 1 ; j < toAdd[i].size() ; j++)
        {
            newChild.setProperty(temporalParams[j], toAdd[i][j], nullptr);
        }
        parentTree.appendChild(newChild, nullptr);
    }
}

void TT_Augmenter::sortTagVector(TagVector input)
{
    std::sort(input.begin(), input.end(), [this] (PATCH a, PATCH b) { return calculateMagnitude(a) > calculateMagnitude(b); });
}

void TT_Augmenter::sortMagVector(std::vector<float> input)
{
    std::sort(input.begin(), input.end(), [this] (float a, float b) { return a > b; });
}

void TT_Augmenter::errorCheck(TagVector original, TagVector expanded)
{
    std::vector<float> originalMags;
    std::vector<float> expandedMags;
    
    for(int i = 0 ; i < original.size() ; i++)
        originalMags.push_back(calculateMagnitude(original[i]));
    
    for(int i = 0 ; i < original.size() ; i++)
        expandedMags.push_back(calculateMagnitude(expanded[i]));
    
    float originalMean = 0;
    for(int i = 0 ; i < originalMags.size() ; i++)
        originalMean += originalMags[i];
    
    originalMean /= originalMags.size();
    
    float expandedMean = 0;
    for(int i = 0 ; i < expandedMags.size() ; i++)
        expandedMean += expandedMags[i];
    
    expandedMean /= expandedMags.size();
    
    DBG("original mean = " << originalMean);
    DBG("expanded mean = " << expandedMean);
}
