/*
  ==============================================================================

    Combobox_Custom.h
    Created: 7 Feb 2023 4:17:39pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
namespace juce {
class Zen_GreenComboBox : public juce::LookAndFeel_V4
{
public:
    Zen_GreenComboBox(){};
    ~Zen_GreenComboBox() {};
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& box) override
    {
        auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
        Rectangle<int> boxBounds (0, 0, width, height);

        g.setColour (juce::Colours::grey);
        g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

        g.setColour (juce::Colours::lightgreen);
        g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

        Rectangle<int> arrowZone (width - 30, 0, 20, height);
        Path path;
        path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
        path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
        path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

        g.setColour (juce::Colours::black);
        g.strokePath (path, PathStrokeType (2.0f));

        
        
        
        
    }
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            const bool isSeparator, const bool isActive,
                            const bool isHighlighted, const bool isTicked,
                            const bool hasSubMenu, const String& text,
                            const String& shortcutKeyText,
                            const Drawable* icon, const Colour* const textColourToUse) override
{
    juce::Colour color = juce::Colours::lightgreen;
if (isSeparator)
{
auto r  = area.reduced (5, 0);
r.removeFromTop (roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

g.setColour (color.withAlpha (0.3f));
g.fillRect (r.removeFromTop (1));
}
else
{
auto r  = area.reduced (1);

if (isHighlighted && isActive)
{
g.setColour (juce::Colours::white.withAlpha(0.3f));
g.fillRect (r);

g.setColour (juce::Colours::black);
}
else
{
    g.setColour (color.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
}

r.reduce (jmin (5, area.getWidth() / 20), 0);

auto font = getPopupMenuFont();

auto maxFontHeight = (float) r.getHeight() / 1.3f;

if (font.getHeight() > maxFontHeight)
font.setHeight (maxFontHeight);

g.setFont (font);

auto iconArea = r.removeFromLeft (roundToInt (maxFontHeight)).toFloat();

if (icon != nullptr)
{
icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
r.removeFromLeft (roundToInt (maxFontHeight * 0.5f));
}
else if (isTicked)
{
/*
auto tick = getTickShape (1.0f);
g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
*/
    Rectangle<int> arrowZone (area.getWidth() - 30, 0, 20,area.getHeight());
    Path path;
    path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
    path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

    g.setColour (juce::Colours::black);
    g.strokePath (path, PathStrokeType (2.0f));
}

if (hasSubMenu)
{
auto arrowH = 0.6f * getPopupMenuFont().getAscent();

auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
auto halfH = static_cast<float> (r.getCentreY());

Path path;
path.startNewSubPath (x, halfH - arrowH * 0.5f);
path.lineTo (x + arrowH * 0.6f, halfH);
path.lineTo (x, halfH + arrowH * 0.5f);

g.strokePath (path, PathStrokeType (2.0f));
}

r.removeFromRight (3);
g.drawFittedText (text, r, Justification::centredLeft, 1);

if (shortcutKeyText.isNotEmpty())
{
auto f2 = font;
f2.setHeight (f2.getHeight() * 0.75f);
f2.setHorizontalScale (0.95f);
g.setFont (f2);

g.drawText (shortcutKeyText, r, Justification::centredRight, true);
}
}
}
    
};
class Zen_RedComboBox : public juce::LookAndFeel_V4
{
public:
    Zen_RedComboBox(){};
    ~Zen_RedComboBox() {};
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& box) override
    {
        auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
        Rectangle<int> boxBounds (0, 0, width, height);

        g.setColour (juce::Colours::grey);
        g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

        g.setColour (juce::Colours::red);
        g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

        Rectangle<int> arrowZone (width - 30, 0, 20, height);
        Path path;
        path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
        path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
        path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

        g.setColour (juce::Colours::black);
        g.strokePath (path, PathStrokeType (2.0f));
        
    }
    
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            const bool isSeparator, const bool isActive,
                            const bool isHighlighted, const bool isTicked,
                            const bool hasSubMenu, const String& text,
                            const String& shortcutKeyText,
                            const Drawable* icon, const Colour* const textColourToUse) override
{
    juce::Colour color = juce::Colours::red;
if (isSeparator)
{
auto r  = area.reduced (5, 0);
r.removeFromTop (roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

g.setColour (color.withAlpha (0.3f));
g.fillRect (r.removeFromTop (1));
}
else
{
auto r  = area.reduced (1);

if (isHighlighted && isActive)
{
g.setColour (juce::Colours::white.withAlpha(0.3f));
g.fillRect (r);

g.setColour (juce::Colours::black);
}
else
{
    g.setColour (color.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
}

r.reduce (jmin (5, area.getWidth() / 20), 0);

auto font = getPopupMenuFont();

auto maxFontHeight = (float) r.getHeight() / 1.3f;

if (font.getHeight() > maxFontHeight)
font.setHeight (maxFontHeight);

g.setFont (font);

auto iconArea = r.removeFromLeft (roundToInt (maxFontHeight)).toFloat();

if (icon != nullptr)
{
icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
r.removeFromLeft (roundToInt (maxFontHeight * 0.5f));
}
else if (isTicked)
{
/*
auto tick = getTickShape (1.0f);
g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
*/
    Rectangle<int> arrowZone (area.getWidth() - 30, 0, 20,area.getHeight());
    Path path;
    path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
    path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

    g.setColour (juce::Colours::black);
    g.strokePath (path, PathStrokeType (2.0f));
}

if (hasSubMenu)
{
auto arrowH = 0.6f * getPopupMenuFont().getAscent();

auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
auto halfH = static_cast<float> (r.getCentreY());

Path path;
path.startNewSubPath (x, halfH - arrowH * 0.5f);
path.lineTo (x + arrowH * 0.6f, halfH);
path.lineTo (x, halfH + arrowH * 0.5f);

g.strokePath (path, PathStrokeType (2.0f));
}

r.removeFromRight (3);
g.drawFittedText (text, r, Justification::centredLeft, 1);

if (shortcutKeyText.isNotEmpty())
{
auto f2 = font;
f2.setHeight (f2.getHeight() * 0.75f);
f2.setHorizontalScale (0.95f);
g.setFont (f2);

g.drawText (shortcutKeyText, r, Justification::centredRight, true);
}
}
}

};
};
