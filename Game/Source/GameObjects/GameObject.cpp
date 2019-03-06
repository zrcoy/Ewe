#include "GamePCH.h"

#include "Mesh/Material.h"
#include "Mesh/Mesh.h"
#include "GameObjects/CameraObject.h"
#include "GameObjects/GameObject.h"
#include "Game/Scene.h"
#include "Physics2D/Box2DManager.h"


GameObject::GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial)
: m_Name( name )
, m_pScene( pScene )
, m_pMesh( pMesh )
, m_pMaterial( pMaterial )
, m_InitialPosition( pos )
, m_InitialRotation( rot )
, m_Position( pos )
, m_Rotation( rot )
, m_Scale( scale )
, m_pBody( nullptr )
{
}

GameObject::~GameObject()
{
    if( m_pBody )
    {
        m_pScene->GetBox2DManager()->GetWorld()->DestroyBody( m_pBody );
    }
}

void GameObject::Reset()
{
    m_Position = m_InitialPosition;
    m_Rotation = m_InitialRotation;

    if( m_pBody )
    {
        m_pBody->SetLinearVelocity( b2Vec2( 0, 0 ) );
        m_pBody->SetAngularVelocity( 0 );
        m_pBody->SetTransform( b2Vec2( m_InitialPosition.x, m_InitialPosition.y ), m_InitialRotation.z * PI / 180 );
        m_pBody->SetAwake( true );
    }
}

void GameObject::OnEvent(Event* pEvent)
{
}

void GameObject::Update(float deltatime)
{
    // Sync our position to the physics body position each frame.
    if( m_pBody )
    {
        b2Vec2 pos = m_pBody->GetPosition();
        float angle = m_pBody->GetAngle();

        m_Position.x = pos.x;
        m_Position.y = pos.y;
        m_Rotation.z = -angle * 180.0f/PI;
    }
}

void GameObject::Draw(CameraObject* pCamera)
{
    if( m_pMesh != nullptr )
    {
        m_pMesh->SetupAttributes( m_pMaterial );

        // Create world matrix.
        MyMatrix worldMat;
        worldMat.CreateSRT( m_Scale, m_Rotation, m_Position );

        m_pMesh->SetupUniforms( m_pMaterial, &worldMat, pCamera );
        m_pMesh->Draw( m_pMaterial );
    }
}

void GameObject::SetPosition(vec3 pos)
{
    m_Position = pos;
    SyncPhysicsBodyToGameObject();
}

void GameObject::SetRotation(vec3 rot)
{
    m_Rotation = rot;
    SyncPhysicsBodyToGameObject();
}

void GameObject::CreateBody(bool isStatic)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set( m_Position.x, m_Position.y );
    bodyDef.angle = -m_Rotation.z * PI / 180.0f;
    if( isStatic )
        bodyDef.type = b2_staticBody;
    else
        bodyDef.type = b2_dynamicBody;
    bodyDef.active = true;
    bodyDef.userData = this;

    m_pBody = m_pScene->GetBox2DManager()->GetWorld()->CreateBody( &bodyDef );
}

void GameObject::AddFixture(b2Shape* pShape, float density, PhysicsCategories category)
{
    assert( m_pBody != nullptr );

    b2FixtureDef fixture;
    fixture.shape = pShape;
    fixture.density = density;
    fixture.friction = 1.0f;
    fixture.isSensor = false;
    fixture.restitution = 0.0f;
    fixture.userData = this;

    switch (category)
    {
    case PhysicsCategories_Default:
        fixture.filter.categoryBits = PhysicsCategories_Default;
        fixture.filter.maskBits = PhysicsCategories_Enemy | PhysicsCategories_Default | PhysicsCategories_Environment | PhysicsCategories_Player | PhysicsCategories_Weapon;
        break;
    case PhysicsCategories_Environment:
        fixture.filter.categoryBits = PhysicsCategories_Environment;
        fixture.filter.maskBits = PhysicsCategories_Enemy | PhysicsCategories_Player;
        break;
    case PhysicsCategories_Player:
        fixture.filter.categoryBits = PhysicsCategories_Player;
        fixture.filter.maskBits = PhysicsCategories_Enemy | PhysicsCategories_Environment;
        break;
    case PhysicsCategories_Weapon:
        fixture.filter.categoryBits = PhysicsCategories_Weapon;
        fixture.filter.maskBits = PhysicsCategories_Enemy | PhysicsCategories_Environment;
        break;
    case PhysicsCategories_Enemy:
        fixture.filter.categoryBits = PhysicsCategories_Enemy;    
        fixture.filter.maskBits =  PhysicsCategories_Environment | PhysicsCategories_Player | PhysicsCategories_Weapon;
        //fixture.filter.maskBits = PhysicsCategories_Weapon;
        break;
    default:
        // use for bp
        int bp = 0;
        assert(bp != 0);
        break;
    }

    m_pBody->CreateFixture( &fixture );
}

void GameObject::AddBox(vec2 size, float density, PhysicsCategories category)
{
    b2PolygonShape shape;
    shape.SetAsBox( size.x/2, size.y/2 );
    
    AddFixture( &shape, density, category);
}

void GameObject::AddCircle(float radius, float density, PhysicsCategories category)
{
    b2CircleShape shape;
    shape.m_p = b2Vec2( 0, 0 );
    shape.m_radius = radius;

    AddFixture( &shape, density, category);
}

void GameObject::SyncPhysicsBodyToGameObject()
{
    m_pBody->SetTransform( b2Vec2( m_Position.x, m_Position.y ), m_Rotation.z * PI/180.0f );
}

void GameObject::BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal)
{
    //OutputMessage( "Normal: %s, %0.2f, %0.2f\n", m_Name.c_str(), worldNormal.x, worldNormal.y );
}

void GameObject::EndContact(b2Fixture* pOtherFixture)
{
    //OutputMessage( "Normal: %s, %0.2f, %0.2f\n", m_Name.c_str(), worldNormal.x, worldNormal.y );
}

void GameObject::ReturnToPool()
{
}

bool GameObject::RemoveFromScene()
{
    return m_pScene->RemoveGameObject( this );
}
