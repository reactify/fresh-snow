

#include "PluginProcessor.h"
#include "PluginEditor.h"

extern "C"{
    void *callbackFunction(ZGCallbackFunction function, void *userData, void *ptr){
        switch (function){
            case ZG_PRINT_STD : break;
            case ZG_PRINT_ERR : break;
            default: break;
        }
        return NULL;
    }
};

JucePdVstAudioProcessor::JucePdVstAudioProcessor()
{
    play = false;
    paramSwitch = true;
    PDAttached = false;
    gain = 0.0f;
    pdParameterNum = 0;
    suspendProcessing(true);
    
// used to determine if host is playing
    setPlayHead(playHead);
    
// holds parameter names. default to n/a, pd patch receiver params extracted later.
    paramStringList = new StringArray();
    for(int i = 0; i < NUMPARAMS + 1; i++)
    {
        paramStringList->add("--n/a");
    }
}

JucePdVstAudioProcessor::~JucePdVstAudioProcessor()
{
// ensure a context/graph has been made before deletion
    if(PDAttached)
    {
        zg_context_delete(context);
    } else {
        
    }
    delete paramStringList;
      
}


void JucePdVstAudioProcessor::openNewPDPatch(const juce::File &f)
{
    
    play = false;
// this is a relatively expensive method so stop audiothread during operation
    suspendProcessing(true);
    if(PDAttached)
    {
        zg_graph_unattach(graph);
    }
    fileNameStr = f.getFileName();
    fileDirStr = f.getParentDirectory().getFullPathName() + "/";
    const char* fileName = fileNameStr.toUTF8();
    const char* fileDir  = fileDirStr.toUTF8();
// extracts pd params
    getPDParams(f);
// create context and graph
    context = zg_context_new(2,2,bufBlockSize,samRate,callbackFunction,NULL);
    graph = zg_context_new_graph_from_file(context,fileDir,fileName);
    zg_graph_attach(graph);
    PDAttached = true;
// resume audiothread
    suspendProcessing(false);
// tells host to update it's autogenerated slider names
    updateHostDisplay();
    play = true;

}

void JucePdVstAudioProcessor::resetPDPatch()
{
    // look at holding last patch parameter state
}

int JucePdVstAudioProcessor::getPDParams(const juce::File &f)
{
/**
 * reads the PD file into a string, tokenizing it by whitespace.
 * PD patch
*/
    FileInputStream *fIS = new FileInputStream(f);
    String pdPatchString = "";
    String result = "";
    StringArray sA;
    paramStringList->clear();
    pdPatchString = fIS->readString();
    int numTokens = sA.addTokens(pdPatchString, " ");
    for(int i = 0; i < numTokens; i++)
    {        
        if(sA.operator[](i).contains("#v"))
        {
            paramStringList->add(sA.operator[](i).dropLastCharacters(1));
        }
        pdParameterNum  = paramStringList->size();
    }
    for(int i = pdParameterNum; i < NUMPARAMS + 1; i++)
    {
        paramStringList->set(i, "--n/a");
    }
    
    delete fIS;
    return pdParameterNum;    
    
}

const String JucePdVstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int JucePdVstAudioProcessor::getNumParameters()
{
    return NUMPARAMS;
}

float JucePdVstAudioProcessor::getParameter (int index)
{
    switch(index)
    {
        case gainParam: return gain;
        default: return 0.0f;
    }
}

void JucePdVstAudioProcessor::setParameter (int index, float newValue)
{
    switch(index)
    {
       // case gainParam: gain = newValue; break;
        default: break;
    }

    
    if(play && PDAttached)
    {
        const char* pdParaName =  paramStringList->operator[](index).toUTF8();
        zg_context_send_messageV(context,pdParaName,0.0,"f",newValue);

    }
    
 
}

const String JucePdVstAudioProcessor::getParameterName (int index)
{
    
   return paramStringList->operator[](index).substring(2);
    
}

const String JucePdVstAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String JucePdVstAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String JucePdVstAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool JucePdVstAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool JucePdVstAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool JucePdVstAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool JucePdVstAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int JucePdVstAudioProcessor::getNumPrograms()
{
    return 0;
}

int JucePdVstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JucePdVstAudioProcessor::setCurrentProgram (int index)
{
}

const String JucePdVstAudioProcessor::getProgramName (int index)
{
    return String::empty;
} 
void JucePdVstAudioProcessor::changeProgramName (int index, const String& newName)
{
   
}

void JucePdVstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samRate = static_cast<float>(sampleRate);
    bufBlockSize = getBlockSize();
    secBuf = new AudioSampleBuffer(1,bufBlockSize*2);
}

void JucePdVstAudioProcessor::releaseResources()
{
  
}

void JucePdVstAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    playHead =  getPlayHead();
    AudioPlayHead::CurrentPositionInfo pos;
    playHead->getCurrentPosition(pos);
    pos.isPlaying ? play=true : play=false;

    if(!isSuspended())
    {
        if(play){
            
            int nSamp = buffer.getNumSamples();
    
            secBuf->copyFrom(0, 0, buffer, 0, 0, nSamp);
            secBuf->copyFrom(0, nSamp, buffer, 0, nSamp, nSamp);
            float* channelData = secBuf->getSampleData (0);
            
            zg_context_process(context,channelData,channelData);

            buffer.copyFrom(0, 0, *secBuf, 0, 0, nSamp);
            buffer.copyFrom(1, 0, *secBuf, 0, nSamp, nSamp);
            
        }
    }
    
    
}

bool JucePdVstAudioProcessor::hasEditor() const
{
    return true; 
}

AudioProcessorEditor* JucePdVstAudioProcessor::createEditor()
{
    return new JucePdVstAudioProcessorEditor (this);
}

void JucePdVstAudioProcessor::getStateInformation (MemoryBlock& destData)
{
  
}

void JucePdVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JucePdVstAudioProcessor();
}

/*
 //context = zg_context_new(0,1,512,44100.f,callbackFunction,NULL);
 //graph = zg_context_new_graph_from_file(context,"/Users/thomasrushmore/Desktop/","simple_osc.pd");
 // zg_graph_attach(graph);
 */

