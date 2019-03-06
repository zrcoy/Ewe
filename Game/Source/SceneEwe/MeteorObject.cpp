#include "GamePCH.h"

#include "MeteorObject.h"
#include "ShakeCameraObject.h"
#include "Events/RemoveFromSceneEvent.h"
#include "Game/Game.h"
#include "Game/ObjectPool.h"
#include "Game/Scene.h"
#include "GameObjects/GameObject.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

MeteorObject::MeteorObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: GameObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
    m_pPoolThisObjectCameFrom = 0;
}

MeteorObject::~MeteorObject()
{
}

void MeteorObject::Reset()
{
    assert( m_pScene != 0 );

    RemoveFromSceneEvent* pEvent = new RemoveFromSceneEvent( this );
    m_pScene->GetGame()->GetEventManager()->QueueEvent( pEvent );
}

bool MeteorObject::RemoveFromScene()
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

void MeteorObject::Update(float deltatime)
{
    GameObject::Update( deltatime );

    if( m_Position.y < -10 )
    {
        Reset();
    }
}

void MeteorObject::SetPool(ObjectPool<GameObject*>* pPool)
{
    m_pPoolThisObjectCameFrom = pPool;
}

void MeteorObject::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    GameObject::BeginContact( pOtherFixture, worldPosition, worldNormal );

    ((ShakeCameraObject*)m_pScene->GetGameObjectByName( "Camera" ))->Shake( 1 );
    
    Reset();
}

void MeteorObject::EndContact(b2Fixture* pOtherFixture)
{
    GameObject::EndContact( pOtherFixture );
}
