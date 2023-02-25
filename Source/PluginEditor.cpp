/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "bpm_to_hz.h"
#include "bpm_to_samps.h"
#include "BlockInterpol.h"
//==============================================================================
AMSYNCAudioProcessorEditor::AMSYNCAudioProcessorEditor (AMSYNCAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),

Interpol_Delay_Left_Slider("<- Interpolation of Delay Time Left Channel ->"),
Interpol_AmpModFreq_Left_Slider("<- Interpolation of Amplitude Modulation Left Channel ->"),
Mix_Delay_Left_Slider("Mix of Delay Left Channel"),
Mix_Amp_Mod_Left_Slider("Mix of Amplitude Modulation Left Channel"),
Interpol_Delay_Right_Slider("<- Interpolation of Delay Time Right Channel ->"),
Interpol_AmpModFreq_Right_Slider("<- Interpolation of Amplitude Modulation Right Channel ->"),
Mix_Delay_Right_Slider(" Mix of Delay Right Channel"),
Mix_Amp_Mod_Right_Slider(" Mix of Amplitude Modulation Right Channel"),

Interpol_Delay_Left_Attach(audioProcessor.treeState,floatParams[0],Interpol_Delay_Left_Slider),
Interpol_Delay_Right_Attach(audioProcessor.treeState, floatParams[1], Interpol_Delay_Right_Slider),
Interpol_AmpModFreq_Left_Attach(audioProcessor.treeState, floatParams[2], Interpol_AmpModFreq_Left_Slider),
Interpol_AmpModFreq_Right_Attach(audioProcessor.treeState,floatParams[3],
    Interpol_AmpModFreq_Right_Slider),
Mix_Delay_Left_Attach(audioProcessor.treeState, floatParams[4], Mix_Delay_Left_Slider),
Mix_Delay_Right_Attach(audioProcessor.treeState, floatParams[5], Mix_Delay_Right_Slider),
Mix_Amp_Mod_Left_Attach(audioProcessor.treeState, floatParams[6], Mix_Amp_Mod_Left_Slider),
Mix_Amp_Mod_Right_Attach(audioProcessor.treeState, floatParams[7], Mix_Amp_Mod_Right_Slider),
DelayTimeLeft1_Attach(audioProcessor.treeState,choiceParams[0],DelayTimeLeft1),
DelayTimeRight1_Attach(audioProcessor.treeState, choiceParams[1], DelayTimeRight1),
AmpModLeft1_Attach(audioProcessor.treeState, choiceParams[2], AmpModLeft1),
AmpModRight1_Attach(audioProcessor.treeState, choiceParams[3],AmpModRight1),
DelayTimeLeft2_Attach(audioProcessor.treeState, choiceParams[4], DelayTimeLeft2),
DelayTimeRight2_Attach(audioProcessor.treeState
                       , choiceParams[5], DelayTimeRight2),
AmpModLeft2_Attach(audioProcessor.treeState,choiceParams[6],AmpModLeft2),
AmpModRight2_Attach(audioProcessor.treeState, choiceParams[7], AmpModRight2)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //startTimerHz(33);
    setSize (600, 600);
    setResizable(true, true);
    setResizeLimits(600, 600,1200,1200);
    getConstrainer()->setFixedAspectRatio(1.2);
    tooltip.setMillisecondsBeforeTipAppears(1000);
    Interpol_Delay_Left_Slider.setStyles(Zen_Slider::ZenStyles::greenslider);
   
    
    Interpol_Delay_Left_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    
    float value1 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[0])));
    float value2 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[4])));
    float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[0]));
    delayInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    Interpol_Delay_Left_Slider.onValueChange = [this](){
        float value1 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[0])));
        float value2 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[4])));
        float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[0]));
        delayInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    };
    delayInterpolTextButton.attachToComponent(&Interpol_Delay_Left_Slider, true);
    addAndMakeVisible(Interpol_Delay_Left_Slider);
    delayInterpolTextButton.setColour(juce::Label::outlineColourId, juce::Colours::lightgreen);
  //  delayInterpolTextButton.backgroundColourId(juce::Colours::green);
    addAndMakeVisible(delayInterpolTextButton);
  
    float value3 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[2])));
    float value4 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[6])));
    float interpol2 = linear_interp(value3, value4, *audioProcessor.treeState.getRawParameterValue(floatParams[2]));
    ampModInterpolTextButton.setText(juce::String(interpol2),juce::dontSendNotification);
    
    Interpol_AmpModFreq_Left_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    Interpol_AmpModFreq_Left_Slider.onValueChange = [this](){
        float value1 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[2])));
        float value2 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[6])));
        float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[2]));
        ampModInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    };
    ampModInterpolTextButton.attachToComponent(&Interpol_AmpModFreq_Left_Slider, true);
    ampModInterpolTextButton.setColour(juce::Label::outlineColourId, juce::Colours::lightgreen);
    Interpol_AmpModFreq_Left_Slider.setStyles(Zen_Slider::ZenStyles::greenslider);
    addAndMakeVisible(Interpol_AmpModFreq_Left_Slider);
    addAndMakeVisible(ampModInterpolTextButton);
    
    
    ampModInterpolTextButtonR.attachToComponent(&Interpol_AmpModFreq_Right_Slider, true);
    float value5 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[3])));
    float value6 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[7])));
    float interpol3 = linear_interp(value5, value6, *audioProcessor.treeState.getRawParameterValue(floatParams[3]));
    ampModInterpolTextButtonR.setText(juce::String(interpol3),juce::dontSendNotification);
    ampModInterpolTextButtonR.setColour(juce::Label::outlineColourId, juce::Colours::red);
    
    Interpol_AmpModFreq_Right_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    
    Interpol_AmpModFreq_Right_Slider.setStyles(Zen_Slider::ZenStyles::redslider);
    Interpol_AmpModFreq_Right_Slider.onValueChange = [this](){
        float value5 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[3])));
        float value6 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[7])));
        float interpol3 = linear_interp(value5, value6, *audioProcessor.treeState.getRawParameterValue(floatParams[3]));
        ampModInterpolTextButtonR.setText(juce::String(interpol3),juce::dontSendNotification);
    };
    addAndMakeVisible(Interpol_AmpModFreq_Right_Slider);
    ampModInterpolTextButtonR.attachToComponent(&Interpol_AmpModFreq_Right_Slider, true);
    addAndMakeVisible(ampModInterpolTextButtonR);
    
    Interpol_Delay_Right_Slider.setStyles(Zen_Slider::ZenStyles::redslider);
    Interpol_Delay_Right_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    float value7 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[1])));
    float value8 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[5])));
    float interpol4 = linear_interp(value7, value8, *audioProcessor.treeState.getRawParameterValue(floatParams[1]));
    delayInterpolTextButtonR.setText(juce::String(interpol4),juce::dontSendNotification);
    delayInterpolTextButtonR.setColour(juce::Label::outlineColourId, juce::Colours::red);
    
    Interpol_Delay_Right_Slider.onValueChange = [this](){
        float value7 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[1])));
        float value8 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[5])));
        float interpol4 = linear_interp(value7, value8, *audioProcessor.treeState.getRawParameterValue(floatParams[1]));
        delayInterpolTextButtonR.setText(juce::String(interpol4),juce::dontSendNotification);
    };
    delayInterpolTextButtonR.attachToComponent(&Interpol_Delay_Right_Slider, true);
    
    addAndMakeVisible(Interpol_Delay_Right_Slider);
    addAndMakeVisible(delayInterpolTextButtonR);
    
    
    
    
    
    
    
    delayMixTextButton.attachToComponent(&Mix_Delay_Left_Slider, true);
    delayMixTextButton.setColour(juce::Label::outlineColourId, juce::Colours::lightgreen);
    delayMixTextButton.setText(juce::String(*audioProcessor.treeState.getRawParameterValue(floatParams[4])* 100.0) + " %",juce::dontSendNotification);
    Mix_Delay_Left_Slider.setStyles(Zen_Slider::ZenStyles::greenslider);
    Mix_Delay_Left_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    Mix_Delay_Left_Slider.onValueChange = [this](){
        delayMixTextButton.setText(juce::String(Mix_Delay_Left_Slider.getValue() * 100.0) + " %", juce::dontSendNotification);
    };
    addAndMakeVisible(Mix_Delay_Left_Slider);
    addAndMakeVisible(delayMixTextButton);
    
    
    
    delayMixTextButtonR.attachToComponent(&Mix_Delay_Right_Slider, true);
    delayMixTextButtonR.setText(juce::String(*audioProcessor.treeState.getRawParameterValue(floatParams[5])* 100.0) + " %",juce::dontSendNotification);
    delayMixTextButtonR.setColour(juce::Label::outlineColourId, juce::Colours::red);
    Mix_Delay_Right_Slider.setStyles(Zen_Slider::ZenStyles::redslider);
    Mix_Delay_Right_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    Mix_Delay_Right_Slider.onValueChange = [this](){
        delayMixTextButtonR.setText(juce::String(Mix_Delay_Right_Slider.getValue() * 100.0) + " %", juce::dontSendNotification);
    };
    addAndMakeVisible(Mix_Delay_Right_Slider);
    addAndMakeVisible(delayMixTextButton);
    
    ampModMixTextButton.setText(juce::String(*audioProcessor.treeState.getRawParameterValue(floatParams[6])* 100.0) + " %",juce::dontSendNotification);
    ampModMixTextButton.attachToComponent(&Mix_Amp_Mod_Left_Slider, true);
    ampModMixTextButton.setColour(juce::Label::outlineColourId, juce::Colours::lightgreen);
   
    Mix_Amp_Mod_Left_Slider.setStyles(Zen_Slider::ZenStyles::greenslider);
    Mix_Amp_Mod_Left_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    Mix_Amp_Mod_Left_Slider.onValueChange = [this](){
        ampModMixTextButton.setText(juce::String(Mix_Amp_Mod_Left_Slider.getValue() * 100.0) + " %", juce::dontSendNotification);
    };
    addAndMakeVisible(Mix_Amp_Mod_Left_Slider);
    addAndMakeVisible(ampModMixTextButton);
    
    
    
    
    ampModMixTextButtonR.attachToComponent(&Mix_Amp_Mod_Right_Slider, true);
    ampModMixTextButtonR.setColour(juce::Label::outlineColourId, juce::Colours::red);
    ampModMixTextButtonR.setText(juce::String(*audioProcessor.treeState.getRawParameterValue(floatParams[7]) * 100.0) + " %",juce::dontSendNotification);
    Mix_Amp_Mod_Right_Slider.setStyles(Zen_Slider::ZenStyles::redslider);
    Mix_Amp_Mod_Right_Slider.setTextBoxStyle(Zen_Slider::NoTextBox, true, 0, 0);
    Mix_Amp_Mod_Right_Slider.onValueChange = [this](){
        ampModMixTextButtonR.setText(juce::String(Mix_Amp_Mod_Right_Slider.getValue() * 100.0) + " %", juce::dontSendNotification);
    };
    
    
    addAndMakeVisible(Mix_Amp_Mod_Right_Slider);
    addAndMakeVisible(ampModInterpolTextButtonR);
    DelayTimeLeft1.addItem("1/4", 1);
    DelayTimeLeft1.addItem("1/8", 2);
    DelayTimeLeft1.addItem("1/16", 3);
    DelayTimeLeft1.addItem("1/32", 4);
    DelayTimeLeft1.addItem("1/64", 5);
    DelayTimeLeft1.addItem("1/128", 6);
    DelayTimeLeft1.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[0]))+1);
    
    
    
    
    DelayTimeLeft1.setStyles(Zen_Combobox::ZenStyles::greenbox);
    addAndMakeVisible(DelayTimeLeft1);
   
    DelayTimeLeft1.onChange = [this]() {
        float value1 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[0])));
        float value2 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[4])));
        float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[0]));
        delayInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    };
    
    
    DelayTimeLeft2.addItem("1/4", 1);
    DelayTimeLeft2.addItem("1/8", 2);
    DelayTimeLeft2.addItem("1/16", 3);
    DelayTimeLeft2.addItem("1/32", 4);
    DelayTimeLeft2.addItem("1/64", 5);
    DelayTimeLeft2.addItem("1/128", 6);
    DelayTimeLeft2.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[4]))+1);
    DelayTimeLeft2.setStyles(Zen_Combobox::ZenStyles::greenbox);
    
    DelayTimeLeft2.onChange = [this]() {
        float value1 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[0])));
        float value2 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[4])));
        float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[0]));
        delayInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    };
    
    
    
    
    addAndMakeVisible(DelayTimeLeft2);
    
    AmpModLeft1.addItem("1/4", 1);
    AmpModLeft1.addItem("1/8", 2);
    AmpModLeft1.addItem("1/16", 3);
    AmpModLeft1.addItem("1/32", 4);
    AmpModLeft1.addItem("1/64", 5);
    AmpModLeft1.addItem("1/128", 6);
    AmpModLeft1.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[2]))+1);
    AmpModLeft1.setStyles(Zen_Combobox::ZenStyles::greenbox);
    
    AmpModLeft1.onChange = [this](){
        float value1 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[2])));
        float value2 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[6])));
        float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[2]));
        ampModInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    };
    
    
    addAndMakeVisible(AmpModLeft1);
   
    AmpModLeft2.addItem("1/4", 1);
    AmpModLeft2.addItem("1/8", 2);
    AmpModLeft2.addItem("1/16", 3);
    AmpModLeft2.addItem("1/32", 4);
    AmpModLeft2.addItem("1/64", 5);
    AmpModLeft2.addItem("1/128", 6);
    AmpModLeft2.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[6]))+1);
    AmpModLeft2.setStyles(Zen_Combobox::ZenStyles::greenbox);
    
    AmpModLeft2.onChange = [this](){
        float value1 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[2])));
        float value2 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[6])));
        float interpol = linear_interp(value1, value2, *audioProcessor.treeState.getRawParameterValue(floatParams[2]));
        ampModInterpolTextButton.setText(juce::String(interpol),juce::dontSendNotification);
    };
    
    
    
    addAndMakeVisible(AmpModLeft2);
    
    DelayTimeRight1.addItem("1/4", 1);
    DelayTimeRight1.addItem("1/8", 2);
    DelayTimeRight1.addItem("1/16", 3);
    DelayTimeRight1.addItem("1/32", 4);
    DelayTimeRight1.addItem("1/64", 5);
    DelayTimeRight1.addItem("1/128", 6);
    DelayTimeRight1.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[1]))+1);
    DelayTimeRight1.setStyles(Zen_Combobox::ZenStyles::redbox);
    
    DelayTimeRight1.onChange = [this](){
        float value7 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[1])));
        float value8 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[5])));
        float interpol4 = linear_interp(value7, value8, *audioProcessor.treeState.getRawParameterValue(floatParams[1]));
        delayInterpolTextButtonR.setText(juce::String(interpol4),juce::dontSendNotification);
    };
    
    
    
    
    addAndMakeVisible(DelayTimeRight1);
    
    DelayTimeRight2.addItem("1/4", 1);
    DelayTimeRight2.addItem("1/8", 2);
    DelayTimeRight2.addItem("1/16", 3);
    DelayTimeRight2.addItem("1/32", 4);
    DelayTimeRight2.addItem("1/64", 5);
    DelayTimeRight2.addItem("1/128", 6);
    DelayTimeRight2.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[5]))+1);
    DelayTimeRight2.setStyles(Zen_Combobox::ZenStyles::redbox);
    
    DelayTimeRight2.onChange = [this](){
        float value7 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[1])));
        float value8 = bpm_to_s(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[5])));
        float interpol4 = linear_interp(value7, value8, *audioProcessor.treeState.getRawParameterValue(floatParams[1]));
        delayInterpolTextButtonR.setText(juce::String(interpol4),juce::dontSendNotification);
    };
    
    
    addAndMakeVisible(DelayTimeRight2);
    
    AmpModRight1.addItem("1/4", 1);
    AmpModRight1.addItem("1/8", 2);
    AmpModRight1.addItem("1/16", 3);
    AmpModRight1.addItem("1/32", 4);
    AmpModRight1.addItem("1/64", 5);
    AmpModRight1.addItem("1/128", 6);
    AmpModRight1.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[3]))+1);
    AmpModRight1.setStyles(Zen_Combobox::ZenStyles::redbox);
    
    AmpModRight1.onChange = [this](){
        float value5 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[3])));
        float value6 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[7])));
        float interpol3 = linear_interp(value5, value6, *audioProcessor.treeState.getRawParameterValue(floatParams[3]));
        ampModInterpolTextButtonR.setText(juce::String(interpol3),juce::dontSendNotification);
    };
    
    
    
    
    
    
    
    addAndMakeVisible(AmpModRight1);
    
    AmpModRight2.addItem("1/4", 1);
    AmpModRight2.addItem("1/8", 2);
    AmpModRight2.addItem("1/16", 3);
    AmpModRight2.addItem("1/32", 4);
    AmpModRight2.addItem("1/64", 5);
    AmpModRight2.addItem("1/128", 6);
    AmpModRight2.setSelectedId(int(*audioProcessor.treeState.getRawParameterValue(choiceParams[7]))+1);
    AmpModRight2.setStyles(Zen_Combobox::ZenStyles::redbox);
    
    
    
    AmpModRight2.onChange = [this](){
        float value5 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[3])));
        float value6 = bpm_to_herz(audioProcessor.m_bpm, int(*audioProcessor.treeState.getRawParameterValue(choiceParams[7])));
        float interpol3 = linear_interp(value5, value6, *audioProcessor.treeState.getRawParameterValue(floatParams[3]));
        ampModInterpolTextButtonR.setText(juce::String(interpol3),juce::dontSendNotification);
    };
    addAndMakeVisible(AmpModRight2);
    
}

AMSYNCAudioProcessorEditor::~AMSYNCAudioProcessorEditor()
{
    
}

//==============================================================================
void AMSYNCAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

}

void AMSYNCAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int width = getLocalBounds().getWidth();
    const int height =  getLocalBounds().getHeight();
    const int widthx = width / 10;
    const int widthx_combo = widthx / 4;
    const int heighty = height / 10;
    const int sliderwidth = width / 2.0;
    const int sliderheight = height / 12;
    //Combobox
    AmpModLeft1.setBounds(widthx_combo,heighty, widthx * 1.2, heighty * 0.75);
    //Same Y with offset on X
    AmpModLeft2.setBounds(widthx * 8, heighty, widthx * 1.2, heighty * 0.75);
    
    //Slider
    Interpol_AmpModFreq_Left_Slider.setBounds(widthx * 3, heighty * 0.5, sliderwidth, sliderheight);
    //Same X with offset on Y
    Mix_Amp_Mod_Left_Slider.setBounds(widthx * 3, heighty * 1.5, sliderwidth, sliderheight);
    
    //Combobox
    DelayTimeLeft1.setBounds(widthx_combo,heighty * 3.5, widthx  * 1.2, heighty * 0.75);
    //Same Y with offset on X
    DelayTimeLeft2.setBounds(widthx * 8, heighty * 3.5, widthx * 1.2, heighty * 0.75);
    
    //Slider
    Interpol_Delay_Left_Slider.setBounds(widthx * 3, heighty * 2.5, sliderwidth, sliderheight);
    //Same X with offset on Y
    Mix_Delay_Left_Slider.setBounds(widthx * 3, heighty * 3.5, sliderwidth, sliderheight);
    
    //Combobox
    AmpModRight1.setBounds(widthx_combo,heighty * 5.5, widthx * 1.2, heighty * 0.75);
    //Same Y with offset on X
    AmpModRight2.setBounds(widthx * 8, heighty * 5.5, widthx * 1.2, heighty * 0.75);
    
    //Slider
    Interpol_AmpModFreq_Right_Slider.setBounds(widthx * 3, heighty * 5, sliderwidth, sliderheight);
    //Same X with offset on Y
    Mix_Amp_Mod_Right_Slider.setBounds(widthx * 3, heighty * 6, sliderwidth, sliderheight);
    
    //Combobox
    DelayTimeRight1.setBounds(widthx_combo,heighty * 7.5, widthx * 1.2, heighty * 0.75);
    //Same Y with offset on X
    DelayTimeRight2.setBounds(widthx * 8, heighty * 7.5, widthx * 1.2, heighty * 0.75);
    
    //Slider
    Interpol_Delay_Right_Slider.setBounds(widthx * 3, heighty * 7, sliderwidth, sliderheight);
    //Same X with offset on Y
    Mix_Delay_Right_Slider.setBounds(widthx * 3, heighty * 8, sliderwidth, sliderheight);
}
