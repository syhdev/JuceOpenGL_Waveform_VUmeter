/*
  ==============================================================================

	WaveformViewer.cpp
	Created: 14 May 2020 8:30:51pm
	Author:  Sylvain

  ==============================================================================
*/

#include "VUMetre.h"

//==============================================================================
VUMetre::VUMetre()
{
	//Sets the OpenGL version to 3.2
	glContext.setOpenGLVersionRequired(OpenGLContext::OpenGLVersion::openGL3_2);
	glContext.setRenderer(this);
	glContext.attachTo(*this);
	glContext.setContinuousRepainting(true);

	setSize(800, 600);

	vertexShader =
#include "VUvertexShader.glsl"
		;

	fragmentShader =
#include "VUfragmentShader.glsl"
		;

	addAndMakeVisible(title);
	title.setText("VU Meter", dontSendNotification);
	title.setColour(Label::textColourId, Colours::orange);
	title.setJustificationType(Justification::left);
}

VUMetre::~VUMetre()
{
	glContext.setContinuousRepainting(false);
	glContext.detach();
}

//==============================================================================
void VUMetre::paint(Graphics& g)
{
}

void VUMetre::resized()
{
	Rectangle<int> area(getLocalBounds());
	title.setBounds(area.removeFromTop(30));
}


void VUMetre::newOpenGLContextCreated()
{
	createShaders();
}

void VUMetre::openGLContextClosing()
{
	shaderProgram.reset();
	shape.reset();
	attributes.reset();
	uniforms.reset();
}


/** The OpenGL rendering callback.
	 */
void VUMetre::renderOpenGL()
{
	jassert(OpenGLHelpers::isContextActive());

	auto desktopScale = (float)glContext.getRenderingScale();
	OpenGLHelpers::clear(Colours::azure);

	// Draw Juce stuff
	//drawBackgroundStuff(desktopScale);

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

	//shape->draw(glContext, *attributes, *uniforms, circularBuffer);

	// Reset the element buffers so child Components draw correctly
	glContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	glContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VUMetre::createShaders()
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

		shape.reset(new VUShape(glContext));
		attributes.reset(new VUAttributes(glContext, *shaderProgram));
		uniforms.reset(new VUUniforms(glContext, *shaderProgram));

		statusText = "GLSL: v" + String(OpenGLShaderProgram::getLanguageVersion(), 2);
	}
	else
	{
		statusText = newShaderProgram->getLastError();
	}
}

Matrix3D<float> VUMetre::getProjectionMatrix() const
{
	auto w = 1.0f / (0.5f + 0.1f);
	auto h = w * getLocalBounds().toFloat().getAspectRatio(false);

	return Matrix3D<float>::fromFrustum(-w, w, -h, h, 4.0f, 30.0f);
}

Matrix3D<float> VUMetre::getViewMatrix() const
{

	Matrix3D<float> viewMatrix({ 0.0f, 0.0f, -10.0f }); // [4]

	return viewMatrix;
}

void VUMetre::drawBackgroundStuff(float desktopScale)
{
	std::unique_ptr<LowLevelGraphicsContext> glRenderer(createOpenGLGraphicsContext(glContext,
		roundToInt(desktopScale * getWidth()),
		roundToInt(desktopScale * getHeight())));

	if (glRenderer.get() != nullptr)
	{
		Graphics g(*glRenderer);
		g.setColour(Colours::darkslategrey);
		g.fillAll();
	}
}

