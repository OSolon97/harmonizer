/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HarmonizerAudioProcessorEditor::HarmonizerAudioProcessorEditor (HarmonizerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	sliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, gainSlider);
	drySliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DRY_GAIN_ID, dryGainSlider);
	intervalSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, INTERVAL_ID, intervalSlider);

    setSize (400, 300);


	gainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	gainSlider.setRange(-60.0f, 0.0f);
	gainSlider.setValue(-15.0f);
	addAndMakeVisible(&gainSlider);

	gainLabel.setText("HARMONY GAIN", dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);
	addAndMakeVisible(&gainLabel);

	dryGainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	dryGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	dryGainSlider.setRange(-60.0f, 0.0f);
	dryGainSlider.setValue(-15.0f);
	addAndMakeVisible(&dryGainSlider);

	dryGainLabel.setText("GAIN", dontSendNotification);
	dryGainLabel.attachToComponent(&dryGainSlider, false);
	addAndMakeVisible(&dryGainLabel);
	
	intervalSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	intervalSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 15);
	intervalSlider.setRange(-12, 12, 1);
	intervalSlider.setValue(0);
	addAndMakeVisible(&intervalSlider);

	intervalLabel.setText("INTERVAL IN SEMITONES", dontSendNotification);
	intervalLabel.attachToComponent(&intervalSlider, false);
	addAndMakeVisible(&intervalLabel);
}

HarmonizerAudioProcessorEditor::~HarmonizerAudioProcessorEditor()
{
}

//==============================================================================
void HarmonizerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void HarmonizerAudioProcessorEditor::resized()
{
		dryGainSlider.setBounds(50, 150, 100, 100);
		gainSlider.setBounds(250, 150, 100, 100);
		intervalSlider.setBounds(25, 25, 350, 100);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
