/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Sine_Wave.h"
#include "bpm_to_hz.h"
#include "bpm_to_samps.h"
#include "Parameters.h"
#include "BlockInterpol.h"
#include "gain_block.h"
#include "Fract_Delay_Line.h"
#include "DDI.h"
//==============================================================================
/**
*/
class AMSYNCAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    AMSYNCAudioProcessor();
    ~AMSYNCAudioProcessor() override;

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
    juce::AudioProcessorValueTreeState treeState;
    float m_bpm = 0;
   
private:
    // create parameter layout
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::unique_ptr<Sine_Wave> amp_Sine_WaveL;
    std::unique_ptr<Sine_Wave> amp_Sine_WaveR;
    std::unique_ptr<DDI> fract_delayLineL;
    std::unique_ptr<DDI> fract_delayLineR;
    float m_sampleRate = -1;
    std::unique_ptr<Gain_Block> mixGain_Delay_L;
    std::unique_ptr<Gain_Block> mixGain_Delay_R;
    std::unique_ptr<Gain_Block> mixGain_AmpMod_L;
    std::unique_ptr<Gain_Block> mixGain_AmpMod_R;
    std::unique_ptr<Gain_Block> wet_mixGain_AmpMod_L;
    std::unique_ptr<Gain_Block> wet_mixGain_AmpMod_R;
    std::unique_ptr<Gain_Block> wet_mixGain_Delay_L;
    std::unique_ptr<Gain_Block> wet_mixGain_Delay_R;
    juce::AudioPlayHead* playhead;
   
    
    
    std::atomic<float>* paramInterpolDelayL = nullptr;
    std::atomic<float>* paramInterpolDelayR  = nullptr;
    std::atomic<float>* paramInterpolAmpModL  = nullptr;
    std::atomic<float>* paramInterpolAmpModR  = nullptr;
    std::atomic<float>* paramMixDelayL = nullptr;
    std::atomic<float>* paramMixDelayR  = nullptr;
    std::atomic<float>* paramMixAmpModL  = nullptr;
    std::atomic<float>* paramMixAmpModR  = nullptr;
    std::atomic<float>* paramDelayTimeL_1 = nullptr;
    std::atomic<float>* paramDelayTimeL_2 = nullptr;
    std::atomic<float>* paramAmpModFreqL_1 = nullptr;
    std::atomic<float>* paramAmpModFreqL_2 = nullptr;
    std::atomic<float>* paramDelayTimeR_1 = nullptr;
    std::atomic<float>* paramDelayTimeR_2 = nullptr;
    std::atomic<float>* paramAmpModFreqR_1 = nullptr;
    std::atomic<float>* paramAmpModFreqR_2 = nullptr;
    
    
    juce::AudioBuffer<float> l_Wet_MixAmpModblock;
    juce::AudioBuffer<float> r_Wet_MixAmpModblock;
    juce::AudioBuffer<float> l_Dry_MixAmpModblock;
    juce::AudioBuffer<float> r_Dry_MixAmpModblock;
    
    juce::AudioBuffer<float> l_Wet_MixDelayblock;
    juce::AudioBuffer<float> r_Wet_MixDelayblock;
    juce::AudioBuffer<float> l_Dry_MixDelayblock;
    juce::AudioBuffer<float> r_Dry_MixDelayblock;
    bool isPlaying = false;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMSYNCAudioProcessor)
};
