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

	//backpack은 이부분 반대로
	stbi_set_flip_vertically_on_load(false); 
	unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);
	
	if (data)
	{
		GLenum format = GL_RGB;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// �ؽ�ó wrapping/filtering �ɼ� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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