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

	addAndMakeVisible(numSamplesLabel);
	numSamplesLabel.setText("Num samples in current audio data: ", dontSendNotification);
	numSamplesLabel.setColour(Label::textColourId, Colours::orange);
	numSamplesLabel.setJustificationType(Justification::left);

	for (int i = 0; i < 16; i++)
	{
		freqLabels.push_back(new Label("freq" + String(i), "freq" + String(i)));
		addAndMakeVisible(freqLabels.back());
		freqLabels.back()->setColour(Label::textColourId, Colours::orange);
		freqLabels.back()->setJustificationType(Justification::left);

		magLabels.push_back(new Label("ampl" + String(i), "ampl" + String(i)));
		addAndMakeVisible(magLabels.back());
		magLabels.back()->setColour(Label::textColourId, Colours::orange);
		magLabels.back()->setJustificationType(Justification::left);
	}

	addAndMakeVisible(computeTimeLabel);
	computeTimeLabel.setText("FFT compute time: ", dontSendNotification);
	computeTimeLabel.setColour(Label::textColourId, Colours::orange);
	computeTimeLabel.setJustificationType(Justification::left);

	addAndMakeVisible(mainFreqLabel);
	mainFreqLabel.setText("Main frequency: ", dontSendNotification);
	mainFreqLabel.setColour(Label::textColourId, Colours::orange);
	mainFreqLabel.setJustificationType(Justification::left);
}

FFTViewer::~FFTViewer()
{
}

void FFTViewer::goForDrawing(bool go)
{
	isDrawing = go;

	if (isDrawing)
	{
		DBG("Drawing");
		nfft = 1024;
		cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
		cx_in = new kiss_fft_scalar[nfft];
		cx_out = new kiss_fft_cpx[(int)(nfft / 2) + 1];

		startTimer(100);
	}
	else
	{
		DBG("Stop drawing");
		stopTimer();
		points.clear();
		numSamplesInBuffer = 0;
		repaint();
		/*free(cfg);
		delete[] cx_in;
		delete[] cx_out;*/
	}
}

void FFTViewer::paint(Graphics& g)
{
	g.setColour(Colours::blue);
	if (!points.empty())
	{
		for (int i = 0; i < nfft / 2 + 1; i++)
		{
			g.drawVerticalLine(points[i].getX(), 300 - points[i].getY(), 300);
		}
	}
}

void FFTViewer::resized()
{
	Rectangle<int> area(getLocalBounds());
	title.setBounds(area.removeFromTop(30));

	numSamplesLabel.setBounds(area.removeFromTop(30));

	Rectangle<int> data(area.removeFromTop(350));

	Rectangle<int> left(data.removeFromLeft(200));
	Rectangle<int> right(data.removeFromLeft(200));

	for (int i = 0; i < 16; i++)
	{
		freqLabels[i]->setBounds(left.removeFromTop(20));
		magLabels[i]->setBounds(right.removeFromTop(20));
	}

	mainFreqLabel.setBounds(area.removeFromTop(30));
	computeTimeLabel.setBounds(area);
}

void FFTViewer::timerCallback()
{
	computeFFT();
	repaint();
	resized();
}

float FFTViewer::tau(float x)
{
	return 0.25 * log10(3.0 * x * x + 6.0 * x + 1.0) - (sqrt(6.0) / 24.0) * log10((x + 1.0 - sqrt(2.0 / 3.0)) / (x + 1.0 + sqrt(2.0 / 3.0)));
}

void FFTViewer::computeFFT()
{
	auto start = std::chrono::high_resolution_clock::now();

	SamplesBuffer<float> sb;

	points.clear();

	int emptySpace = nfft;
	int head = 0;


	// fill the signal buffer to compute
	while (emptySpace != 0)
	{

		if (!circularBuffer->isEmpty())
		{

			sb = circularBuffer->get();
			numSamplesInBuffer = sb.numSamples;
			numSamplesLabel.setText("Num samples in current audio data: " + String(numSamplesInBuffer), dontSendNotification);

			if (sb.numSamples <= emptySpace)
			{
				for (int i = 0; i < sb.numSamples; i++)
				{
					cx_in[head] = sb.buffer[i];
					head++;
				}
				emptySpace -= sb.numSamples;
			}
			else
			{
				for (int i = 0; i < emptySpace; i++)
				{
					cx_in[head] = sb.buffer[i];
					head++;
				}

				emptySpace = 0;
			}
		}
	}

	kiss_fftr(cfg, cx_in, cx_out);

	int offset = 10;
	float max_mag = 0;
	int k = 0; // index of max mag


	// Looking for max magnitude
	for (int i = 0; i < nfft / 2 + 1; i++)
	{
		float mag = sqrt(cx_out[i].r * cx_out[i].r + cx_out[i].i * cx_out[i].i);
		if (mag >= max_mag)
		{
			max_mag = mag;
			k = i;
		}

		if (i >= offset && i < 16 + offset)
		{
			double freq = (double)i * (sampleRate) / (double)nfft;
			freqLabels[i - offset]->setText(String(freq), dontSendNotification);
			magLabels[i - offset]->setText(String(mag), dontSendNotification);
		}
		points.push_back(Point<float>(20 + i, 10.0 * mag));
	}

	// Compute Quinn's second estimator (http://dspguru.com/dsp/howtos/how-to-interpolate-fft-peak/ ;
	//									 https://stackoverflow.com/questions/41783512/fft-calculating-exact-frequency-between-frequency-bins)
	float ap = (cx_out[k + 1].r * cx_out[k].r + cx_out[k + 1].i * cx_out[k].i) / (cx_out[k].r * cx_out[k].r + cx_out[k].i * cx_out[k].i);
	float dp = -1.0 * ap / (1.0 - ap);
	float am = (cx_out[k - 1].r * cx_out[k].r + cx_out[k - 1].i * cx_out[k].i) / (cx_out[k].r * cx_out[k].r + cx_out[k].i * cx_out[k].i);
	float dm = am / (1.0 - am);
	float d;
	if (dp > 0 && dm > 0)
	{
		d = dp;
	}
	else
	{
		d = dm;
	}
	float interpolatedIndex = k + d;

	float freq = interpolatedIndex * (sampleRate) / (double)nfft;

	mainFreqLabel.setText("Main frequency: " + String(freq) + " Hz", dontSendNotification);
	

	// Computation time
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();

	computeTimeLabel.setText("FFT compute time: " + String(duration) + " micro seconds", dontSendNotification);
}
