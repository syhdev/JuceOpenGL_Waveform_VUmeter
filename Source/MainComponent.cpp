/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() :
	waveformViewer(),
	vuMetre(),
	fftViewer(),
	state(Stopped),
	circularBuffer(16)
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize(1200, 800);

	// Some platforms require permissions to open input channels so request that here
	if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
		&& !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
	{
		RuntimePermissions::request(RuntimePermissions::recordAudio,
			[&](bool granted) { if (granted)  setAudioChannels(2, 2); });
	}
	else
	{
		// Specify the number of input and output channels that we want to open
		setAudioChannels(2, 2);
	}

	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open...");
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);

	transportSource.addChangeListener(this);

	addAndMakeVisible(waveformViewer);
	addAndMakeVisible(vuMetre);
	addAndMakeVisible(fftViewer);


	addAndMakeVisible(&wvButton);
	wvButton.setButtonText("Waveform");
	wvButton.setClickingTogglesState(true);
	wvButton.setRadioGroupId(34567);
	wvButton.setColour(TextButton::textColourOffId, Colours::black);
	wvButton.setColour(TextButton::textColourOnId, Colours::black);
	wvButton.setColour(TextButton::buttonColourId, Colours::white);
	wvButton.setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
	wvButton.setConnectedEdges(Button::ConnectedOnBottom);
	wvButton.setToggleState(true, dontSendNotification);
	wvButton.onClick = [this] { wvButtonClicked(); };

	addAndMakeVisible(&vuButton);
	vuButton.setButtonText("VU meter");
	vuButton.setClickingTogglesState(true);
	vuButton.setRadioGroupId(34567);
	vuButton.setColour(TextButton::textColourOffId, Colours::black);
	vuButton.setColour(TextButton::textColourOnId, Colours::black);
	vuButton.setColour(TextButton::buttonColourId, Colours::white);
	vuButton.setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
	vuButton.setConnectedEdges(0);
	vuButton.onClick = [this] { vuButtonClicked(); };

	addAndMakeVisible(&fftButton);
	fftButton.setButtonText("FFT");
	fftButton.setClickingTogglesState(true);
	fftButton.setRadioGroupId(34567);
	fftButton.setColour(TextButton::textColourOffId, Colours::black);
	fftButton.setColour(TextButton::textColourOnId, Colours::black);
	fftButton.setColour(TextButton::buttonColourId, Colours::white);
	fftButton.setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
	fftButton.setConnectedEdges(Button::ConnectedOnTop);
	fftButton.onClick = [this] { fftButtonClicked(); };
	


	formatManager.registerBasicFormats();

	waveformViewer.setCircularBuffer(&circularBuffer);
	vuMetre.setCircularBuffer(&circularBuffer);
	fftViewer.setCircularBuffer(&circularBuffer);
}

MainComponent::~MainComponent()
{
	// This shuts down the audio device and clears the audio source.
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	// This function will be called when the audio device is started, or when
	// its settings (i.e. sample rate, block size, etc) are changed.

	// You can use this function to initialise any resources you might need,
	// but be careful - it will be called on the audio thread, not the GUI thread.

	fftViewer.setSampleRate(sampleRate);

	// For more details, see the help for AudioProcessor::prepareToPlay()

	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	// Your audio-processing code goes here!

	// For more details, see the help for AudioProcessor::getNextAudioBlock()

	if (readerSource.get() == nullptr)
	{
		// Right now we are not producing any data, in which case we need to clear the buffer
		// (to prevent the output of random noise)
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	transportSource.getNextAudioBlock(bufferToFill);
	circularBuffer.put(bufferToFill.buffer->getReadPointer(1), bufferToFill.buffer->getNumSamples());
}

void MainComponent::releaseResources()
{
	// This will be called when the audio device stops, or when it is being
	// restarted due to a setting change.

	// For more details, see the help for AudioProcessor::releaseResources()

	transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	// You can add your drawing code here!
}

void MainComponent::resized()
{
	Rectangle<int> area(getBounds());

	Rectangle<int> btnZone = area.removeFromTop(150);
	Rectangle<int> choiceZone = btnZone.removeFromRight(200);

	openButton.setBounds(btnZone.removeFromTop(50));
	playButton.setBounds(btnZone.removeFromTop(50));
	stopButton.setBounds(btnZone.removeFromTop(50));

	wvButton.setBounds(choiceZone.removeFromTop(30));
	vuButton.setBounds(choiceZone.removeFromTop(30));
	fftButton.setBounds(choiceZone.removeFromTop(30));

	waveformViewer.setBounds(area.removeFromLeft(400));
	vuMetre.setBounds(area.removeFromLeft(400));
	fftViewer.setBounds(area);
}

void MainComponent::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;

		switch (state)
		{
		case Stopped:
			stopButton.setEnabled(false);
			playButton.setEnabled(true);
			transportSource.setPosition(0.0);
			break;

		case Starting:
			playButton.setEnabled(false);
			transportSource.start();
			break;

		case Playing:
			stopButton.setEnabled(true);
			break;

		case Stopping:
			transportSource.stop();
			break;
		}
	}
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &transportSource)
	{
		if (transportSource.isPlaying())
			changeState(Playing);
		else
			changeState(Stopped);
	}
}

void MainComponent::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...",
		{},
		"*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto* reader = formatManager.createReaderFor(file);

		if (reader != nullptr)
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
			playButton.setEnabled(true);
			readerSource.reset(newSource.release());
		}
	}
}

void MainComponent::playButtonClicked()
{
	changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
	changeState(Stopping);
}

void MainComponent::wvButtonClicked()
{
	vuMetre.goForDrawing(false);
	fftViewer.goForDrawing(false);
	waveformViewer.goForDrawing(true);
}

void MainComponent::vuButtonClicked()
{
	waveformViewer.goForDrawing(false);
	fftViewer.goForDrawing(false);
	vuMetre.goForDrawing(true);
}

void MainComponent::fftButtonClicked()
{
	waveformViewer.goForDrawing(false);
	vuMetre.goForDrawing(false);
	fftViewer.goForDrawing(true);
}

