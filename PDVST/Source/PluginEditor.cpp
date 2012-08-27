

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JucePdVstAudioProcessorEditor::JucePdVstAudioProcessorEditor (JucePdVstAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter)

{
    setSize (600, 400);

    ownerFlt = ownerFilter;
    
    browserButton = new TextButton("Open PD File");
    browserButton->setBounds(20,20, 100, 30);
    browserButton->addListener(this);
    addAndMakeVisible(browserButton);
    browserOn = false;
    
    resetButton = new TextButton("Refresh PD Patch");
    resetButton->setBounds(20, 70, 100, 30);
    resetButton->addListener(this);
    addAndMakeVisible(resetButton);
    
    
    File directory =  File::getSpecialLocation(File::userDesktopDirectory);
    int flags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    filter = new WildcardFileFilter("*.pd", "", "pd");
    browser = new FileBrowserComponent(flags,directory,filter,NULL);
    addAndMakeVisible(browser);
    browser->setTopLeftPosition(140, 20);
    browser->setSize(307, 298);
    browser->addListener(this);
    
    // QUICKLY CHANGED WITHOUT CHECKING
    browser->setVisible(false);
}

JucePdVstAudioProcessorEditor::~JucePdVstAudioProcessorEditor()
{
    
    delete browser;
    delete filter;
    deleteAllChildren();
    //delete browserButton;
    //delete currentF;
    
    
}

void JucePdVstAudioProcessorEditor::makeBrowser()
{
    
    browserOn = !browserOn;
    browser->setVisible(browserOn);
  
    
}

//==============================================================================
void JucePdVstAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("PDVST",
                      0, 0, getWidth(), getHeight(),
                      Justification::centred, 1);
}





void JucePdVstAudioProcessorEditor::selectionChanged()
{
    JucePdVstAudioProcessor*  thisPros = getProcessor();


}

void JucePdVstAudioProcessorEditor::fileClicked(const juce::File &file, const juce::MouseEvent &e)
{
    JucePdVstAudioProcessor*  thisPros = getProcessor();
   // thisPros->openNewPDPatch(file);

}

void JucePdVstAudioProcessorEditor::fileDoubleClicked(const juce::File &file)
{
    JucePdVstAudioProcessor*  thisPros = getProcessor();
    thisPros->openNewPDPatch(file);
    browser->setVisible(false);
    currentF = &file;
    
    
    getProcessor()->updateHostDisplay();
}

void JucePdVstAudioProcessorEditor::browserRootChanged(const juce::File &newRoot)
{
    JucePdVstAudioProcessor*  thisPros = getProcessor();

}




void JucePdVstAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    JucePdVstAudioProcessor*  thisPros = getProcessor();

    if(button == resetButton)
    {
        //thisPros->resetPDPatch();
        if(thisPros->PDAttached)
        {
            thisPros->openNewPDPatch(*currentF);
        }
    }
    if(button == browserButton)
    {
        makeBrowser();
        
    }
}












