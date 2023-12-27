#include "material.h"

namespace renderer
{
	Material::Material(Shader* pShader, graphics::ITexture* pTexture)
		: pShader(pShader), pTexture(pTexture)
	{

	}

	Material::~Material() = default;

	Shader* Material::getShader() const
	{
		return pShader;
	}

	graphics::ITexture* Material::getTexture() const
	{
		return pTexture;
	}
}