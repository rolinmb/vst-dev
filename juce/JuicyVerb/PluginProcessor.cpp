#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuicyVerbAudioProcessor::JuicyVerbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, &undoManager, "Parameters", createParams())
#endif
{
}

JuicyVerbAudioProcessor::~JuicyVerbAudioProcessor()
{
}

//==============================================================================
const juce::String JuicyVerbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuicyVerbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuicyVerbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuicyVerbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuicyVerbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuicyVerbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuicyVerbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuicyVerbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JuicyVerbAudioProcessor::getProgramName (int index)
{
    return {};
}

void JuicyVerbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JuicyVerbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    reverb.prepare(spec);
}

void JuicyVerbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuicyVerbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JuicyVerbAudioProcessor::updateReverbSettings()
{
    params.roomSize = *apvts.getRawParameterValue("SIZE");
    params.damping = *apvts.getRawParameterValue("DAMP");
    params.width = *apvts.getRawParameterValue("WIDTH");
    params.wetLevel = *apvts.getRawParameterValue("MIX");
    params.dryLevel = 1.0f - *apvts.getRawParameterValue("MIX");
    params.freezeMode = *apvts.getRawParameterValue("FREEZE");

    reverb.setParameters(params);
}

void JuicyVerbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    updateReverbSettings();
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);
    reverb.process(ctx);
}

//==============================================================================
bool JuicyVerbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JuicyVerbAudioProcessor::createEditor()
{
    return new JuicyVerbAudioProcessorEditor (*this, undoManager);
}

//==============================================================================
void JuicyVerbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void JuicyVerbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuicyVerbAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout JuicyVerbAudioProcessor::createParams()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    const auto range = juce::NormalisableRange<float>{ 0.0f, 1.0f, 0.0001f, 1.0f };
    const auto defaultValue = 0.5f;

    auto convertToPercent = [](float value, int /*maximumStringLength*/)
    {
        value *= 100.0f;

        if (value < 10.0f)
            return juce::String(value, 2) + " %";
        else if (value < 100.0f)
            return juce::String(value, 1) + " %";
        else
            return juce::String(value, 0) + " %";
    };

    auto convertFromPercent = [](const juce::String& string)
    {
        return string.getFloatValue() * 0.01f;
    };

    // push back parameters to params vector
    layout.add(std::make_unique<juce::AudioParameterFloat>("SIZE", "Size", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterFloat>("DAMP", "Damp", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterBool>("FREEZE", "Freeze", false));

    /*
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto sizeParams = std::make_unique<juce::AudioParameterFloat>("SIZE", "Size", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent);

    auto dampParams = std::make_unique<juce::AudioParameterFloat>("DAMP", "Damp", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent);

    auto widthParams = std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent);

    auto mixParams = std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", range, defaultValue,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        convertToPercent, convertFromPercent
    );

    auto freezeParams = std::make_unique<juce::AudioParameterBool>("FREEZE", "Freeze", false);

    params.push_back(std::move(sizeParams));
    params.push_back(std::move(dampParams));
    params.push_back(std::move(widthParams));
    params.push_back(std::move(mixParams));
    params.push_back(std::move(freezeParams));
    return { params.begin(), params.end() };
    */

    return layout;
}

juce::AudioProcessorValueTreeState& JuicyVerbAudioProcessor::getPluginState()
{
    return apvts;
}