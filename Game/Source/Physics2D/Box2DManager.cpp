#include "GamePCH.h"

#include "Box2DManager.h"
#include "Box2DContactListener.h"
#include "../../Libraries/Box2D/Box2D/Box2D.h"

Box2DManager::Box2DManager()
{
    m_pWorld = new b2World( b2Vec2( 0, -10 ) );
    m_pContactListener = new Box2DContactListener();

    m_pWorld->SetContactListener( m_pContactListener );
}

Box2DManager::~Box2DManager()
{
    delete m_pContactListener;
    delete m_pWorld;
}

void Box2DManager::Update(float deltatime)
{
    m_pWorld->Step( 1/60.0f, 3, 8 );
}
