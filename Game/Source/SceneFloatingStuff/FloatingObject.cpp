#include "GamePCH.h"

#include "FloatingObject.h"

FloatingObject::FloatingObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: GameObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
}

FloatingObject::~FloatingObject()
{
}

void FloatingObject::Update(float deltatime)
{
    GameObject::Update( deltatime );

    //TODO_Floating: Make the objects float on the water.

    float offsetY=0;
    float time = GetSystemTimeSinceGameStart();
    offsetY += sin(m_Position.x / 5.0 + time) * 1.0f;
    offsetY -= sin(m_Position.x / 40.0 + time) * 2.0f;

    m_Position.y = offsetY;
}
