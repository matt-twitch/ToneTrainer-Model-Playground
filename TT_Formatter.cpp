/*
  ==============================================================================

    Formater.cpp
    Created: 26 Jan 2024 11:04:38am
    Author:  Matt Twitchen

  ==============================================================================
*/

#include "TT_Formatter.h"

#define spectralParams DataNodes::ParameterNodes::spectralParams
#define temporalParams DataNodes::ParameterNodes::temporalParams

#define spectralTags DataNodes::TagNodes::Spectral::Patch::patchTags
#define temporalTags DataNodes::TagNodes::Temporal::Patch::patchTags

#define spectralParents DataNodes::TagNodes::Spectral::Parents::parentTags
#define temporalParents DataNodes::TagNodes::Temporal::Parents::parentTags

TT_Formatter::TT_Formatter(TT_Augmenter* tta)
{
    augmenter = tta;
    jassert(augmenter != nullptr);
    // add state check here
    dataTree = augmenter->getDataTree();
    jassert(dataTree != nullptr);
    
    spectralTree = dataTree->getChildWithName(DataNodes::TypeNodes::Spectral);
    temporalTree = dataTree->getChildWithName(DataNodes::TypeNodes::Temporal);
}

TT_Formatter::~TT_Formatter()
{
    
}

void TT_Formatter::formatSpectralData()
{
    juce::ValueTree brightParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::BrightParent);
    for(int i = 0 ; i < brightParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = brightParent.getChild(i);
        vec_t childData;
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            childData.push_back(child.getProperty(spectralParams[j]));
        }
        spectralData.data.push_back(childData);

        vec_t encodedLabel = {1, 0 , 0 , 0, 0};
        spectralData.labels.push_back(encodedLabel);
    }
    
    juce::ValueTree darkParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::DarkParent);
    for(int i = 0 ; i < darkParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = darkParent.getChild(i);
        vec_t childData;
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            childData.push_back(child.getProperty(spectralParams[j]));
        }
        spectralData.data.push_back(childData);

        vec_t encodedLabel = {0, 1, 0, 0, 0};
        spectralData.labels.push_back(encodedLabel);
    }
    
    juce::ValueTree resonantParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::ResonantParent);
    for(int i = 0 ; i < resonantParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = resonantParent.getChild(i);
        vec_t childData;
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            childData.push_back(child.getProperty(spectralParams[j]));
        }
        spectralData.data.push_back(childData);

        vec_t encodedLabel = {0, 0, 1, 0, 0};
        spectralData.labels.push_back(encodedLabel);
    }
    
    juce::ValueTree softParent = spectralTree.getChildWithName(DataNodes::TagNodes::Spectral::Parents::SoftParent);
    for(int i = 0 ; i < softParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = softParent.getChild(i);
        vec_t childData;
        for(int j = 0 ; j < spectralParams.size() ; j++)
        {
            childData.push_back(child.getProperty(spectralParams[j]));
        }
        spectralData.data.push_back(childData);

        vec_t encodedLabel = {0, 0, 0, 1, 0};
        spectralData.labels.push_back(encodedLabel);
    }
}

void TT_Formatter::formatTemporalData()
{
    juce::ValueTree pluckParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::PluckParent);
    for(int i = 0 ; i < pluckParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = pluckParent.getChild(i);
        vec_t childData;
        for(int j = 1 ; j < temporalParams.size() ; j++)
        {
            childData.push_back(child.getProperty(temporalParams[j]));
        }
        temporalData.data.push_back(childData);

        vec_t encodedLabel = {0, 0, 0, 1, 0, 0, 0, 0, 0};
        temporalData.labels.push_back(encodedLabel);
    }
    
    juce::ValueTree longReleaseParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::LongReleaseParent);
    for(int i = 0 ; i < longReleaseParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = longReleaseParent.getChild(i);
        vec_t childData;
        for(int j = 1 ; j < temporalParams.size() ; j++)
        {
            childData.push_back(child.getProperty(temporalParams[j]));
        }
        temporalData.data.push_back(childData);

        vec_t encodedLabel = {0, 0, 1, 0, 0, 0, 0, 0, 0};
        temporalData.labels.push_back(encodedLabel);
    }
    
    juce::ValueTree swellParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::SwellParent);
    for(int i = 0 ; i < swellParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = swellParent.getChild(i);
        vec_t childData;
        for(int j = 1 ; j < temporalParams.size() ; j++)
        {
            childData.push_back(child.getProperty(temporalParams[j]));
        }
        temporalData.data.push_back(childData);

        vec_t encodedLabel = {0, 1, 0, 0, 0, 0, 0, 0, 0};
        temporalData.labels.push_back(encodedLabel);
    }
    
    juce::ValueTree shortParent = temporalTree.getChildWithName(DataNodes::TagNodes::Temporal::Parents::ShortParent);
    for(int i = 0 ; i < shortParent.getNumChildren() ; i++)
    {
        juce::ValueTree child = shortParent.getChild(i);
        vec_t childData;
        for(int j = 1 ; j < temporalParams.size() ; j++)
        {
            childData.push_back(child.getProperty(temporalParams[j]));
        }
        temporalData.data.push_back(childData);

        vec_t encodedLabel = {1, 0, 0, 0, 0, 0, 0, 0, 0};
        temporalData.labels.push_back(encodedLabel);
    }
}

void TT_Formatter::scrambleSpectralData()
{
    std::vector<size_t> indices(spectralData.data.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    tensor_t shuffledData (indices.size());
    tensor_t shuffledLabels (indices.size());
    
    for (size_t i = 0; i < indices.size(); ++i)
    {
        shuffledData[i] = spectralData.data[indices[i]];
        shuffledLabels[i] = spectralData.labels[indices[i]];
    }
    
    spectralData.data = std::move(shuffledData);
    spectralData.labels = std::move(shuffledLabels);
}

void TT_Formatter::scrambleTemporalData()
{
    std::vector<size_t> indices(temporalData.data.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    tensor_t shuffledData (indices.size());;
    tensor_t shuffledLabels (indices.size());;
    
    for (size_t i = 0; i < indices.size(); ++i)
    {
        shuffledData[i] = temporalData.data[indices[i]];
        shuffledLabels[i] = temporalData.labels[indices[i]];
    }
    
    temporalData.data = std::move(shuffledData);
    temporalData.labels = std::move(shuffledLabels);
}
