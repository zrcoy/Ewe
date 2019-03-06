#include "GamePCH.h"

#include "SceneEwe/Weapon.h"
#include "Weapon.h"
#include "Events/RemoveFromSceneEvent.h"
#include "Game/Game.h"
#include "Game/Scene.h"

Weapon::Weapon(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
    : GameObject(pScene, name, pos, rot, scale, pMesh, pMaterial)
{

    m_GotWeapon = true;
    m_pPoolThisObjectCameFrom = 0;
}

Weapon::~Weapon()
{
}

void Weapon::Reset()
{
    //GameObject::Reset();


   // m_GotWeapon = false;

    assert(m_pScene != 0);

    RemoveFromSceneEvent* pEvent = new RemoveFromSceneEvent(this);
    m_pScene->GetGame()->GetEventManager()->QueueEvent(pEvent);
}

bool Weapon::RemoveFromScene()
{
    if (GameObject::RemoveFromScene())
    {
        b2Body* pBody = GetBody();
        pBody->SetActive(false);
        pBody->SetLinearVelocity(b2Vec2(0, 0));
        pBody->SetAngularVelocity(0);
        pBody->SetTransform(b2Vec2(-10000, -10000), 0);

        if (m_pPoolThisObjectCameFrom)
        {
            m_pPoolThisObjectCameFrom->AddNewObjectToPool(this);
        }

        return true;

        //printf("X: %f", m_pBody->GetPosition().x);
    }

    return false;
}

void Weapon::SetPool(ObjectPool<GameObject*>* pPool)
{
    m_pPoolThisObjectCameFrom = pPool;
}

void Weapon::OnEvent(Event* pEvent)
{
    
}

void Weapon::Update(float deltatime)
{
    GameObject::Update(deltatime);

    
}

void Weapon::Draw(CameraObject * pCamera)
{
    if (m_GotWeapon)
    {
        GameObject::Draw(pCamera);
    }
}

void Weapon::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    GameObject::BeginContact(pOtherFixture, worldPosition, worldNormal);

    auto other = (GameObject*)(pOtherFixture->GetUserData());

    if (other->GetName().compare(0, 6, "Meteor") == 0)
    {
        // create remove event only if hit with meteor
        Reset();
    }
    

}

void Weapon::EndContact(b2Fixture* pOtherFixture)
{
    GameObject::EndContact(pOtherFixture);
}
