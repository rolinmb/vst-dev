/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuicySynthAudioProcessor::JuicySynthAudioProcessor()
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
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

JuicySynthAudioProcessor::~JuicySynthAudioProcessor()
{
}

//==============================================================================
const juce::String JuicySynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuicySynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuicySynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuicySynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuicySynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuicySynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuicySynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuicySynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JuicySynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void JuicySynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JuicySynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void JuicySynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuicySynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JuicySynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    {
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                auto& oscWaveChoice = *apvts.getRawParameterValue("WAVEFORM");

                auto& fmFreq = *apvts.getRawParameterValue("FM FREQUENCY");
                auto& fmDepth = *apvts.getRawParameterValue("FM DEPTH");

                auto& atk = *apvts.getRawParameterValue("ATTACK");
                auto& dcy = *apvts.getRawParameterValue("DECAY");
                auto& stn = *apvts.getRawParameterValue("SUSTAIN");
                auto& rel = *apvts.getRawParameterValue("RELEASE");

                auto& fAtk = *apvts.getRawParameterValue("FILTER ATTACK");
                auto& fDcy = *apvts.getRawParameterValue("FILTER DECAY");
                auto& fStn = *apvts.getRawParameterValue("FILTER SUSTAIN");
                auto& fRel = *apvts.getRawParameterValue("FILTER RELEASE");

                auto& filterType = *apvts.getRawParameterValue("FILTER TYPE");
                auto& cutoff = *apvts.getRawParameterValue("FILTER CUTOFF FREQUENCY");
                auto& resonance = *apvts.getRawParameterValue("FILTER RESONANCE");

                voice->getOscillator().setWaveType(oscWaveChoice);
                voice->getOscillator().updateFm(fmFreq, fmDepth);
                // Don't necessarily need .load() because of Atomic wrapper
                voice->getAdsr().update(atk.load(), dcy.load(), stn.load(), rel.load());
                voice->getFilterAdsr().update(fAtk.load(), fDcy.load(), fStn.load(), fRel.load());
                voice->updateFilter(filterType, cutoff, resonance);
            }
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool JuicySynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JuicySynthAudioProcessor::createEditor()
{
    return new JuicySynthAudioProcessorEditor (*this);
}

//==============================================================================
void JuicySynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JuicySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuicySynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout JuicySynthAudioProcessor::createParams()
{


    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterChoice>("WAVEFORM", "Waveform", juce::StringArray{ "Sine", "Saw", "Square" }, 0)); // ComboBox to switch oscillator waveform

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FM FREQUENCY", "FM Freq.", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FM DEPTH", "FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "A", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "D", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "S", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "R", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER ATTACK", "Filter A", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER DECAY", "Filter D", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER SUSTAIN", "Filter S", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER RELEASE", "Filter R", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTER TYPE", "Filter Type", juce::StringArray{ "Low-Pass", "Band-Pass", "High-Pass" }, 0)); // ComboBox to switch filter types
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER CUTOFF FREQUENCY", "Filter Freq", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER RESONANCE", "Filter Resonance", juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f }, 1.0f));
    
    return { params.begin(), params.end() };
}