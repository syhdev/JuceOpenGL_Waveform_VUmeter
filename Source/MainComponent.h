/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveformViewer.h"
#include "VUMetre.h"
#include "FFTViewer.h"

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public AudioAppComponent,
	public ChangeListener
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint(Graphics& g) override;
	void resized() override;

private:
	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};
	//==============================================================================
	TextButton wvButton;
	TextButton vuButton;
	TextButton fftButton;
	void wvButtonClicked();
	void vuButtonClicked();
	void fftButtonClicked();
	//==============================================================================
	AudioFormatManager formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioTransportSource transportSource;
	TransportState state;
	void changeState(TransportState newState);
	void changeListenerCallback(ChangeBroadcaster* source) override;
	//==============================================================================
	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;
	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	//==============================================================================
	WaveformViewer waveformViewer;
	VUMetre vuMetre;
	GraphicsCircularBuffer<float> circularBuffer;
	//==============================================================================
	FFTViewer fftViewer;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
