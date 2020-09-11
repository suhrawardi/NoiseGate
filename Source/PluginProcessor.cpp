/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoiseGateAudioProcessor::NoiseGateAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       .withInput  ("SideChain", juce::AudioChannelSet::stereo(), true)
                       )
{
  addParameter (threshold = new juce::AudioParameterFloat ("threshold", "Threshold", 0.0f, 1.0f, 0.5f));
  addParameter (threshold = new juce::AudioParameterFloat ("alpha", "Alpha", 0.0f, 1.0f, 0.8f));
}

NoiseGateAudioProcessor::~NoiseGateAudioProcessor()
{
}

//==============================================================================
const juce::String NoiseGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoiseGateAudioProcessor::acceptsMidi() const
{
    return false;
}

bool NoiseGateAudioProcessor::producesMidi() const
{
    return false;
}

bool NoiseGateAudioProcessor::isMidiEffect() const
{
    return false;
}

double NoiseGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoiseGateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoiseGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoiseGateAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NoiseGateAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoiseGateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NoiseGateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NoiseGateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool NoiseGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    return true;
}

void NoiseGateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
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
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool NoiseGateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoiseGateAudioProcessor::createEditor()
{
    return new NoiseGateAudioProcessorEditor (*this);
}

//==============================================================================
void NoiseGateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NoiseGateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGateAudioProcessor();
}
