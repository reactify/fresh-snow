/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINEDITOR_H_3A4DDF96__
#define __PLUGINEDITOR_H_3A4DDF96__

#include "JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class JucePdVstAudioProcessorEditor  : public AudioProcessorEditor,
public FileBrowserListener, public ButtonListener
{
public:
    JucePdVstAudioProcessorEditor (JucePdVstAudioProcessor* ownerFilter);
    ~JucePdVstAudioProcessorEditor();
    
    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
    void makeBrowser();
    /*
    void fileDoubleClicked(const juce::File f );
    void selectionChanged();
    void fileClicked (const juce::File f, const MouseEvent e);
    void browserRootChanged (const File f);
    */
    void selectionChanged();
    void fileClicked (const File& file, const MouseEvent& e);
    void fileDoubleClicked (const File& file);
    void browserRootChanged (const File& newRoot) ;
    
    /*
    void clicked();
     void paintButton (Graphics& g,
                              bool isMouseOverButton,
                       bool isButtonDown);
    void buttonStateChanged();
    void internalClickCallback (const ModifierKeys& modifiers);
    */
    void buttonClicked(Button* button);
    
    FileBrowserComponent *browser;
    WildcardFileFilter *filter;
    
    JucePdVstAudioProcessor* ownerFlt;
    TextButton* browserButton;
    TextButton* resetButton;
    juce::File const *currentF;
    
    bool browserOn;
      JucePdVstAudioProcessor* getProcessor() const
    
    {
        return static_cast<JucePdVstAudioProcessor*> (getAudioProcessor());
    }
    
};


#endif  // __PLUGINEDITOR_H_3A4DDF96__
