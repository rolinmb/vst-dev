#include "PluginProcessor.h"
#include "PluginEditor.h"

ArtifactorAudioProcessor::ArtifactorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
}

ArtifactorAudioProcessor::~ArtifactorAudioProcessor()
{
}

const juce::String ArtifactorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ArtifactorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ArtifactorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ArtifactorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ArtifactorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ArtifactorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ArtifactorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ArtifactorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ArtifactorAudioProcessor::getProgramName (int index)
{
    return {};
}

void ArtifactorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void ArtifactorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ArtifactorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArtifactorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ArtifactorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    // Cleanup the buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
    // Processing below
    auto& gainVal = *apvts.getRawParameterValue("GAIN");
    auto& rangeVal = *apvts.getRawParameterValue("RANGE");
    auto& blendVal = *apvts.getRawParameterValue("BLEND");
    auto& volumeVal = *apvts.getRawParameterValue("VOLUME");
    int fuzzType = (int) *apvts.getRawParameterValue("FUZZ TYPE");
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        // Iterate through each sample in buffer
        for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            float cleanOut = *channelData;
            switch (fuzzType)
            {
                case 0:
                    *channelData += (gainVal * rangeVal) * 10.0; // Simple fuzz; double signal and multiply by factor using driveVal
                    *channelData = (((*channelData * blendVal)   // blend multiplier for wet signal
                        + (cleanOut * (1.f / blendVal)))         // add back clean signal with inverse of blend multiplier
                        / 2)                                     // average of sum of blended and cleanblend signals
                        * volumeVal;                             // adjust final signal by volume multiplier
                    break;
                case 1:
                    *channelData *= rangeVal;
                    *channelData = (((((*channelData / fabs(*channelData))
                        * (1.f - expf((gainVal * powf(*channelData, 2)) / fabs(*channelData))))    // exponential distortion curve to get wet signal
                        * blendVal)                                                                           // multiply wet signal by blend multiplier
                        + (cleanOut * (1.f / blendVal)))                                                      // add back clean signal with inverse of blend multiplier
                        / 2)                                                                                  // average of sum of blended an cleanblend signals
                        * volumeVal;                                                                          // adjust final signal by volume multiplier
                    break;
                case 2:
                    *channelData *= (gainVal * rangeVal);
                    *channelData = ((((2.f / juce::MathConstants<float>::pi) * atan(*channelData)  // distortion curve w/ asymptotes at +/- 1
                        * blendVal)                                                                // multiply wet signal by blend multiplier
                        + (cleanOut * (1.f / blendVal)))                                           // add back clean signal with inverse of blend multiplier
                        / 2)                                                                       // average of sum of blended an cleanblend signals
                        * volumeVal;                                                               // adjust final signal by volume multiplier
                    break;
                default:
                    *channelData *= 1.11f;
                    break;
            }
            // Clipping after fuzz processing decided
            if (*channelData > 0.8)
            {
                *channelData = 0.8;
            }
            if (*channelData < -0.8)
            {
                *channelData = -0.8;
            }
            // Increment address of write pointer as we iterate through (s = 0; s < numSamples; ++s)
            channelData++;
        }
    }
}

bool ArtifactorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ArtifactorAudioProcessor::createEditor()
{
    return new ArtifactorAudioProcessorEditor (*this);
}

void ArtifactorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ArtifactorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ArtifactorAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout ArtifactorAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain",
        juce::NormalisableRange<float>(0.f, 1.5f, 0.01f), 1.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("RANGE", "Range",
        juce::NormalisableRange<float>(0.f, 3000.f, 1.f), 1.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("BLEND", "Blend",
        juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 1.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume",
        juce::NormalisableRange<float>(0.f, 3.f, 0.01f), 1.f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>("FUZZ TYPE", "Fuzz Type",
        juce::StringArray({ "Regular Fuzz", "Exponential Fuzz", "Arctan Fuzz" }), 0));

    return { params.begin(), params.end() };
}

juce::AudioProcessorValueTreeState& ArtifactorAudioProcessor::getPluginState()
{
    return apvts;
}