/*
  ==============================================================================

	WaveformViewer.cpp
	Created: 14 May 2020 8:30:51pm
	Author:  Sylvain

  ==============================================================================
*/

#include "VUMetre.h"

//==============================================================================
VUMetre::VUMetre():
	circularBuffer(16)
{
	//Sets the OpenGL version to 3.2
	glContext.setOpenGLVersionRequired(OpenGLContext::OpenGLVersion::openGL3_2);
	glContext.setRenderer(this);
	glContext.attachTo(*this);
	glContext.setContinuousRepainting(true);

	setSize(800, 600);

	//addAndMakeVisible(dp);
	//path.startNewSubPath(Point<float>(5, 5));
	//path.lineTo(Point<float>(10, 10));
	//path.lineTo(Point<float>(10, 20));
	//path.lineTo(Point<float>(50, 20));
	//path.lineTo(Point<float>(100, 20));
	//path.closeSubPath();
	//dp.setPath(path);

	//setFramesPerSecond(24);

	vertexShader =
#include "vertexShader.glsl"
		;

	fragmentShader =
#include "fragmentShader.glsl"
		;
}

VUMetre::~VUMetre()
{
	glContext.setContinuousRepainting(false);
	glContext.detach();
}

//==============================================================================
void VUMetre::paint(Graphics& g)
{


	//g.setColour(cOpBackground);
	//g.fillAll();



	/*g.setColour(Colours::aquamarine);
	g.drawVerticalLine(xx, 0.0, 50.0);

	xx++;

	if (xx > 100) {
		xx = 0;
	}*/

	//g.setColour(Colours::white);
	//g.drawVerticalLine(20, 0.0, 100.0);
	//g.drawVerticalLine(40, 0.0, 50.0);

	//DBG("paint");

	//float v;
	//float voffset = 10.0;
	//SamplesBuffer<float> sb = circularBuffer->get();
	//g.setColour(Colours::white);
	//for (int i = 0; i < sb.numSamples; i++) {
	//	v = sb.buffer[i];
	//	g.drawVerticalLine(i, 0.0 + voffset, v * 100.0 + voffset + 3.0);
	//}
}

void VUMetre::resized()
{
	Rectangle<int> area(getLocalBounds());
	//dp.setBounds(area);
}

//void WaveformViewer::update()
//{
//}

//void WaveformViewer::changeListenerCallback(ChangeBroadcaster * source)
//{
//	repaint();
//}

void VUMetre::newOpenGLContextCreated()
{
	DBG("opening GL");
	// Setup Shaders
	createShaders();

	// Setup Buffer Objects
	//glContext.extensions.glGenBuffers(1, &VBO); // Vertex Buffer Object
	//glContext.extensions.glGenBuffers(1, &EBO); // Vertex Buffer Object
}

void VUMetre::openGLContextClosing()
{
	//shaderProgram->release();
	//shaderProgram = nullptr;
	DBG("closing GL");
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

	drawBackgroundStuff(desktopScale);

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

	//if (uniforms->audioData.get() != nullptr)
	//{
	//	SamplesBuffer<float> sb;
	//	sb = circularBuffer->get();
	//	//DBG(sb.buffer[0]);
	//	//FloatVectorOperations::clear(visualizationBuffer, sb.numSamples);
	//	//FloatVectorOperations::add(visualizationBuffer, sb.buffer.getReadPointer(i, 0), RING_BUFFER_READ_SIZE);
	//	uniforms->audioData->set((GLfloat*)sb.buffer, sb.numSamples);
	//}

	shape->draw(glContext, *attributes, &circularBuffer);

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

		shape.reset(new Shape(glContext));
		attributes.reset(new Attributes(glContext, *shaderProgram));
		uniforms.reset(new Uniforms(glContext, *shaderProgram));

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

		/*xx *= 0.995f;
		if (xx <= 0.01f) {
			xx = 1.0f;
		}*/

		/*for (int i = 0; i < 256; i++) {
			g.setColour(Colours::orange);
			g.drawVerticalLine(i, sin(xx * i) * 20.0f+50.0f, getHeight());
		}*/

		/*path.clear();

		path.startNewSubPath(Point<float>(0, getHeight() / 2));
		for (int i = 0; i < 256; i++) {
			path.lineTo(Point<float>(2* i, sin(xx * i) * 40.0f + getHeight()/2));
		}
		path.lineTo(Point<float>(2 * 256, getHeight()));
		path.lineTo(Point<float>(0, getHeight()));
		path.closeSubPath();

		PathStrokeType stroke(3.0f);
		g.setColour(Colours::orange.darker(0.2f));
		g.strokePath(path, stroke);
		g.setColour(Colours::orange.withAlpha(0.5f));
		g.fillPath(path);*/
	}
}

