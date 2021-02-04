#pragma once
#include "glad/glad.h"

namespace se
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool loadTexture(const char* fileName);
		void unLoadTexture();
		void activeTexture();
		GLuint getTexture() const { return texture; }

	private:
		GLuint texture;
		int width;
		int height;
	};
}