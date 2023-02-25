/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AMSYNCAudioProcessor::AMSYNCAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    amp_Sine_WaveL = std::make_unique<Sine_Wave>();
    amp_Sine_WaveR = std::make_unique<Sine_Wave>();
    fract_delayLineL = std::make_unique<Fract_delay_line>();
    fract_delayLineR = std::make_unique<Fract_delay_line>();
    mixGain_Delay_L = std::make_unique<Gain_Block>();
    mixGain_Delay_R = std::make_unique<Gain_Block>();
    mixGain_AmpMod_L = std::make_unique<Gain_Block>();
    mixGain_AmpMod_R = std::make_unique<Gain_Block>();
    wet_mixGain_AmpMod_L = std::make_unique<Gain_Block>();
    wet_mixGain_AmpMod_R = std::make_unique<Gain_Block>();
    wet_mixGain_Delay_L = std::make_unique<Gain_Block>();
    wet_mixGain_Delay_R = std::make_unique<Gain_Block>();
    paramInterpolDelayL = treeState.getRawParameterValue(floatParams[0]);
    paramInterpolDelayR = treeState.getRawParameterValue(floatParams[1]);
    paramInterpolAmpModL = treeState.getRawParameterValue(floatParams[2]);
    paramInterpolAmpModR = treeState.getRawParameterValue(floatParams[3]);
    paramMixDelayL = treeState.getRawParameterValue(floatParams[4]);
    paramMixDelayR = treeState.getRawParameterValue(floatParams[5]);
    paramMixAmpModL = treeState.getRawParameterValue(floatParams[6]);
    paramMixAmpModR = treeState.getRawParameterValue(floatParams[7]);
    
    
    paramDelayTimeL_1 = treeState.getRawParameterValue(choiceParams[0]);
    paramDelayTimeR_1 = treeState.getRawParameterValue(choiceParams[1]);
    paramAmpModFreqL_1 = treeState.getRawParameterValue(choiceParams[2]);
    paramAmpModFreqR_1= treeState.getRawParameterValue(choiceParams[3]);
    paramDelayTimeL_2 = treeState.getRawParameterValue(choiceParams[4]);
    paramDelayTimeR_2 = treeState.getRawParameterValue(choiceParams[5]);
    paramAmpModFreqL_2 = treeState.getRawParameterValue(choiceParams[6]);
    paramAmpModFreqR_2 = treeState.getRawParameterValue(choiceParams[7]);
    
    
}

AMSYNCAudioProcessor::~AMSYNCAudioProcessor()
{
}
juce::AudioProcessorValueTreeState::ParameterLayout
AMSYNCAudioProcessor::createParameterLayout()
{
    // create parameters
    // you could also use a array with strings and add them in a for loop
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    for (auto params_ : floatParams)
    {
        auto pParam = (std::make_unique<juce::AudioParameterFloat>(params_,params_,0.0,1.0,0.5));
        params.push_back(std::move(pParam));
    }
    for (auto params_ : choiceParams)
    {
        auto pParam = (std::make_unique<juce::AudioParameterChoice>(params_,params_,Times,2));
        params.push_back(std::move(pParam));
    }
    
    
    
    return {params.begin(),params.end()};
}

//==============================================================================
const juce::String AMSYNCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AMSYNCAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AMSYNCAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AMSYNCAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AMSYNCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AMSYNCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AMSYNCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AMSYNCAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AMSYNCAudioProcessor::getProgramName (int index)
{
    return {};
}

void AMSYNCAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AMSYNCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;
    m_sampleRate = sampleRate;
    amp_Sine_WaveL->prepare(spec);
    amp_Sine_WaveR->prepare(spec);
    fract_delayLineL->prepare(spec);
    fract_delayLineR->prepare(spec);
    l_Dry_MixAmpModblock.setSize(1, samplesPerBlock);
    l_Wet_MixAmpModblock.setSize(1, samplesPerBlock);
    r_Dry_MixAmpModblock.setSize(1, samplesPerBlock);
    r_Wet_MixAmpModblock.setSize(1, samplesPerBlock);
    l_Dry_MixDelayblock.setSize(1, samplesPerBlock);
    l_Wet_MixDelayblock.setSize(1, samplesPerBlock);
    r_Dry_MixDelayblock.setSize(1, samplesPerBlock);
    r_Wet_MixDelayblock.setSize(1, samplesPerBlock);
    mixGain_Delay_L->prepare();
    mixGain_Delay_R->prepare();
    mixGain_AmpMod_L->prepare();
    mixGain_AmpMod_R->prepare();
    wet_mixGain_Delay_L->prepare();
    wet_mixGain_Delay_R->prepare();
    wet_mixGain_AmpMod_L->prepare();
    wet_mixGain_AmpMod_R->prepare();
}

void AMSYNCAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AMSYNCAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AMSYNCAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto inputblock = juce::dsp::AudioBlock<float>(buffer);
    auto inputblockL = inputblock.getSingleChannelBlock(0);
    auto inputblockR = inputblock.getSingleChannelBlock(1);
    
    // Mix Blocks for dry and wet for amplitude modulation
    auto L_block_AmpMod = juce::dsp::AudioBlock<float>(l_Wet_MixAmpModblock);
    auto R_block_AmpMod = juce::dsp::AudioBlock<float>(r_Wet_MixAmpModblock);
    
    auto L_AmpMod_Mix = juce::dsp::AudioBlock<float>(l_Dry_MixAmpModblock);
    auto R_AmpMod_Mix = juce::dsp::AudioBlock<float>(r_Dry_MixAmpModblock);
    
    // Mix Blocks for dry and wet for delays
    
    auto L_block_Delay = juce::dsp::AudioBlock<float>(l_Wet_MixDelayblock);
    auto R_block_Delay = juce::dsp::AudioBlock<float>(r_Wet_MixDelayblock);
    
    auto L_Delay_Mix = juce::dsp::AudioBlock<float>(l_Dry_MixDelayblock);
    auto R_Delay_Mix = juce::dsp::AudioBlock<float>(r_Dry_MixDelayblock);
    
    //get Playhead you can call this only in audiothread
    playhead = this->getPlayHead();
    //get BPM
    m_bpm = *(playhead->getPosition()->getBpm());
    if(playhead->getPosition()->getIsPlaying()){
        //get ppq_pos for phase
        if(isPlaying == true) {
            float ppq_pos = std::fmod(*(playhead->getPosition()->getPpqPosition()),4);
            amp_Sine_WaveL->phase = ppq_pos * 0.25 * 512.0;
            amp_Sine_WaveR->phase = ppq_pos * 0.25 * 512.0;
            isPlaying = false;
        }
        //Copy One Channel into buffers
        L_block_AmpMod.copyFrom(inputblockL);
        L_AmpMod_Mix.copyFrom(inputblockL);
        
        //set Freq Left Channel
        float frequencyL_1 = bpm_to_herz(m_bpm,int(*paramAmpModFreqL_1));
        float frequencyL_2 = bpm_to_herz(m_bpm,int(*paramAmpModFreqL_2));
        float resFreq = linear_interp(frequencyL_1, frequencyL_2, *paramInterpolAmpModL);
        amp_Sine_WaveL->setFreq(resFreq);
        //Process One Channel
        amp_Sine_WaveL->process(L_block_AmpMod);
        //Mix AmpMod
        mixGain_AmpMod_L->setGain(1.0 - *paramMixAmpModL);
        mixGain_AmpMod_L->process(L_AmpMod_Mix);
        wet_mixGain_AmpMod_L->setGain(*paramMixAmpModL);
        wet_mixGain_AmpMod_L->process(L_block_AmpMod);
        // Add blocks together
        L_AmpMod_Mix.add(L_block_AmpMod);
        // Get Pointers
        auto* dest_amp_mod_L  = inputblockL.getChannelPointer (0);
        auto* input_amp_mod_L  = L_AmpMod_Mix.getChannelPointer (0);
        // Write samples
        for(int i = 0; i < getBlockSize(); i++) {
            dest_amp_mod_L[i] = input_amp_mod_L[i];
        }
        //Copy into buffers
        L_block_Delay.copyFrom(inputblockL);
        L_Delay_Mix.copyFrom(inputblockL);
        //get Delay
        float delayL_1 = bpm_to_samps(m_bpm, int(*paramDelayTimeL_1), m_sampleRate);
        float delayL_2 = bpm_to_samps(m_bpm, int(*paramDelayTimeL_2), m_sampleRate);
        float resDelay = linear_interp(delayL_1, delayL_2, *paramInterpolDelayL);
        fract_delayLineL->setDelay(resDelay);
        // Process
        fract_delayLineL->process(L_block_Delay);
        // Mix delay
        mixGain_Delay_L->setGain(1.0 - *paramMixDelayL);
        mixGain_Delay_L->process(L_Delay_Mix);
        wet_mixGain_Delay_L->setGain(*paramMixDelayL);
        wet_mixGain_Delay_L->process(L_block_Delay);
        // Add buffers
        L_Delay_Mix.add(L_block_Delay);
        //Get pointers
        auto* dest_delay_L  = inputblockL.getChannelPointer (0);
        auto* input_delay_L  = L_Delay_Mix.getChannelPointer (0);
        // Write samples
        for(int i = 0; i < getBlockSize(); i++) {
            dest_delay_L[i] = input_delay_L[i];
        }
        
        
        
        //Copy One Channel into buffers
        R_block_AmpMod.copyFrom(inputblockR);
        R_AmpMod_Mix.copyFrom(inputblockR);
        //set Freq Right Channel
        float frequencyR_1 = bpm_to_herz(m_bpm,int(*paramAmpModFreqR_1));
        float frequencyR_2 = bpm_to_herz(m_bpm,int(*paramAmpModFreqR_2));
        float resFreqR = linear_interp(frequencyR_1, frequencyR_2, *paramInterpolAmpModR);
        amp_Sine_WaveR->setFreq(resFreqR);
        //Process One Channel
        amp_Sine_WaveR->process(R_block_AmpMod);
        
        
        //Mix AmpMod
        mixGain_AmpMod_R->setGain(1.0 - *paramMixAmpModR);
        mixGain_AmpMod_R->process(R_AmpMod_Mix);
        
        wet_mixGain_AmpMod_R->setGain(*paramMixAmpModR);
        wet_mixGain_AmpMod_R->process(R_block_AmpMod);
        // Add blocks together
        R_AmpMod_Mix.add(R_block_AmpMod);
        // Get Pointers
        auto* dest_ampMod_R  = inputblockR.getChannelPointer(0);
        auto* input_ampMod_R  = R_AmpMod_Mix.getChannelPointer(0);
        // Write Channels
        for(int i = 0; i <getBlockSize(); i++) {
            dest_ampMod_R[i] = input_ampMod_R[i];
        }
        
        
        
        //Copy into buffers
        R_block_Delay.copyFrom(inputblockR);
        R_Delay_Mix.copyFrom(inputblockR);
        //get Delay
        float delayR_1 = bpm_to_samps(m_bpm, int(*paramDelayTimeR_1), m_sampleRate);
        float delayR_2 = bpm_to_samps(m_bpm, int(*paramDelayTimeR_2), m_sampleRate);
        float resDelayR = linear_interp(delayR_1, delayR_2, *paramInterpolDelayR);
        fract_delayLineR->setDelay(resDelayR);
        // Process
        fract_delayLineR->process(R_block_Delay);
        // Mix delay
        mixGain_Delay_R->setGain(1.0 - *paramMixDelayR);
        mixGain_Delay_R->process(R_Delay_Mix);
        wet_mixGain_Delay_R->setGain(*paramMixDelayR);
        wet_mixGain_Delay_R->process(R_block_Delay);
        // Add buffers
        R_Delay_Mix.add(R_block_Delay);
        //Get pointers
        auto* dest_delay_R  = inputblockR.getChannelPointer (0);
        auto* input_delay_R  = R_Delay_Mix.getChannelPointer (0);
        // Write samples
        for(int i = 0; i < getBlockSize(); i++) {
            dest_delay_R[i] = input_delay_R[i];
        }
    } else {
        isPlaying = true;
    }
}

//==============================================================================
bool AMSYNCAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AMSYNCAudioProcessor::createEditor()
{
    return  new AMSYNCAudioProcessorEditor (*this);
}

//==============================================================================
void AMSYNCAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    treeState.state.writeToStream(mos);
}

void AMSYNCAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
                              if(tree.isValid() )
                              {
                                  treeState.replaceState(tree);
                              }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AMSYNCAudioProcessor();
}
