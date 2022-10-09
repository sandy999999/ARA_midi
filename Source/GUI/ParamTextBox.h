/*
  ==============================================================================

    ParamTextBox.h
    Created: 1 Apr 2022 1:56:23pm
    Author:  Andreas Sandersen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
#include <JuceHeader.h>

class ParamTextBox : public juce::Label
{
public:
    static juce::String valueShownWithEditor;

    ParamTextBox();

    void resized() override;

    juce::TextEditor* createEditorComponent() override;

    void editorShown (juce::TextEditor* editor) override;

    void editorAboutToBeHidden (juce::TextEditor * ed) override;
};

