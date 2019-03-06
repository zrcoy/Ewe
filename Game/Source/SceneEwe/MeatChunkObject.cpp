#include "GamePCH.h"

#include "MeatChunkObject.h"
#include "Events/RemoveFromSceneEvent.h"
#include "Game/Game.h"
#include "Game/ObjectPool.h"
#include "Game/Scene.h"
#include "GameObjects/GameObject.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

MeatChunkObject::MeatChunkObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: GameObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
    m_pPoolThisObjectCameFrom = 0;
}

MeatChunkObject::~MeatChunkObject()
{
}

void MeatChunkObject::Reset()
{
    assert( m_pScene != 0 );

    RemoveFromSceneEvent* pEvent = new RemoveFromSceneEvent( this );
    m_pScene->GetGame()->GetEventManager()->QueueEvent( pEvent );
}

bool MeatChunkObject::RemoveFromScene()
{
    if( GameObject::RemoveFromScene() )
    {
        b2Body* pBody = GetBody();
        pBody->SetActive( false );
        pBody->SetLinearVelocity( b2Vec2(0,0) );
        pBody->SetAngularVelocity( 0 );
        pBody->SetTransform( b2Vec2(-10000,-10000), 0 );

        if( m_pPoolThisObjectCameFrom )
        {
            m_pPoolThisObjectCameFrom->AddNewObjectToPool( this );
        }

        return true;
    }

    return false;
}

void MeatChunkObject::Update(float deltatime)
{
    GameObject::Update( deltatime );
}

void MeatChunkObject::SetPool(ObjectPool<GameObject*>* pPool)
{
    m_pPoolThisObjectCameFrom = pPool;
}

void MeatChunkObject::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    GameObject::BeginContact( pOtherFixture, worldPosition, worldNormal );
}

void MeatChunkObject::EndContact(b2Fixture* pOtherFixture)
{
    GameObject::EndContact( pOtherFixture );
}
