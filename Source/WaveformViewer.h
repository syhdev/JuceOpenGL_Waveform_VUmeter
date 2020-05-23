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

struct WVVertex
{
	float vertex[3];

	WVVertex(float vx, float vy, float vz)
	{
		vertex[0] = vx;
		vertex[1] = vy;
		vertex[2] = vz;
	}
};

struct WVAttributes
{
	std::unique_ptr<OpenGLShaderProgram::Attribute> vertex;

	WVAttributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		vertex.reset(createAttribute(openGLContext, shaderProgram, "vertex"));
	}

	void enable(OpenGLContext& openGLContext)
	{
		if (vertex.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(vertex->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(WVVertex), 0);
			openGLContext.extensions.glEnableVertexAttribArray(vertex->attributeID);
		}
	}

	void disable(OpenGLContext& openGLContext)
	{
		if (vertex.get() != nullptr)       openGLContext.extensions.glDisableVertexAttribArray(vertex->attributeID);
	}

private:
	static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext, OpenGLShaderProgram& shader, const String& attributeName)
	{
		if (openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attributeName.toRawUTF8()) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Attribute(shader, attributeName.toRawUTF8());
	}
};

struct WVUniforms
{
	std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, audioData, resolution, texture;

	WVUniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		projectionMatrix.reset(createUniform(openGLContext, shaderProgram, "projectionMatrix"));
		viewMatrix.reset(createUniform(openGLContext, shaderProgram, "viewMatrix"));
		resolution.reset(createUniform(openGLContext, shaderProgram, "resolution"));
		texture.reset(createUniform(openGLContext, shaderProgram, "audioTexture"));
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

struct WVVertexBuffer
{
	GLuint VBO;
	OpenGLContext& openGLContext;
	Array<WVVertex> vertices;

	WVVertexBuffer(OpenGLContext& context) : openGLContext(context)
	{
		openGLContext.extensions.glGenBuffers(1, &VBO);
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);

		vertices.add(WVVertex(-1.0, -1.0, 0.0f));
		vertices.add(WVVertex(-1.0, 1.0, 0.0f));
		vertices.add(WVVertex(1.0, 1.0, 0.0f));
		vertices.add(WVVertex(1.0, -1.0, 0.0f));


		openGLContext.extensions.glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr> (static_cast<size_t> (vertices.size()) * sizeof(WVVertex)),
			vertices.getRawDataPointer(),
			GL_STATIC_DRAW);
	}

	~WVVertexBuffer()
	{
		openGLContext.extensions.glDeleteBuffers(1, &VBO);
	}

	void bind()
	{
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}
};

struct WVShape
{
	OwnedArray<WVVertexBuffer> vertexBuffers;
	OpenGLTexture texture;
	PixelARGB image[4096];

	WVShape(OpenGLContext& openGLContext)
	{
		vertexBuffers.add(new WVVertexBuffer(openGLContext));

	}

	void setAudioGLSLTexture(GraphicsCircularBuffer<float>* cb)
	{
		if (!cb->isEmpty())
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
				image[i] = PixelARGB(1, 255, 128, 0);
			}

			texture.loadARGB(image, 16, 1);
		}

		texture.bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void draw(OpenGLContext& openGLContext, WVAttributes& glAttributes, GraphicsCircularBuffer<float>* cb)
	{
		for (auto* vertexBuffer : vertexBuffers)
		{
			vertexBuffer->bind();

			setAudioGLSLTexture(cb);

			glAttributes.enable(openGLContext);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glAttributes.disable(openGLContext);
		}
	}
};





class WaveformViewer : public Component, public OpenGLRenderer
{
public:
	//==============================================================================
	WaveformViewer();
	~WaveformViewer() override;

	//==============================================================================
	void paint(Graphics& g) override;
	void resized() override;
	//==============================================================================
	void newOpenGLContextCreated() override;
	void openGLContextClosing() override;
	void renderOpenGL() override;
	//==============================================================================
	void setCircularBuffer(GraphicsCircularBuffer<float>* cb) { circularBuffer = cb; }


private:
	Label title;
	//==============================================================================
	std::unique_ptr<OpenGLShaderProgram> shaderProgram;
	std::unique_ptr<WVShape> shape;
	std::unique_ptr<WVAttributes> attributes;
	std::unique_ptr<WVUniforms> uniforms;
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
	GraphicsCircularBuffer<float>* circularBuffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformViewer)
};


