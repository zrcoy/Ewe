#include "GamePCH.h"

#include "Material.h"
#include "Mesh/Texture.h"

Material::Material(ShaderProgram* pShader, Texture* pTexture)
{
    m_pShader = pShader;
    m_pTexture = pTexture;
}

Material::~Material()
{
}
