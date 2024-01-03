/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <rubberband/RubberBandStretcher.h>


//==============================================================================
HarmonizerAudioProcessor::HarmonizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
	: treeState(*this, nullptr, Identifier("PARAMETERS"),
		{
		std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, -60.0f, 0.0f, -15.0f),
		std::make_unique<AudioParameterFloat>(DRY_GAIN_ID, DRY_GAIN_NAME, -60.0f, 0.0f, -15.0f),
		std::make_unique<AudioParameterInt>(INTERVAL_ID, INTERVAL_NAME, -12, 12, 0),
		})
{
}


HarmonizerAudioProcessor::~HarmonizerAudioProcessor()
{
}

//==============================================================================
const String HarmonizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmonizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HarmonizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HarmonizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HarmonizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmonizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HarmonizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmonizerAudioProcessor::setCurrentProgram (int index)
{
}

const String HarmonizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void HarmonizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HarmonizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	const int numInputChannels = getTotalNumInputChannels();
	//const int dryBufferSize = samplesPerBlock;
	//dryBuffer.setSize(numInputChannels, dryBufferSize);
	
	pitchShifter = new RubberBand::RubberBandStretcher(sampleRate, numInputChannels);
	pitchShifter->setMaxProcessSize(samplesPerBlock);


}

void HarmonizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmonizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HarmonizerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

	auto sliderGainValue = treeState.getRawParameterValue(GAIN_ID);
	auto sliderDryGainValue = treeState.getRawParameterValue(DRY_GAIN_ID);
	auto intervalValue = treeState.getRawParameterValue(INTERVAL_ID);
	float pitchScale = pow(2, *intervalValue / 12);
	
	//for (int channel = 0; channel < totalNumInputChannels; ++channel)
	//{
		//dryBuffer.copyFrom(channel, 0, buffer.getWritePointer(channel), buffer.getNumSamples(), 1.0);
	//}

	dryBuffer.makeCopyOf(buffer);
	dryBuffer.applyGain(Decibels::decibelsToGain(*sliderDryGainValue));

	pitchShifter->setPitchScale(pitchScale);
	pitchShifter->process((const float* const*)buffer.getArrayOfWritePointers(), buffer.getNumSamples(), false);
	pitchShifter->retrieve((float *const *)buffer.getArrayOfWritePointers(), buffer.getNumSamples());

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		buffer.applyGain(Decibels::decibelsToGain(*sliderGainValue));
		buffer.addFrom(channel, 0, dryBuffer.getWritePointer(channel), dryBuffer.getNumSamples(), 1.0);
	}

}

//==============================================================================
bool HarmonizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HarmonizerAudioProcessor::createEditor()
{
    return new HarmonizerAudioProcessorEditor (*this);
}

//==============================================================================
void HarmonizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HarmonizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonizerAudioProcessor();
}
