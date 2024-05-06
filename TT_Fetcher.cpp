/*
  ==============================================================================

    TT_Fetcher.cpp
    Created: 15 Feb 2024 1:09:45pm
    Author:  Matt Twitchen

  ==============================================================================
*/

#include "TT_Fetcher.h"

#define spectralParams DataNodes::ParameterNodes::spectralParams
#define temporalParams DataNodes::ParameterNodes::temporalParams

#define spectralTags DataNodes::TagNodes::Spectral::Patch::patchTags
#define temporalTags DataNodes::TagNodes::Temporal::Patch::patchTags

#define temporalTagsForCheck DataNodes::TagNodes::Temporal::Patch::TagCheck::patchTags

#define spectralParents DataNodes::TagNodes::Spectral::Parents::parentTags
#define temporalParents DataNodes::TagNodes::Temporal::Parents::parentTags

TT_Fetcher::TT_Fetcher() : patchLibrary("/Users/twitch/TT-Testing/MM2 Library/")
{
    jassert(patchLibrary.isDirectory() && patchLibrary.exists());
}

TT_Fetcher::~TT_Fetcher()
{
    
}

juce::ValueTree TT_Fetcher::initialiseSpectralTag(juce::Identifier tag)
{
    juce::ValueTree tagTree {tag};
    
    for(int i = 0 ; i < spectralParams.size() ; i++)
        tagTree.setProperty(spectralParams[i], 0, nullptr);
    
    return tagTree;
}

juce::ValueTree TT_Fetcher::initialiseTemporalTag(juce::Identifier tag)
{
    juce::ValueTree tagTree {tag};
    
    for(int i = 0 ; i < temporalParams.size() ; i++)
        tagTree.setProperty(temporalParams[i], 0, nullptr);
    
    return tagTree;
}

void TT_Fetcher::parsePatchLibrary()
{
    juce::Array<juce::File> files;
    patchLibrary.findChildFiles(files, juce::File::findFiles, false, "*.xml");
    
    DBG("Parsing patch library ...");
    for(auto& file : files)
    {
        parseXML(file);
    }
    
    constructDataTree();
    isParsed = true;
}

void TT_Fetcher::parseXML(const juce::File& file)
{
    auto xml = juce::XmlDocument(file);
    std::unique_ptr<juce::XmlElement> root = xml.getDocumentElement();
    jassert(root != nullptr);
    
    juce::XmlElement* parameterNode;
    juce::XmlElement* macroNode;
    
    for(auto* node : root->getChildIterator())
    {
        juce::String nodeName = node->getTagName();
        if(nodeName == "metadata")
        {
            juce::String name = node->getStringAttribute("name");
            DBG("Parsing " << name << " ...");
            juce::String timbre = node->getStringAttribute("timbres");
            for(int i = 0 ; i < spectralTags.size() ; i++)
            {
                juce::String toCheck = spectralTags[i].toString();
                if(timbre.contains(toCheck))
                {
                    if(!spectralTagsToFetch.contains(toCheck))
                        spectralTagsToFetch.add(toCheck);
                }
            }
            
            juce::String type = node->getStringAttribute("types");
            DBG("type = " << type);
            for(int i = 0 ; i < temporalTags.size() ; i++)
            {
                juce::String toCheck = temporalTagsForCheck[i];
                if(type.contains(toCheck))
                {
                    DBG("contains = " << toCheck);
                    if(!temporalTagsToFetch.contains(toCheck))
                        temporalTagsToFetch.add(toCheck);
                }
            }
        } else if(nodeName == "parameter_data")
        {
            parameterNode = node;
        } else if(nodeName == "macro_data")
        {
            macroNode = node;
        }
    }
    parseParameterData(parameterNode, macroNode);
}

void TT_Fetcher::parseParameterData(juce::XmlElement* parameterNode, juce::XmlElement* macroNode)
{
    if(!spectralTagsToFetch.isEmpty())
    {
        for(int i = 0 ; i < spectralTagsToFetch.size() ; i++)
        {
            for(int j = 0 ; j < spectralTags.size() ; j++)
            {
                if(spectralTagsToFetch[i] == spectralTags[j].toString())
                {
                    juce::ValueTree nodeTree {spectralTags[j]};
                    for(int k = 0 ; k < spectralParams.size() ; k++)
                    {
                        juce::XmlElement* child = parameterNode->getChildByAttribute("id", spectralParams[k].toString());
                        float value = child->getDoubleAttribute("value");
                        nodeTree.setProperty(spectralParams[k], value, nullptr);
                    }
                    
                    for(auto* node : macroNode->getChildIterator())
                    {
                        for(auto* childNode : node->getChildIterator())
                        {
                            for(int k = 0 ; k < spectralParams.size() ; k++)
                            {
                                if(childNode->getStringAttribute("id") == spectralParams[k].toString())
                                {
                                    float value = node->getDoubleAttribute("amount");
                                    addMacroToProperty(nodeTree, spectralParams[k], value);
                                }
                            }
                        }
                    }
                    
                    fetchParentTree(spectralParents[j]).addChild(nodeTree, spectralIndex, nullptr);
                    spectralIndex++;
                }
            }
        }
        spectralTagsToFetch.clear();
    }
       
    // check vector size
    if(!temporalTagsToFetch.isEmpty())
    {
        for(int i = 0 ; i < temporalTagsToFetch.size() ; i++)
        {
            for(int j = 0 ; j < temporalTags.size() ; j++)
            {
                if(temporalTagsToFetch[i] == temporalTagsForCheck[j])
                {
                    juce::ValueTree nodeTree {temporalTags[j]};
                    for(int k = 0 ; k < temporalParams.size() ; k++)
                    {
                        juce::XmlElement* child = parameterNode->getChildByAttribute("id", temporalParams[k].toString());
                        float value = child->getDoubleAttribute("value");
                        nodeTree.setProperty(temporalParams[k], value, nullptr);
                    }
                    fetchParentTree(temporalParents[j]).addChild(nodeTree, temporalIndex, nullptr);
                    temporalIndex++;
                    
                    for(auto* node : macroNode->getChildIterator())
                    {
                        for(auto* childNode : node->getChildIterator())
                        {
                            for(int k = 0 ; k < temporalParams.size() ; k++)
                            {
                                if(childNode->getStringAttribute("id") == temporalParams[k].toString())
                                {
                                    float value = node->getDoubleAttribute("amount");
                                    addMacroToProperty(nodeTree, temporalParams[k], value);
                                }
                            }
                        }
                    }
                }
            }
        }
        temporalTagsToFetch.clear();
    }
}

void TT_Fetcher::constructDataTree()
{
    spectralTree.addChild(brightParent, 0, nullptr);
    spectralTree.addChild(darkParent, 1, nullptr);
    spectralTree.addChild(resonantParent, 2, nullptr);
    spectralTree.addChild(softParent, 3, nullptr);
    
    temporalTree.addChild(pluckParent, 0, nullptr);
    temporalTree.addChild(longRelParent, 1, nullptr);
    temporalTree.addChild(swellParent, 2, nullptr);
    temporalTree.addChild(shortParent, 3, nullptr);
    
    dataTree.addChild(spectralTree, 0, nullptr);
    dataTree.addChild(temporalTree, 1, nullptr);
}
