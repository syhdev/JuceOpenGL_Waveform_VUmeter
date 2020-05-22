/*
  ==============================================================================

	WaveformViewer.h
	Created: 14 May 2020 8:30:51pm
	Author:  Sylvain

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

struct Vertex
{
	float vertex[3];
	float value[2];

	Vertex(float vx, float vy, float vz, float valx, float valy)
	{
		vertex[0] = vx;
		vertex[1] = vy;
		vertex[2] = vz;
		value[0] = valx;
		value[1] = valy;
	}
};

struct Attributes
{
	std::unique_ptr<OpenGLShaderProgram::Attribute> vertex, value;

	Attributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		vertex.reset(createAttribute(openGLContext, shaderProgram, "vertex"));
		value.reset(createAttribute(openGLContext, shaderProgram, "value"));
	}

	void enable(OpenGLContext& openGLContext)
	{
		if (vertex.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(vertex->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			openGLContext.extensions.glEnableVertexAttribArray(vertex->attributeID);
		}

		if (value.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(value->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
			openGLContext.extensions.glEnableVertexAttribArray(value->attributeID);
		}
	}

	void disable(OpenGLContext& openGLContext)
	{
		if (vertex.get() != nullptr)       openGLContext.extensions.glDisableVertexAttribArray(vertex->attributeID);
		if (value.get() != nullptr)         openGLContext.extensions.glDisableVertexAttribArray(value->attributeID);
	}

private:
	static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext, OpenGLShaderProgram& shader, const String& attributeName)
	{
		if (openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attributeName.toRawUTF8()) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Attribute(shader, attributeName.toRawUTF8());
	}
};

struct Uniforms
{
	std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, audioData, resolution, texture;

	Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		projectionMatrix.reset(createUniform(openGLContext, shaderProgram, "projectionMatrix"));
		viewMatrix.reset(createUniform(openGLContext, shaderProgram, "viewMatrix"));
		resolution.reset(createUniform(openGLContext, shaderProgram, "resolution"));
		audioData.reset(createUniform(openGLContext, shaderProgram, "audioData"));
		texture.reset(createUniform(openGLContext, shaderProgram, "myTexture"));
	}

private:
	static OpenGLShaderProgram::Uniform* createUniform(
		OpenGLContext& openGLContext,
		OpenGLShaderProgram& shaderProgram,
		const String& uniformName)
	{
		if (openGLContext.extensions.glGetUniformLocation(shaderProgram.getProgramID(), uniformName.toRawUTF8()) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Uniform(shaderProgram, uniformName.toRawUTF8());
	}

};

struct VertexBuffer
{
	GLuint VBO;
	GLuint EBO;
	OpenGLContext& openGLContext;
	Array<Vertex> vertices;
	float xx = 1.0;

	VertexBuffer(OpenGLContext& context) : openGLContext(context)
	{
		openGLContext.extensions.glGenBuffers(1, &VBO);
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);

		openGLContext.extensions.glGenBuffers(1, &EBO);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		for (int i = 0; i < 256; i++) {
			vertices.add(Vertex(i / 256.0f, i / 256.0f, 0.0f, 0.0f, 0.0f));
		}


		int indices[2 * 256 - 2];
		for (int i = 0; i < 255; i++) {
			indices[2 * i] = i;
			indices[2 * i + 1] = i + 1;
		}

		openGLContext.extensions.glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr> (static_cast<size_t> (vertices.size()) * sizeof(Vertex)),
			vertices.getRawDataPointer(),
			GL_STATIC_DRAW);

		openGLContext.extensions.glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(int) * (2 * 256 - 2),
			indices,
			GL_STATIC_DRAW);
	}

	~VertexBuffer()
	{
		openGLContext.extensions.glDeleteBuffers(1, &VBO);
		openGLContext.extensions.glDeleteBuffers(1, &EBO);
	}

	void bind()
	{
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}

	void setAudioData()
	{
		vertices.clear();

		xx *= 0.995f;
		if (xx <= 0.01f) {
			xx = 1.0f;
		}

		//for (int i = 0; i < 256; i++) {
		//	vertices.add(Vertex(2* i / 256.0f - 1.0f, sin(xx * i), 0.0f, 0.0f, 0.0f));
		//}

		vertices.add(Vertex(-1.0, -1.0, 0.0f, 0.0f, 0.0f));
		vertices.add(Vertex(-1.0, 1.0, 0.0f, 0.0f, 0.0f));
		vertices.add(Vertex(1.0, 1.0, 0.0f, 0.0f, 0.0f));
		vertices.add(Vertex(1.0, -1.0, 0.0f, 0.0f, 0.0f));


		openGLContext.extensions.glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr> (static_cast<size_t> (vertices.size()) * sizeof(Vertex)),
			vertices.getRawDataPointer(),
			GL_STATIC_DRAW);
	}
};

struct Shape
{
	OwnedArray<VertexBuffer> vertexBuffers;
	OpenGLTexture texture;
	//PixelARGB image[16][16];
	PixelARGB image[4096];
	float xx = 1.0f;

	Shape(OpenGLContext& openGLContext)
	{
		vertexBuffers.add(new VertexBuffer(openGLContext));

		
		/*for (int i = 0; i < 16; i++) 
		{
			for (int j = 0; j < 16; j++)
			{
				image[i][j] = PixelARGB(1, 255, 0, 0);
			}
		}*/

		//texture.loadARGB(*image, 16, 16);



	}


	void draw(OpenGLContext& openGLContext, Attributes& glAttributes, GraphicsCircularBuffer<float>* cb)
	{
		for (auto* vertexBuffer : vertexBuffers)
		{
			vertexBuffer->bind();

			//xx *= 0.995f;
			//if (xx <= 0.01f) {
			//	xx = 1.0f;
			//}

			/*for (int i = 0; i < 16; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					image[i][j] = PixelARGB(1, xx * 255, xx * 128, 20);
				}
			}

			texture.loadARGB(*image, 16, 16);*/

			if(!cb->isEmpty())
			{
				SamplesBuffer<float> sb = cb->get();
				for (int i = 0; i < sb.numSamples; i++)
				{
					image[i] = PixelARGB(1, (sb.buffer[i] + 1) * 255 / 2, 128, 0);
				}

				texture.loadARGB(image, sb.numSamples, 1);
			}
			else {
				for (int i = 0; i < 16; i++)
				{
					image[i] = PixelARGB(1, 255 , 128, 0);
				}

				texture.loadARGB(image, 16, 1);
			}
			

			texture.bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			vertexBuffer->setAudioData();

			glAttributes.enable(openGLContext);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			//glDrawElements(GL_LINES, 2 * 256, GL_UNSIGNED_INT, 0);
			glAttributes.disable(openGLContext);
		}
	}
};





class WaveformViewer : public Component, public OpenGLRenderer //public DrawablePath ////, public AnimatedAppComponent
{
public:
	//==============================================================================
	WaveformViewer();
	~WaveformViewer() override;

	//==============================================================================
	void paint(Graphics& g) override;
	void resized() override;
	//void update() override;
	//==============================================================================
	/** Called before rendering OpenGL, after an OpenGLContext has been associated
		with this OpenGLRenderer (this component is a OpenGLRenderer).
		Sets up GL objects that are needed for rendering.
	 */
	void newOpenGLContextCreated() override;

	/** Called when done rendering OpenGL, as an OpenGLContext object is closing.
		Frees any GL objects created during rendering.
	 */
	void openGLContextClosing() override;

	void renderOpenGL() override;

	GraphicsCircularBuffer<float>* getCircularBuffer() { return &circularBuffer; }


private:

	std::unique_ptr<OpenGLShaderProgram> shaderProgram;
	std::unique_ptr<Shape> shape;
	std::unique_ptr<Attributes> attributes;
	std::unique_ptr<Uniforms> uniforms;
	const char* vertexShader;
	const char* fragmentShader;
	void createShaders();
	//==============================================================================
	OpenGLContext glContext;
	//==============================================================================
	Matrix3D<float> getProjectionMatrix() const;
	Matrix3D<float> getViewMatrix() const;
	//==============================================================================
	void drawBackgroundStuff(float desktopScale);
	//==============================================================================
	GraphicsCircularBuffer<float> circularBuffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformViewer)
};


