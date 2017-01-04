#include "diffusematerial.hpp"


#include <logger.hpp>
#include "shaders/diffuseshader.hpp"


logger_t lModule = loggerModule(loggerLWarning, loggerDFull);


DiffuseMaterial::DiffuseMaterial(const Color& color,
								 const float& diffuse,
								 const float& specular,
								 const float& hardness)
	: Material("Diffusematerial", color),
	  _diffuse(diffuse),
	  _specular(specular),
	  _hardness(hardness)
{
	static const ShaderPtr diffuseShader = new DiffuseShader();
	_setShader(diffuseShader);
	logDebug << getName() << " created." << logEndl;
}


DiffuseMaterial::~DiffuseMaterial()
{
	logDebug << getName() << " removed." << logEndl;
}


const float& DiffuseMaterial::getDiffuse() const
{
	return _diffuse;
}


const float& DiffuseMaterial::getSpecular() const
{
	return _specular;
}


const float&DiffuseMaterial::getHardness() const
{
	return _hardness;
}


void DiffuseMaterial::setDiffuse(const float& diffuse)
{
	_diffuse = diffuse;
}


void DiffuseMaterial::setSpecular(const float& specular)
{
	_specular = specular;
}


void DiffuseMaterial::setHardness(const float& hardness)
{
	_hardness = hardness;
}


void DiffuseMaterial::use()
{
	Material::use();

	_shader->passUniform("material.diffuse", _diffuse);
	_shader->passUniform("material.specular", _specular);
	_shader->passUniform("material.hardness", _hardness);
}
