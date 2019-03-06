#ifndef __Mesh_H__
#define __Mesh_H__

class CameraObject;
class Material;
class Texture;
class VertexFormat;

class Mesh
{
protected:
    GLuint m_VBO;
    GLuint m_IBO;

    unsigned int m_NumVerts;
    unsigned int m_NumIndices;

    GLenum m_PrimitiveType;

public:
    Mesh();
    virtual ~Mesh();

    void Init(VertexFormat* verts, int numVerts, unsigned int* indices, int numIndices, GLenum primitiveType, GLenum usage);
    void Init(const void* verts, int numVerts, GLenum primitiveType, GLenum usage);

    void SetupAttributes(Material* pMaterial);
    void SetupUniforms(Material* pMaterial, mat4* worldMat, CameraObject* pCamera);
    void Draw(Material* pMaterial);

    // The following functions are defined in MeshShapes.cpp:
    void CreateBox(vec2 size, vec2 offset);
    void CreatePlane(vec2 worldSize, ivec2 vertexCount, vec2 pivotPoint);
    void CreateOBJ(const char* objFilename, vec3 scale);
};

#endif //__Mesh_H__
