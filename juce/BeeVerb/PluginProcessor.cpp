#include "PluginProcessor.h"
#include "PluginEditor.h"

BeeVerbAudioProcessor::BeeVerbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, &undoManager, "Parameters", createGuiParams())
#endif
{
    auto storeFloatParam = [&apvts = this->apvts](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(paramID));
        jassert(param != nullptr);
    };

    storeFloatParam(size, "SIZE");
    storeFloatParam(damp, "DAMP");
    storeFloatParam(width, "WIDTH");
    storeFloatParam(mix, "MIX");

    auto storeBoolParam = [&apvts = this->apvts](auto& param, const auto& paramID)
    {
        param = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(paramID));
        jassert(param != nullptr);
    };

    storeBoolParam(freeze, "FREEZE");
}

BeeVerbAudioProcessor::~BeeVerbAudioProcessor()
{
}

//==============================================================================
const juce::String BeeVerbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BeeVerbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BeeVerbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BeeVerbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BeeVerbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BeeVerbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BeeVerbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BeeVerbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BeeVerbAudioProcessor::getProgramName (int index)
{
    return {};
}

void BeeVerbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BeeVerbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());
    reverb.prepare(spec);
}

void BeeVerbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BeeVerbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BeeVerbAudioProcessor::updateReverbSettings()
{
    rvParams.roomSize = size->get();
    rvParams.damping = damp->get();
    rvParams.width = width->get();
    rvParams.wetLevel = mix->get();
    rvParams.dryLevel = 1.0f - mix->get();
    rvParams.freezeMode = freeze->get();
    /*
    // Other method
    rvParams.roomSize = *apvts.getRawParameterValue("SIZE");
    rvParams.damping = *apvts.getRawParameterValue("DAMP");
    rvParams.width = *apvts.getRawParameterValue("WIDTH");
    rvParams.wetLevel = *apvts.getRawParameterValue("MIX");
    rvParams.dryLevel = 1.0f - *apvts.getRawParameterValue("MIX");
    rvParams.freezeMode = *apvts.getRawParameterValue("FREEZE");
    */
    // set juce::dsp::Reverb object with rvParams
    reverb.setParameters(rvParams);
}

void BeeVerbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    // defined above
    updateReverbSettings();
    // using juce::dsp::Reverb object
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> ctx (block);
    reverb.process(ctx);
}

bool BeeVerbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BeeVerbAudioProcessor::createEditor()
{
    return new BeeVerbAudioProcessorEditor (*this, undoManager);
}

void BeeVerbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void BeeVerbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BeeVerbAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout BeeVerbAudioProcessor::createGuiParams()
{
    //std::vector<std::unique_ptr<juce::RangedAudioParameter>> guiParams;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    const auto range = juce::NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 1.0f);
    const auto defaultVal = 0.5f;
    
    auto convertToPercent = [](float value, int maximumStringLength)
    {
        value *= 100.0f;

        if (value < 10.0f)
        {
            return juce::String(value, 2) + "%";
        }
        else if (value < 100.0f)
        {
            return juce::String(value, 1) + "%";
        }
        else
        {
            return juce::String(value, 0) + "%";
        }
    };

    auto convertFromPercent = [](const juce::String& string)
    {
        return string.getFloatValue() * 0.01f;
    };
    layout.add(std::make_unique<juce::AudioParameterFloat>("SIZE", "Size", range, defaultVal,
                                                           juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                           convertToPercent, convertFromPercent));


    layout.add(std::make_unique<juce::AudioParameterFloat>("DAMP", "Damp", range, defaultVal,
                                                           juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                           convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", range, defaultVal,
                                                          juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                          convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterFloat>("WET", "Wet", range, defaultVal, juce::String(),
                                                          juce::AudioProcessorParameter::genericParameter,
                                                          convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterFloat>("DRY", "Dry", range, defaultVal,
                                                          juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                          convertToPercent, convertFromPercent));

    layout.add(std::make_unique<juce::AudioParameterBool>("FREEZE", "Freeze", false));

    return layout;

    /*
    // Other method of initializing apvts object
    auto sizeParams = std::make_unique<juce::AudioParameterFloat>("SIZE", "Size", range, defaultVal,
                                                                  juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                                  convertToPercent, convertFromPercent);

    auto dampParams = std::make_unique<juce::AudioParameterFloat>("DAMP", "Damp", range, defaultVal,
                                                                  juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                                  convertToPercent, convertFromPercent);

    auto widthParams = std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", range, defaultVal,
                                                                   juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                                   convertToPercent, convertFromPercent);

    auto wetParams = std::make_unique<juce::AudioParameterFloat>("WET", "Wet", range, defaultVal,juce::String(),
                                                                 juce::AudioProcessorParameter::genericParameter,
                                                                 convertToPercent, convertFromPercent);

    auto dryParams = std::make_unique<juce::AudioParameterFloat>("DRY", "Dry", range, defaultVal,
                                                                 juce::String(), juce::AudioProcessorParameter::genericParameter,
                                                                 convertToPercent, convertFromPercent);

    auto freezeParams = std::make_unique<juce::AudioParameterBool>("FREEZE", "Freeze", false);

    guiParams.push_back(std::move(sizeParams));
    guiParams.push_back(std::move(dampParams));
    guiParams.push_back(std::move(widthParams));
    guiParams.push_back(std::move(wetParams));
    guiParams.push_back(std::move(dryParams));
    guiParams.push_back(std::move(freezeParams));
    return { guiParams.begin(), guiParams.end() };
    */
}

juce::AudioProcessorValueTreeState& BeeVerbAudioProcessor::getPluginState()
{
    return apvts;
}