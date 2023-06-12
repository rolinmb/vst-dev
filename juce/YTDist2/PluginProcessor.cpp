#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
YTDist2AudioProcessor::YTDist2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState(*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
    treeState.state = juce::ValueTree("savedParams");
}

YTDist2AudioProcessor::~YTDist2AudioProcessor()
{
}
juce::AudioProcessorValueTreeState::ParameterLayout YTDist2AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto driveParam = std::make_unique<juce::AudioParameterFloat>("Drive", "Drive", juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 1.f);
    auto rangeParam = std::make_unique<juce::AudioParameterFloat>("Range", "Range", juce::NormalisableRange<float>(0.f, 3000.f, 1.f), 1.f);
    auto blendParam = std::make_unique<juce::AudioParameterFloat>("Blend", "Blend", juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 1.f);
    auto volumeParam = std::make_unique<juce::AudioParameterFloat>("Volume", "Volume", juce::NormalisableRange<float>(0.f, 3.f, 0.01f), 1.f);

    params.push_back(std::move(driveParam));
    params.push_back(std::move(rangeParam));
    params.push_back(std::move(blendParam));
    params.push_back(std::move(volumeParam));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String YTDist2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool YTDist2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool YTDist2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool YTDist2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double YTDist2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int YTDist2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int YTDist2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void YTDist2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String YTDist2AudioProcessor::getProgramName (int index)
{
    return {};
}

void YTDist2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void YTDist2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void YTDist2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool YTDist2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void YTDist2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float driveVal = *treeState.getRawParameterValue("Drive");
    float rangeVal = *treeState.getRawParameterValue("Range");
    float blendVal = *treeState.getRawParameterValue("Blend");
    float volumeVal = *treeState.getRawParameterValue("Volume");
    if (driveVal == mDrive)
    {
        buffer.applyGain(driveVal);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), mDrive, driveVal);
        mDrive = driveVal;
    }

    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int s = 0; s < buffer.getNumSamples(); s++)
        {
            float clean = *channelData;
            auto sliderDriveVal = treeState.getRawParameterValue("Drive");
            auto sliderRangeVal = treeState.getRawParameterValue("Range");
            auto sliderBlendVal = treeState.getRawParameterValue("Blend");
            auto sliderVolumeVal = treeState.getRawParameterValue("Volume");
            
            *channelData *= (sliderDriveVal * sliderRangeVal); // dereference channelData to process on floating point values

            *channelData = ((((2.f / 3.14159265f) * atan(*channelData) // distortion curve with asymptotes at +/- 1
                            * sliderBlendVal)                   // adjust distortion signal by blend
                            + (clean * (1.f / sliderBlendVal))) // add back clean signal with inverse blend ratio
                            / 2)                          // average of sum of both signals (blended)
                            * sliderVolumeVal;                  // adjust signal by volume multiplier to finish

            channelData++; // increment address in buffer to process
        }
    }
    */
}

//==============================================================================
bool YTDist2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* YTDist2AudioProcessor::createEditor()
{
    return new YTDist2AudioProcessorEditor (*this);
}

//==============================================================================
void YTDist2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void YTDist2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(treeState.state.getType()))
        {
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new YTDist2AudioProcessor();
}
