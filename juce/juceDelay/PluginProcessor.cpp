#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceDelayAudioProcessor::JuceDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "Parameters", createParams())
#endif
{
}

JuceDelayAudioProcessor::~JuceDelayAudioProcessor()
{
}

//==============================================================================
const juce::String JuceDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuceDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuceDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JuceDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void JuceDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

// plugin instance initialization of parameters and memory allocations
void JuceDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mBuffer = (float**) std::malloc(getNumOutputChannels() * sizeof(float*));
    size_t size = (size_t) ((sampleRate*sizeof(float)) + 0.5);
    for (int ch = 0; ch < getNumOutputChannels(); ch++)
    {
        mBuffer[ch] = (float*) std::malloc(size);
        // reset values that buffer's addresses point to if not 0 for each channel(L+R)
        if (*mBuffer[ch])
        {
            memset(mBuffer[ch], 0, size);
        }
    }

}

// plugin instance destructor; used to deallocate mBuffer upon closing the plugin
void JuceDelayAudioProcessor::releaseResources()
{
    if (mBuffer)
    {
        for (int ch = 0; ch < getNumOutputChannels(); ch++)
        {
            std::free(mBuffer[ch]);
        }
        std::free(mBuffer);
        mBuffer = nullptr;
    } 

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void JuceDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    double sampleRate = getSampleRate();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int bufferSize = buffer.getNumSamples();

    // processing algorithm
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        int tempBufferPos = mBufferPos;
        for (int s = 0; s < bufferSize; s++)
        {
            mDelayInSamples = std::max<int>(1, (int)(*treeState.getRawParameterValue("DELAY") * sampleRate));
            float tempSample = *channelData;
            *channelData = mBuffer[channel][tempBufferPos];
            mBuffer[channel][tempBufferPos] = tempSample;
            tempBufferPos++;
            if (tempBufferPos >= mDelayInSamples)
            {
                tempBufferPos = 0;
            }
            channelData++;
        } 

    }

    while (mDelayInSamples && mBufferPos >= mDelayInSamples)
    {
        mBufferPos -= mDelayInSamples;
    } 
}

//==============================================================================
bool JuceDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JuceDelayAudioProcessor::createEditor()
{
    return new JuceDelayAudioProcessorEditor (*this);
}

//==============================================================================
void JuceDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JuceDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceDelayAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout JuceDelayAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto delayParams = std::make_unique<juce::AudioParameterFloat>("DELAY", "Delay", juce::NormalisableRange(0.f, 1.f, 0.1f), 0.f);
    params.push_back(std::move(delayParams));
    return { params.begin(), params.end() };
}