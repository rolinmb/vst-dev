#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::Array<float> getSimpleNoise(int numSamples)
{
    juce::Random r = juce::Random::getSystemRandom();
    juce::Array<float> noise;
    // Generate noise
    for (int s = 0; s < numSamples; s++)
    {
        noise.add((r.nextFloat() - 0.5)*2);
    }

    return noise;
}

static juce::Array<float> getWhiteNoise(int numSamples)
{
    juce::Array<float> noise;

    float z0 = 0;
    float z1 = 0;
    bool generate = false;

    float mu = 0; // center (0)
    float sigma = 1; // spread -1 <-> 1 

    float output = 0;
    float u1 = 0;
    float u2 = 0;

    juce::Random r = juce::Random::getSystemRandom();
    r.setSeed(juce::Time::getCurrentTime().getMilliseconds());
    const float epsilon = std::numeric_limits<float>::min();
    // Generate noise
    for (int s = 0; s < numSamples; s++)
    {
        generate = !generate;
        // Box Muller random number sampling for white noise generation
        if (!generate)
        {
            output = z1 * sigma + mu;
        }
        else
        {
            do
            {
                u1 = r.nextFloat();
                u2 = r.nextFloat();
            } while (u1 <= epsilon);
            
            z0 = sqrtf(-2.0 * logf(u1)) * cosf(2*float(juce::double_Pi) * u2);
            z1 = sqrtf(-2.0 * logf(u1)) * sinf(2*float(juce::double_Pi) * u2);
            output = z0 * sigma + mu; 
        }

        jassert(output == output);
        jassert(output > -50 && output < 50);
        noise.add(output);
    }
    return noise;
}

BiccrushAudioProcessor::BiccrushAudioProcessor()
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

BiccrushAudioProcessor::~BiccrushAudioProcessor()
{
}

//==============================================================================
const juce::String BiccrushAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BiccrushAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BiccrushAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BiccrushAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BiccrushAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BiccrushAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BiccrushAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BiccrushAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BiccrushAudioProcessor::getProgramName (int index)
{
    return {};
}

void BiccrushAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BiccrushAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BiccrushAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BiccrushAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BiccrushAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();
    float noiseAmt = -120 + 120 * (*treeState.getRawParameterValue("NOISE") / 100);
    float bitDepth = *treeState.getRawParameterValue("BITS");
    int rateDivide = (int) *treeState.getRawParameterValue("RATE");

    noiseAmt = juce::jlimit<float>(-120, 0 , noiseAmt);
    noiseAmt = juce::Decibels::decibelsToGain(noiseAmt);
    // Safety check here since some hosts will change buffer size without calling prepToPlay()
    if (noiseBuffer.getNumSamples() != numSamples)
    {
        noiseBuffer.setSize(2, numSamples, false, true, true); // clear/instantiates noiseBuffer
        currentOutputBuffer.setSize(2, numSamples, false, true, true); // clears/instantiates currentOutputBuffer
    }
    // Copy for processing
    currentOutputBuffer.copyFrom(0, 0, buffer.getReadPointer(0), numSamples);
    if (buffer.getNumChannels() > 1)
    {
        currentOutputBuffer.copyFrom(1, 0, buffer.getReadPointer(1), numSamples);
    }
    // Building the noise
    noiseBuffer.clear();
    juce::Array<float> noise = getWhiteNoise(numSamples);
    noiseAmt = juce::jlimit<float>(0, 1, noiseAmt); // range binding
    juce::FloatVectorOperations::multiply(noise.getRawDataPointer(), noiseAmt, numSamples);
    //Add the noise (in stereo)
    juce::FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples);
    juce::FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noise.getRawDataPointer(), numSamples);

    // Multiply mode, so if no signal no noise
    // juce::FloatVectorOperations::multiply(noiseBuffer.getWritePointer(0), currentOutputBuffer.getWritePointer(0), numSamples);
    // juce::FloatVectorOperations::multiply(noiseBuffer.getWritePointer(1), currentOutputBuffer.getWritePointer(1), numSamples);
    
    // Add noise into incoming/input buffer
    currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(0), numSamples);
    currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(1), numSamples);

    // Resampling when needed
    for (int chan = 0; chan < currentOutputBuffer.getNumChannels(); chan++)
    {
        float* data = currentOutputBuffer.getWritePointer(chan);

        for (int i = 0; i < numSamples; i++)
        {
            // Reduce bit depth
            float totalQLevels = powf(2, bitDepth);
            float val = data[i];
            float remainder = fmodf(val, 1/totalQLevels);
            // Quantize
            data[i] = val - remainder;
            if (rateDivide > 1)
            {
                if (i%rateDivide != 0)
                {
                    data[i] = data[i - i%rateDivide];
                }
            }
        }
    }
    // Copy to the output buffer
    buffer.copyFrom(0, 0, currentOutputBuffer, 0, 0, numSamples);
    buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);

}

//==============================================================================
bool BiccrushAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BiccrushAudioProcessor::createEditor()
{
    return new BiccrushAudioProcessorEditor (*this);
}

//==============================================================================
void BiccrushAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BiccrushAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BiccrushAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout BiccrushAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto noiseParams = std::make_unique<juce::AudioParameterFloat>("NOISE", "Noise", juce::NormalisableRange(0.f, 100.f, 0.1f), 0.f);
    auto bitParams = std::make_unique<juce::AudioParameterFloat>("BITS", "Bits", juce::NormalisableRange(2.0f, 32.0f, 1.0f), 32.0f);
    auto rateParams = std::make_unique<juce::AudioParameterFloat>("RATE", "Rate", juce::NormalisableRange(1.0f, 50.0f, 1.0f), 1.0f);
    params.push_back(std::move(noiseParams));
    params.push_back(std::move(bitParams));
    params.push_back(std::move(rateParams));
    return { params.begin(), params.end() };
}