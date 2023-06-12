#pragma once

#include <JuceHeader.h>

//==============================================================================
class BeeVerbAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    BeeVerbAudioProcessor();
    ~BeeVerbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createGuiParams();
    juce::AudioProcessorValueTreeState& getPluginState();

private:
    juce::AudioProcessorValueTreeState apvts;

    juce::AudioParameterFloat* size { nullptr };
    juce::AudioParameterFloat* damp { nullptr };
    juce::AudioParameterFloat* width { nullptr };
    juce::AudioParameterFloat* mix { nullptr };
    juce::AudioParameterBool* freeze { nullptr };

    void updateReverbSettings();

    juce::dsp::Reverb::Parameters rvParams;
    juce::dsp::Reverb reverb;

    juce::UndoManager undoManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BeeVerbAudioProcessor)
};
