#include "GamePCH.h"

#include "FloatingMineObject.h"
#include "Game/Scene.h"

FloatingMineObject::FloatingMineObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: FloatingObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
    m_AttachedToPlayer = false;
}

FloatingMineObject::~FloatingMineObject()
{
}

void FloatingMineObject::Update(float deltatime)
{
    FloatingObject::Update( deltatime );
}
