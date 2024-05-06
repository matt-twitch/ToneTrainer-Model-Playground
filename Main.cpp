/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "TT_Formatter.h"
#include "TT_Spectral.h"
#include "TT_Temporal.h"
#include "TT_Augmenter.h"

/*
 TO DO
 - Logic for handling macro assignments
 - Adding augmented data to main data tree
 - GUI
 */

//==============================================================================
int main (int argc, char* argv[])
{
    TT_Fetcher fetcher;
    fetcher.parsePatchLibrary();
    
    TT_Augmenter augmenter(&fetcher);
    augmenter.setMode(Mode::NOISE);
    augmenter.fetchSpectralData();
    augmenter.augmentSpectralTags();
    augmenter.fetchTemporalData();
    augmenter.augmentTemporalTags();
    
    TT_Formatter formatter (&augmenter);
    formatter.formatSpectralData();
    formatter.scrambleSpectralData();
    formatter.formatTemporalData();
    formatter.scrambleTemporalData();
    
    ParameterData spectralData = formatter.getSpectralData();
    ParameterData temporalData = formatter.getTemporalData();
    
    TT_Spectral spectralModel;
    spectralModel.construct();
    spectralModel.train(spectralData.data, spectralData.labels);

    TT_Temporal temporalModel;
    temporalModel.construct();
    temporalModel.train(temporalData.data, temporalData.labels);
    
    return 0;
}
