#ifndef __SHADER_HPP__
#define __SHADER_HPP__


#include <string>
#include <sharedpointer.h>
#include "opengls.hpp"
#include "component.hpp"


class Shader : public Component
{
	public:
		Shader(const std::string& name = std::string(),
			   const std::string& src = std::string());
		Shader(const Shader& sh);
		Shader(Shader&& sh);
		~Shader();

		Shader& operator=(const Shader& sh);
		Shader& operator=(Shader&& sh);

		virtual void initUniformsLocations(const GLuint&);
		virtual void prepareForRender();

		const char* getSrcPtr() const;
		const std::string& getSrc() const;

	private:
		std::string _src;
};

using ShaderPtr = SharedPointer<Shader>;


#endif // __SHADER_HPP__
