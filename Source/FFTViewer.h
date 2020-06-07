/*
  ==============================================================================

    FFTViewer.h
    Created: 6 Jun 2020 5:55:52pm
    Author:  Sylvain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <chrono>
#include "CircularBuffer.h"
#include "kiss_fftr.h"

//==============================================================================
/*
*/
class FFTViewer : public Component, public Timer
{
public:
    FFTViewer();
    ~FFTViewer();

    void setSampleRate(double sr) { sampleRate = sr; }

    //==============================================================================

    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================

    void setCircularBuffer(GraphicsCircularBuffer<float>* cb) { circularBuffer = cb; }
    void goForDrawing(bool go);

    //==============================================================================

    void timerCallback() override;

private:
    Label title, numSamplesLabel, computeTimeLabel, mainFreqLabel;
    std::vector<Label*> freqLabels;
    std::vector<Label*> magLabels;
    std::vector<Point<float>> points;
    //==============================================================================
    double sampleRate;
    //==============================================================================
    GraphicsCircularBuffer<float>* circularBuffer;
    bool isDrawing = false;
    int numSamplesInBuffer = 0;
    //==============================================================================
    void computeFFT();
    int nfft;
    kiss_fftr_cfg cfg;
    kiss_fft_scalar* cx_in;
    kiss_fft_cpx* cx_out;
    //==============================================================================
    float tau(float x);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTViewer)
};
