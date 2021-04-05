#pragma once
#include <string>

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
		GLuint getTexture() const { return ID; }

	public:
		std::string path;
		std::string type;

	private:
		GLuint ID;
		int width;
		int height;
	};
}