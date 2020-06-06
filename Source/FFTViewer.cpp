/*
  ==============================================================================

    FFTViewer.cpp
    Created: 6 Jun 2020 5:55:52pm
    Author:  Sylvain

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FFTViewer.h"

//==============================================================================
FFTViewer::FFTViewer()
{
    addAndMakeVisible(title);
    title.setText("FFT", dontSendNotification);
    title.setColour(Label::textColourId, Colours::orange);
    title.setJustificationType(Justification::left);

}

FFTViewer::~FFTViewer()
{
}

void FFTViewer::paint (Graphics& g)
{
    Rectangle<int> area(getLocalBounds());
    title.setBounds(area.removeFromTop(30));
}

void FFTViewer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
