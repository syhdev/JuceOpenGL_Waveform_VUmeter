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

	addAndMakeVisible(numSamplesTitleLabel);
	numSamplesTitleLabel.setText("Num samples in current audio data: ", dontSendNotification);
	numSamplesTitleLabel.setColour(Label::textColourId, Colours::orange);
	numSamplesTitleLabel.setJustificationType(Justification::left);
}

FFTViewer::~FFTViewer()
{
}

void FFTViewer::goForDrawing(bool go)
{
	isDrawing = go;
	if (isDrawing)
	{
		startTimer(100);
	}
	else
	{
		stopTimer();
		label = 0;
		repaint();
	}
}

void FFTViewer::paint(Graphics& g)
{
	g.setColour(Colours::orange);

	g.drawText(String(label), Rectangle<float>(220, 30, 50, 30), Justification::centred);
}

void FFTViewer::resized()
{
	Rectangle<int> area(getLocalBounds());
	title.setBounds(area.removeFromTop(30));

	numSamplesTitleLabel.setBounds(area.removeFromTop(30));
}

void FFTViewer::timerCallback()
{
	computeFFT();
	repaint();
}

void FFTViewer::computeFFT()
{
	SamplesBuffer<float> sb;

	if (!circularBuffer->isEmpty())
	{
		sb = circularBuffer->get();
		label = sb.numSamples;
	}
}
