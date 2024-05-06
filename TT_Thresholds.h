/*
  ==============================================================================

    TT_Thresholds.h
    Created: 29 Mar 2024 4:36:51pm
    Author:  Matt Twitchen

  ==============================================================================
*/

#pragma once

namespace Thresholds
{
    namespace Spectral
    {
        namespace Bright
        {
            static float filterFrequency = 0.2;
        }

        namespace Dark
        {
            static float filterFrequency = 0.1;
        }

        namespace Resonant
        {
            static float filterEmphasis = 0.5;
        }

        namespace Soft
        {
            static float filterFrequency = 0.1;
            static float filterEmphasis = 0.1;
        }
    }
    namespace Temporal
    {
        namespace Pluck
        {
            static float filterAttack = 0.1;
            static float vcaAttack = 0.1;
        }

        namespace LongRelease
        {
            static float filterRelease = 0.3;
            static float vcaRelease = 0.3;
        }

        namespace Swell
        {
            static float filterAttack = 0.4;
            static float vcaAttack = 0.2;
        }

        namespace Short
        {
            static float vcaAttack = 0.1;
            static float vcaSustain = 0.1;
        }
    }
}
