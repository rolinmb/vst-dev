#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
YoutubeDistortionAudioProcessor::YoutubeDistortionAudioProcessor()
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

YoutubeDistortionAudioProcessor::~YoutubeDistortionAudioProcessor()
{
}

//==============================================================================
const juce::String YoutubeDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool YoutubeDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool YoutubeDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool YoutubeDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double YoutubeDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int YoutubeDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int YoutubeDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void YoutubeDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String YoutubeDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void YoutubeDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void YoutubeDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void YoutubeDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool YoutubeDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void YoutubeDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // get param vals by dereferencing treeState pointer method return type
    float driveVal = *treeState.getRawParameterValue("DRIVE");
    float rangeVal = *treeState.getRawParameterValue("RANGE");
    float blendVal = *treeState.getRawParameterValue("BLEND");
    float volumeVal = *treeState.getRawParameterValue("VOLUME");
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        // iterate through each sample in buffer (each sample is value in range [-1.0, 1.0] )
        for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            float cleanOut = *channelData;

            *channelData *= driveVal * rangeVal;
            
            *channelData = ((((2.f / 3.141592653f) * atan(*channelData) // distortion curve w/ asymptotes at +/- 1
                            * blendVal)                                 // multiply distortion signal by blend multiplier
                            + (cleanOut * (1.f / blendVal)))            // add back clean signal with inverse of blend multiplier
                            / 2)                                        // average of sum of blended an clean signals
                            * volumeVal;                                // adjust final signal by volume multiplier

            // clip at [-0.8, 0.8] for fuzz/distortion effect
            if (*channelData > 0.8)
            {
                *channelData = 0.8;
            }
            else if (*channelData < -0.8)
            {
                *channelData = -0.8;
            }


            channelData++;
        }
    }
}

//==============================================================================
bool YoutubeDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* YoutubeDistortionAudioProcessor::createEditor()
{
    return new YoutubeDistortionAudioProcessorEditor (*this);
}

//==============================================================================
void YoutubeDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void YoutubeDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new YoutubeDistortionAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout YoutubeDistortionAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto driveParams = std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 1.f);
    auto rangeParams = std::make_unique<juce::AudioParameterFloat>("RANGE", "Range", juce::NormalisableRange<float>(0.f, 3000.f, 1.f), 1.f);
    auto blendParams = std::make_unique<juce::AudioParameterFloat>("BLEND", "Blend", juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 1.f);
    auto volumeParams = std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", juce::NormalisableRange<float>(0.f, 3.f, 0.01f), 1.f);
    params.push_back(std::move(driveParams));
    params.push_back(std::move(rangeParams));
    params.push_back(std::move(blendParams));
    params.push_back(std::move(volumeParams));
    return { params.begin(), params.end() };
}