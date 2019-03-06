#ifndef __Material_H__
#define __Material_H__

class ShaderProgram;
class Texture;

class Material
{
protected:
    ShaderProgram* m_pShader;
    Texture* m_pTexture;

public:
    Material(ShaderProgram* pShader, Texture* pTexture);
    virtual ~Material();

    ShaderProgram* GetShader() { return m_pShader; }
    Texture* GetTexture() { return m_pTexture; }
};

#endif //__Material_H__
