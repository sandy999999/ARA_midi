/*
  ==============================================================================

    ParamTextBox.cpp
    Created: 1 Apr 2022 1:56:23pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParamTextBox.h"

juce::String ParamTextBox::valueShownWithEditor = "";

ParamTextBox::ParamTextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (false, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
}

void ParamTextBox::resized()
{
    juce::Label::resized();
    setFont (getWidth() * 0.2f);
}

juce::TextEditor* ParamTextBox::createEditorComponent()
{
    auto* ed = juce::Label::createEditorComponent();

    ed->setJustification (juce::Justification::centred);
    ed->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
    ed->setInputRestrictions (5, "0123456789.");
    ed->setIndents (4, 0);

    return ed;
}

void ParamTextBox::editorShown (juce::TextEditor* ed)
{
    getParentComponent()->setMouseCursor (juce::MouseCursor::NoCursor);
    ed->clear();
    ed->setText (valueShownWithEditor);
}

void ParamTextBox::editorAboutToBeHidden (juce::TextEditor* /*ed*/)
{
    getParentComponent()->setMouseCursor (juce::MouseCursor::NormalCursor);
}
