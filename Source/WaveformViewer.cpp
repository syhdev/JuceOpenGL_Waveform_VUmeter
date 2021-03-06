/*
  ==============================================================================

	WaveformViewer.cpp
	Created: 14 May 2020 8:30:51pm
	Author:  Sylvain

  ==============================================================================
*/

#include "WaveformViewer.h"

//==============================================================================
WaveformViewer::WaveformViewer()
{
	//Sets the OpenGL version to 3.2
	glContext.setOpenGLVersionRequired(OpenGLContext::OpenGLVersion::openGL3_2);
	glContext.setRenderer(this);
	glContext.attachTo(*this);
	glContext.setContinuousRepainting(true);

	setSize(800, 600);

	addAndMakeVisible(title);
	title.setText("Waveform Viewer", dontSendNotification);
	title.setColour(Label::textColourId, Colours::orange);
	title.setJustificationType(Justification::left);

	vertexShader =
#include "WVvertexShader.glsl"
		;

	fragmentShader =
#include "WVfragmentShader.glsl"
		;
}

WaveformViewer::~WaveformViewer()
{
	glContext.setContinuousRepainting(false);
	glContext.detach();
}

//==============================================================================
void WaveformViewer::paint(Graphics& g)
{}

void WaveformViewer::resized()
{
	Rectangle<int> area(getLocalBounds());
	title.setBounds(area.removeFromTop(30));
}


void WaveformViewer::newOpenGLContextCreated()
{
	createShaders();
}

void WaveformViewer::openGLContextClosing()
{
	shaderProgram.reset();
	shape.reset();
	attributes.reset();
	uniforms.reset();
}


/** The OpenGL rendering callback.
	 */
void WaveformViewer::renderOpenGL()
{
	jassert(OpenGLHelpers::isContextActive());

	auto desktopScale = (float)glContext.getRenderingScale();
	OpenGLHelpers::clear(Colours::azure);

	drawBackgroundStuff(desktopScale);

	if (isDrawing)
	{

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glContext.extensions.glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

		shaderProgram->use();

		if (uniforms->projectionMatrix.get() != nullptr)
			uniforms->projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1, false);

		if (uniforms->viewMatrix.get() != nullptr)
			uniforms->viewMatrix->setMatrix4(getViewMatrix().mat, 1, false);

		if (uniforms->resolution.get() != nullptr)
			uniforms->resolution->set((GLfloat)desktopScale * getWidth(), (GLfloat)desktopScale * getHeight());

		if (uniforms->texture.get() != nullptr)
			uniforms->texture->set((GLint)0);

		shape->draw(glContext, *attributes, circularBuffer);

		// Reset the element buffers so child Components draw correctly
		glContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void WaveformViewer::createShaders()
{

	std::unique_ptr<OpenGLShaderProgram> newShaderProgram(new OpenGLShaderProgram(glContext));
	String statusText;

	if (newShaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertexShader))
		&& newShaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragmentShader))
		&& newShaderProgram->link())
	{
		shape.reset();
		attributes.reset();
		uniforms.reset();

		shaderProgram.reset(newShaderProgram.release());
		shaderProgram->use();

		shape.reset(new WVShape(glContext));
		attributes.reset(new WVAttributes(glContext, *shaderProgram));
		uniforms.reset(new WVUniforms(glContext, *shaderProgram));

		statusText = "GLSL: v" + String(OpenGLShaderProgram::getLanguageVersion(), 2);
	}
	else
	{
		statusText = newShaderProgram->getLastError();
	}
}

Matrix3D<float> WaveformViewer::getProjectionMatrix() const
{
	auto w = 1.0f / (0.5f + 0.1f);
	auto h = w * getLocalBounds().toFloat().getAspectRatio(false);

	return Matrix3D<float>::fromFrustum(-w, w, -h, h, 4.0f, 30.0f);
}

Matrix3D<float> WaveformViewer::getViewMatrix() const
{

	Matrix3D<float> viewMatrix({ 0.0f, 0.0f, -10.0f });

	return viewMatrix;
}

void WaveformViewer::drawBackgroundStuff(float desktopScale)
{
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(glContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));

	if (glRenderer.get() != nullptr)
	{
		Graphics g(*glRenderer);
		g.setColour(Colours::darkslategrey);
		g.fillAll();

		g.setColour(Colours::white);
		for (int i = 0; i < getWidth() / 20; i++)
		{
			g.drawVerticalLine(20 * i, 0, getHeight());
		}

		for (int j = 0; j < getHeight() / 20; j++)
		{
			g.drawHorizontalLine(20 * j, 0, getWidth());
		}
	}
}

