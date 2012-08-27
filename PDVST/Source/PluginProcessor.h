/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_363C2C3D__
#define __PLUGINPROCESSOR_H_363C2C3D__

#include "JuceHeader.h"
#include "ZenGarden.h"
#include <fstream.h>
#include <string.h>
#include <iostream>

#define NUMPARAMS 20

//==============================================================================
/**
*/
class JucePdVstAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JucePdVstAudioProcessor();
    ~JucePdVstAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    
    AudioPlayHead *playHead;

    //==============================================================================
    int getNumPrograms();
    
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    void openNewPDPatch(const File &file);
    int getPDParams(const File &file);
    void resetPDPatch();
    
    bool twitch;

    enum Parameters
    {
        gainParam = 0,
        totalNumParams
    };
    StringArray *paramStringList;
    bool PDAttached;
    float samRate;
    int bufBlockSize;
    

private:
    
    PdContext *context;
    PdGraph *graph;
    float gain;
    bool paramSwitch;
    bool play;
    
    String fileNameStr, fileDirStr;
    
    AudioSampleBuffer *secBuf;
    
    StringArray pdParameterList;
    int pdParameterNum;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JucePdVstAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_363C2C3D__
