#pragma once


namespace se
{
	class Shader
	{
	public:
		Shader() = default;
		~Shader();

	private:	
		unsigned int shaderProgram;
		unsigned int fragmentShader;
		unsigned int vertextShader;
	};
}