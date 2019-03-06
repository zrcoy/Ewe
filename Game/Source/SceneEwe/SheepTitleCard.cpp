#include "GamePCH.h"

#include "SheepTitleCard.h"
#include "Game/Scene.h"
#include "GameObjects/GameObject.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

SheepTitleCard::SheepTitleCard(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: GameObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
    m_TimeAlive = 0.0f;
}

SheepTitleCard::~SheepTitleCard()
{
}

void SheepTitleCard::Reset()
{
    GameObject::Reset();

    m_TimeAlive = 0.0f;
}

void SheepTitleCard::Update(float deltatime)
{
    GameObject::Update( deltatime );

    m_TimeAlive += deltatime;

    if( m_TimeAlive > 0.5f )
    {
        m_Position.y += deltatime*4;
    }
}

void SheepTitleCard::Display()
{
    m_Position.y = 3.0f;
    m_TimeAlive = 0.0f;
}
