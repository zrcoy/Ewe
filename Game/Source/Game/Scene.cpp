#include "GamePCH.h"

#include "Game.h"
#include "Game/ResourceManager.h"
#include "Game/ObjectPool.h"
#include "Scene.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Physics2D/Box2DManager.h"

Scene::Scene(Game* pGame, ResourceManager* pResources)
{
    m_pGame = pGame;
    m_pResources = pResources;

    m_pBox2DManager = nullptr;
}

Scene::~Scene()
{
    for( auto pObject: m_pGameObjects )
    {
        delete pObject;
    }

    delete m_pBox2DManager;
}

void Scene::LoadContent()
{
}

void Scene::Reset()
{
    // Reset all of the Scene objects in the list.
    for( auto pObject: m_pGameObjects )
    {
        pObject->Reset();
    }
}

void Scene::OnEvent(Event* pEvent)
{
    // Send the event to Scene objects in the list.
    for( auto pObject: m_pGameObjects )
    {
        pObject->OnEvent( pEvent );
    }
}

void Scene::Update(float deltatime)
{
    CheckForGLErrors();

    if( m_pBox2DManager )
    {
        m_pBox2DManager->Update( deltatime );
    }

    // Update all of the Scene objects in the list.
    for( unsigned int i=0; i<m_pGameObjects.size(); i++ )
    {
        m_pGameObjects[i]->Update( deltatime );
    }

    CheckForGLErrors();
}

void Scene::Draw()
{
    CheckForGLErrors();

    CameraObject* pCamera = (CameraObject*)GetGameObjectByName( "Camera" );
    assert( pCamera );

    // Render all of the Scene objects in the list.
    for( auto pObject: m_pGameObjects )
    {
        pObject->Draw( pCamera );
    }

    CheckForGLErrors();
}

void Scene::AddGameObject(GameObject* pObject)
{
    m_pGameObjects.push_back( pObject );
}

bool Scene::RemoveGameObject(GameObject* pObject)
{
    auto iteratorForObject = std::find( m_pGameObjects.begin(), m_pGameObjects.end(), pObject );
    
    if( iteratorForObject != m_pGameObjects.end() )
    {
        m_pGameObjects.erase( iteratorForObject );
        return true;
    }

    return false;
}

GameObject* Scene::GetGameObjectByName(std::string name)
{
    for( auto pObject: m_pGameObjects )
    {
        if( pObject->GetName() == name )
        {
            return pObject;
        }
    }

    return nullptr;
}
