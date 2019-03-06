#include "GamePCH.h"

#include "GameEweScene.h"
#include "SheepObject.h"
#include "SheepTitleCard.h"
#include "Game/Scene.h"
#include "GameObjects/GameObject.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

SheepObject::SheepObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: GameObject( pScene, name, pos, rot, scale, pMesh, pMaterial )
{
    m_pController = 0;

    m_GotHit = false;
 
    m_IsDead = false;
}

SheepObject::~SheepObject()
{
}

void SheepObject::Reset()
{
    GameObject::Reset();

    m_GotHit = false;
    m_IsDead = false;

}

void SheepObject::SetController(PlayerController* pController)
{
    m_pController = pController;
}

void SheepObject::OnEvent(Event* pEvent)
{
    if( pEvent->GetEventType() == EventType_Input )
    {
        InputEvent* pInput = (InputEvent*)pEvent;

        // Debug: Kill the player if 'K' is pressed.
        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == 'K' )
        {
            m_GotHit = true;
        }
        // enable the weapon.
  
    }
}

void SheepObject::Update(float deltatime)
{
    GameObject::Update( deltatime );

    if( m_pController == 0 )
        return;

    // Handle end game states.
    if( m_IsDead == true )
    {
        ((SheepTitleCard*)m_pScene->GetGameObjectByName( "Lose" ))->Display();
        return;
    }

    if( m_Position.y < -5 )
    {
        ((SheepTitleCard*)m_pScene->GetGameObjectByName( "Win" ))->Display();
    }

    // Handle getting hit.
    if( m_GotHit )
    {
        // TODO_ICA1: Start the meat chunk spawning.
        
        ((GameEweScene*)(m_pScene))->SpawnMeatChunks(m_Position, 30);
        //static_cast<GameEweScene*>(m_pScene)->SpawnMeatChunks(m_Position, 30);

        m_IsDead = true;

        m_pBody->SetTransform( b2Vec2( 10000, 0 ), 0 );

        return;
    }

    vec2 dir(0, 0);
    // TODO_ICA1: Make the SheepObject make small jumps to the left or right based on input from the player controller object.
    if (m_pController->IsHeld_Left())
    {
        if (m_CanJump)
        {
            dir.x = -1.0f;
            dir.y = 1;
        }
        m_CanJump = false;
    }
    if (m_pController->IsHeld_Right())
    {
        if (m_CanJump)
        {
            dir.x = 1.0f;
            dir.y = 1;
        }
        m_CanJump = false;
    }
    float force = 50.0f;
    m_pBody->ApplyForce(b2Vec2(force*dir.x, force*dir.y*5), m_pBody->GetWorldCenter(), true);
}

void SheepObject::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    GameObject::BeginContact( pOtherFixture, worldPosition, worldNormal );

    if( worldNormal.y < 0.5f )
        m_CanJump = true;

    // TODO_ICA1: Set m_GotHit to true if the sheep is hit by a meteor.
        auto other = (GameObject*)(pOtherFixture->GetUserData());

        if (other->GetName().compare(0,6,"Meteor") == 0)
        {
            // get hit by one of the meat chunk
            m_GotHit = true;
        }
    
   

}

void SheepObject::EndContact(b2Fixture* pOtherFixture)
{
    GameObject::EndContact( pOtherFixture );
}
