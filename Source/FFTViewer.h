/*
  ==============================================================================

    FFTViewer.h
    Created: 6 Jun 2020 5:55:52pm
    Author:  Sylvain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

//==============================================================================
/*
*/
class FFTViewer : public Component, public Timer
{
public:
    FFTViewer();
    ~FFTViewer();

    //==============================================================================

    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================

    void setCircularBuffer(GraphicsCircularBuffer<float>* cb) { circularBuffer = cb; }
    void goForDrawing(bool go);

    //==============================================================================

    void timerCallback() override;

private:
    Label title, numSamplesTitleLabel;
    //==============================================================================
    GraphicsCircularBuffer<float>* circularBuffer;
    bool isDrawing = false;
    int label = 0;
    //==============================================================================
    void computeFFT();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTViewer)
};
