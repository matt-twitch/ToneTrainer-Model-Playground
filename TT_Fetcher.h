/*
  ==============================================================================

    TT_Fetcher.h
    Created: 15 Feb 2024 1:09:45pm
    Author:  Matt Twitchen

  ==============================================================================
*/

#pragma once
#include "TT_DataNodes.h"

/*
 TO DO:
 - initialise methods should add properties before being added to parent
 - separate tags for filter and normal envelope
 - redo tree initialisation process
 - data cleaning
 
 
 ON COMPLETION
 - Add tag parents to main spectral temporal trees
 - Add tag grouping trees to main data node
 
 */

class TT_Fetcher
{
public:
    
    TT_Fetcher();
    ~TT_Fetcher();
    
    juce::ValueTree initialiseSpectralTag(juce::Identifier tag); // returns an instance of a spectral tag
    juce::ValueTree initialiseTemporalTag(juce::Identifier tag); // returns an instance of a temporal tag
    
    void parsePatchLibrary();
    void parseXML(const juce::File& file);
    void parseParameterData(juce::XmlElement* parameterNode, juce::XmlElement* macroNode);
    
    void constructDataTree();
    std::shared_ptr<juce::ValueTree> getDataTree() { return std::make_shared<juce::ValueTree>(dataTree); }
    
    bool getState() { return isParsed; }
    
private:
    
    juce::ValueTree fetchParentTree(juce::Identifier toFetch)
    {
        for(int i = 0 ; i < parents.size() ; i++)
        {
            if(parents[i].hasType(toFetch))
                return parents[i];
        }
    }
    
    void addMacroToProperty(juce::ValueTree tree, juce::Identifier paramID, float valueToAdd)
    {
        float value = (float)tree.getProperty(paramID) + valueToAdd;
        
        if(value > 1)
            value = 1.f;
        
        tree.setProperty(paramID, value, nullptr);
    }
    
    void sortTree(juce::ValueTree treeToSort)
    {
        treeToSort.sort(sorter, nullptr, false);
    }
    
    juce::File patchLibrary;
    
    juce::ValueTree dataTree {DataNodes::Data};
    juce::ValueTree spectralTree {DataNodes::TypeNodes::Spectral};
    juce::ValueTree temporalTree {DataNodes::TypeNodes::Temporal};
    
    juce::ValueTree brightParent {DataNodes::TagNodes::Spectral::Parents::BrightParent};
    juce::ValueTree darkParent {DataNodes::TagNodes::Spectral::Parents::DarkParent};
    juce::ValueTree resonantParent {DataNodes::TagNodes::Spectral::Parents::ResonantParent};
    juce::ValueTree softParent {DataNodes::TagNodes::Spectral::Parents::SoftParent};
    
    juce::ValueTree pluckParent {DataNodes::TagNodes::Temporal::Parents::PluckParent};
    juce::ValueTree longRelParent {DataNodes::TagNodes::Temporal::Parents::LongReleaseParent};
    juce::ValueTree swellParent {DataNodes::TagNodes::Temporal::Parents::SwellParent};
    juce::ValueTree shortParent {DataNodes::TagNodes::Temporal::Parents::ShortParent};
    
    juce::Array<juce::ValueTree> parents = {brightParent, darkParent, resonantParent, softParent,
                                            pluckParent, longRelParent, swellParent, shortParent};
    
    juce::Array<juce::String> spectralTagsToFetch;
    juce::Array<juce::String> temporalTagsToFetch;
    
    int spectralIndex = 0;
    int temporalIndex = 0;
    bool isParsed = false;
    
    struct Sorter
    {
        int compareElements (const juce::ValueTree& first, const juce::ValueTree& second)
        {
            return first.getProperty ("text").toString().compare (second.getProperty ("text").toString());
        }
    } sorter;
};
