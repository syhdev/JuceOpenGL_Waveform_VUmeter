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

struct VUVertex
{
	float vertex[3];
	float value[2];

	VUVertex(float vx, float vy, float vz, float valx, float valy)
	{
		vertex[0] = vx;
		vertex[1] = vy;
		vertex[2] = vz;
		value[0] = valx;
		value[1] = valy;
	}
};

struct VUAttributes
{
	std::unique_ptr<OpenGLShaderProgram::Attribute> vertex, value;

	VUAttributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		vertex.reset(createAttribute(openGLContext, shaderProgram, "vertex"));
		value.reset(createAttribute(openGLContext, shaderProgram, "value"));
	}

	void enable(OpenGLContext& openGLContext)
	{
		if (vertex.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(vertex->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(VUVertex), 0);
			openGLContext.extensions.glEnableVertexAttribArray(vertex->attributeID);
		}

		if (value.get() != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(value->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(VUVertex), (GLvoid*)(sizeof(float) * 3));
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

struct VUUniforms
{
	std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, audioData, resolution, texture;

	VUUniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
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

struct VUVertexBuffer
{
	GLuint VBO;
	GLuint EBO;
	OpenGLContext& openGLContext;
	Array<VUVertex> vertices;

	VUVertexBuffer(OpenGLContext& context) : openGLContext(context)
	{
		openGLContext.extensions.glGenBuffers(1, &VBO);
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);

		openGLContext.extensions.glGenBuffers(1, &EBO);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}

	~VUVertexBuffer()
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

		vertices.add(VUVertex(-1.0, -1.0, 0.0f, 0.0f, 0.0f));
		vertices.add(VUVertex(-1.0, 1.0, 0.0f, 0.0f, 0.0f));
		vertices.add(VUVertex(1.0, 1.0, 0.0f, 0.0f, 0.0f));
		vertices.add(VUVertex(1.0, -1.0, 0.0f, 0.0f, 0.0f));


		openGLContext.extensions.glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr> (static_cast<size_t> (vertices.size()) * sizeof(VUVertex)),
			vertices.getRawDataPointer(),
			GL_STATIC_DRAW);
	}
};

struct VUShape
{
	OwnedArray<VUVertexBuffer> vertexBuffers;
	OpenGLTexture texture;
	PixelARGB image[4096];
	float xx = 1.0f;

	VUShape(OpenGLContext& openGLContext)
	{
		vertexBuffers.add(new VUVertexBuffer(openGLContext));
	}


	void draw(OpenGLContext& openGLContext, VUAttributes& glAttributes, GraphicsCircularBuffer<float>* cb)
	{
		for (auto* vertexBuffer : vertexBuffers)
		{
			vertexBuffer->bind();

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
			glAttributes.disable(openGLContext);
		}
	}
};





class VUMetre : public Component, public OpenGLRenderer
{
public:
	//==============================================================================
	VUMetre();
	~VUMetre() override;
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
	std::unique_ptr<VUShape> shape;
	std::unique_ptr<VUAttributes> attributes;
	std::unique_ptr<VUUniforms> uniforms;
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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VUMetre)
};


