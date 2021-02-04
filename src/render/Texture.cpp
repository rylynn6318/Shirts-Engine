#include "render/Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

se::Texture::Texture() : texture(0), width(0), height(0)
{

}

se::Texture::~Texture()
{

}


bool se::Texture::loadTexture(const char* fileName)
{
	int channels = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 텍스처 wrapping/filtering 옵션 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//텍스쳐 로드
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);
	int format = GL_RGB;
	if (channels == 3)
	{
		format = GL_RGB;
	}
	else if (channels == 4)
	{
		format = GL_RGBA;
	}
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR:: Failed to load Texture" << std::endl;
		return false;
	}

	stbi_image_free(data);
	return true;
}

void se::Texture::activeTexture()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void se::Texture::unLoadTexture()
{
	glDeleteTextures(1, &texture);
}