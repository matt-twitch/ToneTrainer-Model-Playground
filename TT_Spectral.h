/*
  ==============================================================================

    TT_Spectral.h
    Created: 29 Feb 2024 3:31:16pm
    Author:  Matt Twitchen

  ==============================================================================
*/

#pragma once
#include "TT_Formatter.h"

using namespace tiny_dnn;

class TT_Spectral
{
public:
    
    void construct()
    {
        // Encoder
        nn << fully_connected_layer(paramDim, paramDim);
        nn << activation::leaky_relu();
        nn << recurrent_layer(lstm(paramDim, paramDim), paramDim);
        nn << activation::leaky_relu();
        nn << fully_connected_layer(paramDim, hiddenSize);
        nn << activation::leaky_relu();
        nn << fully_connected_layer(hiddenSize, latentDim);
        
        // Decoder
        nn << fully_connected_layer(latentDim, hiddenSize);
        nn << activation::leaky_relu();
        nn << fully_connected_layer(hiddenSize, paramDim);
        nn << activation::leaky_relu();
        nn << recurrent_layer(lstm(paramDim, paramDim), paramDim);
        nn << activation::leaky_relu();
        nn << fully_connected_layer(paramDim, paramDim);
        nn << activation::sigmoid();
    }
    
    void train(tensor_t data, tensor_t labels) // pass in training data as arguments
    {
        DBG("Training TT_Spectral ... ");
        
        divideData(data, labels);
        
        nn.weight_init(weight_init::xavier());
        nn.bias_init(weight_init::xavier());
        
        nn.fit<mse>(opt, trainLabels, trainData, batchSize, epochs, [](){},
                    [&](){
                            float loss = nn.get_loss<mse>(validateLabels, validateData);
                            DBG("loss = " << loss);
                            //if(loss > prevLoss)
                                //nn.stop_ongoing_training();
                            //else
                                //prevLoss = loss;
                        });
        
        
        nn.save("spectral-model");
        // construct graph from training inputs
        
        DBG("Training of TT_Spectral finished");
    }
    
    vec_t generate(vec_t input)
    {
        return nn.predict(input);
    }
    
private:
    
    void divideData(tensor_t data, tensor_t labels)
    {
        int trainThresh = data.size() * trainProp;
        int validateThresh = trainThresh + (data.size() * validateProp);
        
        jassert(validateThresh <= data.size());
        
        for(int i = 0 ; i < data.size() ; i++)
        {
            if(i < trainThresh) // training data
            {
                trainData.push_back(data[i]);
                trainLabels.push_back(labels[i]);
            } else if(i > trainThresh && i < validateThresh) // validation data
            {
                validateData.push_back(data[i]);
                validateLabels.push_back(labels[i]);
            }
        }
    }
    
    network<tiny_dnn::sequential> nn;
    core::backend_t backend_type = core::default_engine();
    int paramDim = 5;
    int hiddenSize = 3;
    int latentDim = 1;
    
    float trainProp = 0.8;
    float validateProp = 0.2;
    
    tensor_t trainData;
    tensor_t trainLabels;
    tensor_t validateData;
    tensor_t validateLabels;
    
    adam opt;
    size_t batchSize = 32;
    int epochs = 200;
    
    float prevLoss = 1000;
};
