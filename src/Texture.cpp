#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

#include <iostream>

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_Filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_height(0), m_BPP(0)
{
	// Load the texture data from file
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_height, &m_BPP, 4);

	if (!m_LocalBuffer)
	{
		std::cerr << "Failed to load texture: " << path << '\n';
		std::cerr << "Reason: " << stbi_failure_reason() << '\n';
	}

	// Generate and bind the texture
	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Set texture parameters
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Upload the texture data to the GPU
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	// Unbind the texture
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	// Free the local buffer
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}