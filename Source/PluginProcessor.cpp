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
    lowPassCoeff = 0.0f;
    sampleCountDown = 0;
}

void NoiseGateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool NoiseGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
        && ! layouts.getMainInputChannelSet().isDisabled();
}

void NoiseGateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto mainInputOutput = getBusBuffer (buffer, true, 0);
    auto sideChainInput  = getBusBuffer (buffer, true, 1);

    auto alphaCopy = alpha->get();
    auto thresholdCopy = threshold->get();

    for (auto j = 0; j < buffer.getNumSamples(); ++j)
    {
      auto mixedSamples = 0.0f;

      for (auto i = 0; i < sideChainInput.getNumChannels(); ++i)
          mixedSamples += sideChainInput.getReadPointer (i) [j];

      mixedSamples /= static_cast<float> (sideChainInput.getNumChannels());
      lowPassCoeff = (alphaCopy * lowPassCoeff) + ((1.0f - alphaCopy) * mixedSamples);

      if (lowPassCoeff >= thresholdCopy)
          sampleCountDown = (int) getSampleRate();

      for (auto i = 0; i < mainInputOutput.getNumChannels(); ++i)
          *mainInputOutput.getWritePointer (i, j) = sampleCountDown > 0 ? *mainInputOutput.getReadPointer (i, j) : 0.0f;

      if (sampleCountDown > 0)
          --sampleCountDown;
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
