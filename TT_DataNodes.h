/*
  ==============================================================================

    TT_DateNodes.h
    Created: 16 Feb 2024 10:41:46am
    Author:  Matt Twitchen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace DataNodes
{
    
    static const inline juce::Identifier Data {"Data"};
    static const inline juce::Identifier ParamValue {"Param_Value"};

    namespace TypeNodes
    {
        static const inline juce::Identifier Spectral {"Spectral"};
        static const inline juce::Identifier Temporal {"Temporal"};
    };
    
    namespace TagNodes
    {
        namespace Spectral
        {
            namespace Parents
            {
                static const inline juce::Identifier BrightParent {"Bright_Parent"};
                static const inline juce::Identifier DarkParent {"Dark_Parent"};
                static const inline juce::Identifier ResonantParent {"Resonant_Parent"};
                static const inline juce::Identifier SoftParent {"Soft_Parent"};
            
                const juce::Array<juce::Identifier> parentTags {BrightParent, DarkParent,
                                                                ResonantParent, SoftParent};
            }
        
            namespace Patch
            {
                static const inline juce::Identifier Bright {"Bright"};
                static const inline juce::Identifier Dark {"Dark"};
                static const inline juce::Identifier Resonant {"Resonant"};
                static const inline juce::Identifier Soft {"Soft"};
            
                const juce::Array<juce::Identifier> patchTags {Bright, Dark, Resonant, Soft};
            };
        };
    
        namespace Temporal
        {
            namespace Parents
            {
                static const inline juce::Identifier PluckParent {"Pluck_Parent"};
                static const inline juce::Identifier LongReleaseParent {"Long_Release_Parent"};
                static const inline juce::Identifier SwellParent {"Swell_Parent"};
                static const inline juce::Identifier ShortParent {"Short_Parent"};
            
                const juce::Array<juce::Identifier> parentTags {PluckParent, LongReleaseParent,
                                                                SwellParent, ShortParent};
            };
        
            namespace Patch
            {
                static const inline juce::Identifier Pluck {"Pluck"};
                static const inline juce::Identifier LongRelease {"Long_Release"};
                static const inline juce::Identifier Swell {"Swell"};
                static const inline juce::Identifier Short {"Short"};
            
                const juce::Array<juce::Identifier> patchTags {Pluck, LongRelease, Swell, Short};
            
                namespace TagCheck // long release in the patch library uses a space, however identifiers can't use spaces
                {
                static const inline juce::String Pluck = "Pluck";
                static const inline juce::String LongRelease = "Long Release";
                static const inline juce::String Swell = "Swell";
                static const inline juce::String Short = "Short";
            
                const juce::Array<juce::String> patchTags {Pluck, LongRelease, Swell, Short};
                }
            };
        };
    };

    namespace ParameterNodes
    {
        static const inline juce::Array<juce::Identifier> spectralParams = {"FilterFrequency", "FilterEmphasis", "FilterContour", "Osc2Detune", "Osc3Detune"};
                                                                     
        static const inline juce::Array<juce::Identifier> temporalParams = {"EnvType", "FilterAttack", "FilterDecay", "FilterSustain", "FilterRelease", "VcaAttack", "VcaDecay", "VcaSustain", "VcaRelease", "FilterContour"};
    };
}
