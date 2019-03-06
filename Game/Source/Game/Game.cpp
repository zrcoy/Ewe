#include "GamePCH.h"

#include "Events/GameEventTypes.h"
#include "Events/RemoveFromSceneEvent.h"
#include "Game/Game.h"
#include "Game/ImguiManager.h"
#include "Game/ResourceManager.h"
#include "Game/Scene.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/PlayerController.h"
#include "Mesh/Mesh.h"
#include "Mesh/Texture.h"
#include "SceneEwe/GameEweScene.h"

Game::Game(Framework* pFramework)
: GameCore( pFramework, new EventManager() )
{
    m_pImGuiManager = nullptr;

    m_pResources = nullptr;

    for( int i=0; i<4; i++ )
    {
        m_pControllers[i] = nullptr;
    }

    m_pActiveScene = nullptr;
    m_pLoadedScene1 = nullptr;
}

Game::~Game()
{
    delete m_pLoadedScene1;

    for( int i=0; i<4; i++ )
    {
        delete m_pControllers[i];
    }

    delete m_pResources;

    delete m_pImGuiManager;
}

void Game::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    // Set OpenGL to draw to the entire window.
    glViewport( 0, 0, width, height );
}

void Game::LoadContent()
{
#if WIN32
    // Turn on V-Sync.
    wglSwapInterval( 1 );
#endif

    // Turn on depth buffer testing.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // Turn on alpha blending.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Enable back-face culling.
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CW );

    // Create our resource manager.
    m_pResources = new ResourceManager();

    // Create our meshes.
    {
        m_pResources->AddMesh( "Box", new Mesh() )->CreateBox( vec2(1,1), vec2(0,0) );
    }

    // Create our shaders.
    {
        m_pResources->AddShader( "Texture", new ShaderProgram( "Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag" ) );
    }

    // Load some textures.
    {
        m_pResources->AddTexture( "Megaman", new Texture( "Data/Textures/Megaman.png" ) );
    }

    // Create some materials.
    {
        m_pResources->AddMaterial( "Megaman", new Material( m_pResources->GetShader( "Texture" ), m_pResources->GetTexture( "Megaman" ) ) );
    }

    // Create our controllers.
    for( int i=0; i<4; i++ )
    {
        m_pControllers[i] = new PlayerController();
    }

    // Create our scenes.
    m_pLoadedScene1 = new GameEweScene( this, m_pResources );
    m_pLoadedScene1->LoadContent();

    // Set the active scene.
    m_pActiveScene = m_pLoadedScene1;

    // Create an ImGui object.
    m_pImGuiManager = new ImGuiManager();
    m_pImGuiManager->Init();

    CheckForGLErrors();
}

void Game::OnEvent(Event* pEvent)
{
    m_pImGuiManager->OnEvent( pEvent );

    m_pControllers[0]->OnEvent( pEvent );

    if( pEvent->GetEventType() == GameEventType_RemoveFromScene )
    {
        RemoveFromSceneEvent* pRemoveFromSceneEvent = (RemoveFromSceneEvent*)pEvent;

        GameObject* pGameObject = pRemoveFromSceneEvent->GetGameObject();


        pGameObject->RemoveFromScene();


    }

    m_pActiveScene->OnEvent( pEvent );

#if WIN32
    // Enable/Disable V-Sync with F1 and F2.
    if( pEvent->GetEventType() == EventType_Input )
    {
        InputEvent* pInput = (InputEvent*)pEvent;

        // Enable V-Sync.
        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == VK_F1 )
            wglSwapInterval( 1 );

        // Disable V-Sync.
        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == VK_F2 )
            wglSwapInterval( 0 );

        // Reset active scene.
        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == 'R' )
            m_pActiveScene->Reset();
    }
#endif //WIN32
}

void Game::Update(float deltatime)
{
    // Once our events are processed, tell ImGui we're starting a new frame.
    m_pImGuiManager->StartFrame( (float)m_pFramework->GetWindowWidth(), (float)m_pFramework->GetWindowHeight(), deltatime );

    m_pActiveScene->Update( deltatime );
}

void Game::Draw()
{
    // Setup the values we will clear to, then actually clear the color and depth buffers.
    glClearColor( 0.0f, 0.0f, 0.4f, 0.0f ); // (red, green, blue, alpha) - dark blue.
#if WIN32
    glClearDepth( 1 ); // 1 is maximum depth.
#endif
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw our scene.
    m_pActiveScene->Draw();

    // Tell imgui we're at the end of our frame, so it will draw all queued objects.
    m_pImGuiManager->EndFrame();

    CheckForGLErrors();
}
