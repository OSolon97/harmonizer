/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HarmonizerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HarmonizerAudioProcessorEditor (HarmonizerAudioProcessor&);
    ~HarmonizerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HarmonizerAudioProcessor& processor;
	Slider gainSlider;
	Slider dryGainSlider;
	Slider intervalSlider;
	Label gainLabel;
	Label dryGainLabel;
	Label intervalLabel;

public:
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> drySliderValue;
	std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> intervalSliderValue;
	



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizerAudioProcessorEditor)
};
