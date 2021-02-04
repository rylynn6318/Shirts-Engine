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

	private:
		GLuint texture;
		int width;
		int height;
	};
}