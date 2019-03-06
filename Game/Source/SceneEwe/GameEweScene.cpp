#include "GamePCH.h"

#include "GameEweScene.h"
#include "MeatChunkObject.h"
#include "MeteorObject.h"
#include "ShakeCameraObject.h"
#include "SheepObject.h"
#include "Weapon.h"
#include "SheepTitleCard.h"
#include "Game/Game.h"
#include "Game/ResourceManager.h"
#include "Game/Scene.h"
#include "Physics2D/Box2DManager.h"

const float GameEweScene::INITIAL_PROJECTILE_SPAWN_DELAY = 1.0f;
const float GameEweScene::TIME_BETWEEN_PROJECTILE_SPAWNS = 0.3f;
const int GameEweScene::NUM_PROJECTILES_IN_POOL = 100;
const int GameEweScene::NUM_MEAT_CHUNKS_IN_POOL = 100;
const int GameEweScene::NUM_WEAPONS_IN_POOL = 100;


GameEweScene::GameEweScene(Game* pGame, ResourceManager* pResources)
    : Scene(pGame, pResources)
    , m_ProjectilePool()
    , m_MeatChunkPool()
    , m_WeaponPool()
    , m_WeaponPool2()
{
    m_ProjectileSpawnTimer = INITIAL_PROJECTILE_SPAWN_DELAY;
    m_GotWeapon = false;
    m_RotatorMaxSpeed = 0.f;
}

GameEweScene::~GameEweScene()
{
}

void GameEweScene::OnEvent(Event* pEvent)
{
    Scene::OnEvent(pEvent);
    // Send the event to Scene objects in the list.
    
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* pInput = (InputEvent*)pEvent;

        // enable the weapon.
        if (pInput->GetInputDeviceType() == InputDeviceType_Keyboard && pInput->GetID() == 'P')
        {
            m_GotWeapon = true;
        }
        if (pInput->GetInputDeviceType() == InputDeviceType_Mouse && pInput->GetID() == 0)      // left button id
        {
            m_RotatorMaxSpeed++;
        }
        if (pInput->GetInputDeviceType() == InputDeviceType_Mouse && pInput->GetID() == 1)      // right button id
        {
            m_RotatorMaxSpeed--;
        }
    }
}

void GameEweScene::Reset()
{
    Scene::Reset();

    m_ProjectileSpawnTimer = INITIAL_PROJECTILE_SPAWN_DELAY;
}

void GameEweScene::LoadContent()
{
    Scene::LoadContent();

    m_pBox2DManager = new Box2DManager();

    // Load textures specific to this game.  Will assert if name is already in use.
    {
        m_pResources->AddTexture("Ground", new Texture("Data/SceneEwe/Ground.png"));
        m_pResources->AddTexture("Meteor", new Texture("Data/SceneEwe/Meteor.png"));
        m_pResources->AddTexture("Sheep", new Texture("Data/SceneEwe/Sheep.png"));
        m_pResources->AddTexture("Bone", new Texture("Data/SceneEwe/Bone.png"));
        m_pResources->AddTexture("Meat", new Texture("Data/SceneEwe/Meat.png"));
        m_pResources->AddTexture("Title", new Texture("Data/SceneEwe/Title.png"));
        m_pResources->AddTexture("BG", new Texture("Data/SceneEwe/BG.png"));
        m_pResources->AddTexture("Win", new Texture("Data/SceneEwe/Win.png"));
        m_pResources->AddTexture("Lose", new Texture("Data/SceneEwe/Lose.png"));
        m_pResources->AddTexture("Kunai", new Texture("Data/SceneEwe/Kunai.png"));
        m_pResources->AddTexture("Kunai2", new Texture("Data/SceneEwe/Kunai2.png"));

    }

    // Create materials specific to this game.  Will assert if name is already in use.
    {
        m_pResources->AddMaterial("Ground", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Ground")));
        m_pResources->AddMaterial("Meteor", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Meteor")));
        m_pResources->AddMaterial("Sheep", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Sheep")));
        m_pResources->AddMaterial("Bone", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Bone")));
        m_pResources->AddMaterial("Meat", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Meat")));
        m_pResources->AddMaterial("Title", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Title")));
        m_pResources->AddMaterial("BG", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("BG")));
        m_pResources->AddMaterial("Win", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Win")));
        m_pResources->AddMaterial("Lose", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Lose")));
        m_pResources->AddMaterial("Kunai", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Kunai")));
        m_pResources->AddMaterial("Kunai2", new Material(m_pResources->GetShader("Texture"), m_pResources->GetTexture("Kunai2")));

    }

    // Create some game objects in the order we want them drawn, from back to front, due to transparency.
    {
        // Camera.
        CameraObject* pCamera = new ShakeCameraObject(this, "Camera", vec3(0, 0, -15), vec3(0, 0, 0), vec3(1, 1, 1));
        AddGameObject(pCamera);

        // BG.
        GameObject* pBG = new GameObject(this, "BG", vec3(0, 0, 1), vec3(0, 0, 0), vec3(15, 15, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("BG"));
        AddGameObject(pBG);

        // Title cards.
        GameObject* pTitle = new SheepTitleCard(this, "Title", vec3(0, 3, -1), vec3(0, 0, 0), vec3(10, 1, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Title"));
        GameObject* pWin = new SheepTitleCard(this, "Win", vec3(0, 20, -1), vec3(0, 0, 0), vec3(10, 1, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Win"));
        GameObject* pLose = new SheepTitleCard(this, "Lose", vec3(0, 20, -1), vec3(0, 0, 0), vec3(10, 1, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Lose"));

        AddGameObject(pTitle);
        AddGameObject(pWin);
        AddGameObject(pLose);

        // Player.
        float playersize = 1.0f;
        m_pPlayer = new SheepObject(this, "Player", vec3(0, -4, 0), vec3(0, 0, 0), vec3(playersize, playersize, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Sheep"));
        m_pPlayer->SetController(m_pGame->GetController(0));
        m_pPlayer->CreateBody(false);
        m_pPlayer->AddBox(vec2(playersize, playersize), 1, PhysicsCategories_Player);
        AddGameObject(m_pPlayer);
        

        // Ultimate
        //SpawnUltimate(m_pPlayer);




        // Ground
        GameObject* pGround = new GameObject(this, "Ground", vec3(0, -5, 0), vec3(0, 0, 0), vec3(10, 1, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Ground"));
        pGround->CreateBody(true);
        pGround->AddBox(vec2(10, 1), 1, PhysicsCategories_Environment);
        AddGameObject(pGround);


    }

    // Fill a pool with 100 meteors.
    {
        for (int i = 0; i < NUM_PROJECTILES_IN_POOL; i++)
        {
            float meteorSize = 0.5f;

            char tempstr[50];
            sprintf_s(tempstr, 50, "Meteor%d", i);
            MeteorObject* pMeteor = new MeteorObject(this, tempstr,
                vec3(-10000, -10000, 0), vec3(0, 0, 0), vec3(meteorSize, meteorSize, 1),
                m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Meteor"));
            pMeteor->CreateBody(false);
            pMeteor->AddCircle(meteorSize / 2, 50, PhysicsCategories_Enemy);
            pMeteor->GetBody()->SetActive(false);


            m_ProjectilePool.AddNewObjectToPool(pMeteor);
            pMeteor->SetPool(&m_ProjectilePool);
        }
        int bp = 0;
    }



    // Fill a pool with 100 random bits of meat and bone.
    {
        for (int i = 0; i < NUM_MEAT_CHUNKS_IN_POOL; i++)
        {
            float meatChunkSize = 0.5f;

            char tempstr[50];
            sprintf_s(tempstr, 50, "MeatChunk%d", i);

            Material* chunkMaterial = m_pResources->GetMaterial("Meat");
            if (rand() % 2 == 0)
                chunkMaterial = m_pResources->GetMaterial("Bone");

            MeatChunkObject* pMeatChunk = new MeatChunkObject(this, tempstr,
                vec3(-10000, -10000, 0), vec3(0, 0, 0), vec3(meatChunkSize, meatChunkSize, 1),
                m_pResources->GetMesh("Box"), chunkMaterial);
            pMeatChunk->CreateBody(false);
            pMeatChunk->AddCircle(meatChunkSize / 6, 50, PhysicsCategories_Environment);
            pMeatChunk->GetBody()->SetActive(false);


            m_MeatChunkPool.AddNewObjectToPool(pMeatChunk);
            pMeatChunk->SetPool(&m_MeatChunkPool);
        }
    }


    // Fill a pool with 100 weapons1.
    {
        for (int i = 0; i < NUM_WEAPONS_IN_POOL; i++)
        {
            float weapon_w = 2.5f;
            float weapon_h = .5f;
            //vec3 posOffset = vec3(2, 0, 0);
           // vec3 rotOffset = vec3(0, 0, 90.f);

            char tempstr[50];
            sprintf_s(tempstr, 50, "Weapon%d", i);
            Weapon* pWeapon = new Weapon(this, tempstr,
                vec3(-10000, -10000, 0), vec3(0, 0, 0), vec3(weapon_w, weapon_h, 1),
                m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Kunai"));
            pWeapon->CreateBody(false);
            pWeapon->AddBox(weapon_w, 1, PhysicsCategories_Weapon);
            pWeapon->GetBody()->SetActive(false);


            m_WeaponPool.AddNewObjectToPool(pWeapon);
            pWeapon->SetPool(&m_WeaponPool);


        }



    }


    // Fill a pool with 100 weapons2.
    {
        for (int i = 0; i < NUM_WEAPONS_IN_POOL; i++)
        {
            float weapon_w = 2.5f;
            float weapon_h = .5f;
            //vec3 posOffset = vec3(2, 0, 0);
            // vec3 rotOffset = vec3(0, 0, 90.f);

            char tempstr[50];
            sprintf_s(tempstr, 50, "Weapon%d", i);
            Weapon* pWeapon2 = new Weapon(this, tempstr,
                vec3(-10000, -10000, 0), vec3(0, 0, 0), vec3(weapon_w, weapon_h, 1),
                m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Kunai2"));
            pWeapon2->CreateBody(false);
            pWeapon2->AddCircle(weapon_w, 1, PhysicsCategories_Weapon);
            pWeapon2->GetBody()->SetActive(false);


            m_WeaponPool2.AddNewObjectToPool(pWeapon2);
            pWeapon2->SetPool(&m_WeaponPool2);


        }
    }



    // create rotator 
    SpawnRotator();

    
 
}

void GameEweScene::CreateRevoluteJoint(GameObject* objA, GameObject* objB, b2Vec2 anchorPos, bool motor, float speed, float torque)
{
    //Define joint between two bodies
    b2RevoluteJointDef jointdef;

    jointdef.Initialize(objA->GetBody(), objB->GetBody(), anchorPos);
    jointdef.enableMotor = motor;
    jointdef.motorSpeed = speed;
    jointdef.maxMotorTorque = torque;
    this->GetBox2DManager()->GetWorld()->CreateJoint(&jointdef);
}

void GameEweScene::Update(float deltatime)
{
    Scene::Update(deltatime);

    m_ProjectileSpawnTimer -= deltatime;
    if (m_ProjectileSpawnTimer < 0)
    {
        GameObject* pGameObject = m_ProjectilePool.GetObjectFromPool();

        if (pGameObject)
        {
            AddGameObject(pGameObject);

            // TODO_ICA1: Set the meteors initial position and give them a push towards the ground.
            pGameObject->SetPosition(vec3((float)(rand() % 10 + 1),(float) (rand() % 10 + 1), 0));
            float force = 100.0f;
            float randForce_X = (rand() % 21 - 10) / 10.0f;     // random force on x axis from -1 to 1 , first make it -10 to 10, then / 10.0f
            pGameObject->GetBody()->SetActive(true);
            pGameObject->GetBody()->ApplyForce(force * b2Vec2(randForce_X * 30, -1), pGameObject->GetBody()->GetWorldCenter(), true);

        }

        if (m_GotWeapon)
        {
            SpawnUltimate(m_pPlayer);
            m_GotWeapon = false;
        }
        m_ProjectileSpawnTimer = TIME_BETWEEN_PROJECTILE_SPAWNS;
    }

    

    //Control speed on rotator
    if(m_RotatorMaxSpeed>0)
    ControlRotatorSpeed(GetGameObjectByName("Rotator"));
}

void GameEweScene::Draw()
{
    Scene::Draw();

}

void GameEweScene::SpawnMeatChunks(vec3 spawnPos, unsigned int numChunks)
{
    // TODO_ICA1: Spawn a bunch of meat chunks and add forces to them, make sure they spin as they fly off.


    for (unsigned int i = 0; i < numChunks; i++)
    {

        GameObject* pMeatChunk = m_MeatChunkPool.GetObjectFromPool();
        AddGameObject(pMeatChunk);
        pMeatChunk->SetPosition(spawnPos);
        float force = 100.0f;
        float randForce_X = (rand() % 21 - 10) / 10.0f;     // random force on x axis from -1 to 1 , first make it -10 to 10, then / 10.0f
        pMeatChunk->GetBody()->ApplyForceToCenter(force * b2Vec2(randForce_X, 1 * 8), true);
        pMeatChunk->GetBody()->SetActive(true);
        pMeatChunk->GetBody()->ApplyTorque(10.0, true);
    }

}

void GameEweScene::ControlRotatorSpeed(GameObject * rotator)
{
    b2Vec2 linVel = rotator->GetBody()->GetLinearVelocity();
    linVel.Normalize();
    linVel *= m_RotatorMaxSpeed;
    rotator->GetBody()->SetLinearVelocity(linVel);
}

void GameEweScene::SpawnUltimate(SheepObject* pPlayer)
{
    //player's two spinning weapon connected with revolute joint
    float force = 100.0f;
    float weapon_w = 2.5f;
    //float weapon_h = .5f;
    vec3 posOffset = vec3(2, 5, 0);
    vec3 posOffset2 = vec3(-2, 0, 0);
    vec3 rotOffset = vec3(0, 0, 90.f);
    //GameObject* pWeapon = new Weapon(this, "Weapon", pPlayer->GetPosition() + posOffset, pPlayer->GetRotation(), vec3(weapon_w, weapon_h, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Kunai"));
    GameObject* pWeapon = m_WeaponPool.GetObjectFromPool();
    AddGameObject(pWeapon);
    pWeapon->SetPosition(pPlayer->GetPosition() + posOffset);
    pWeapon->GetBody()->SetActive(true); 
    //pWeapon->GetBody()->ApplyForceToCenter(b2Vec2(1 * force, 0), true);
   // pWeapon->GetBody()->ApplyTorque(20.0f, true);

    GameObject* pWeapon2 = m_WeaponPool2.GetObjectFromPool();
    pWeapon2->SetPosition(pWeapon->GetPosition() + posOffset);
    //pWeapon2->AddCircle(weapon_w, 1, PhysicsCategories_Player);
    AddGameObject(pWeapon2);
    pWeapon2->GetBody()->SetActive(true);
    //pWeapon2->GetBody()->ApplyForceToCenter(b2Vec2(1 * force, 0), true);
    //pWeapon2->GetBody()->ApplyTorque(20.0f, true);



   /* pWeapon->CreateBody(false);
    pWeapon->AddBox(weapon_w, 1, PhysicsCategories_Player);
    AddGameObject(pWeapon);

    Weapon* pWeapon2 = new Weapon(this, "Weapon", pWeapon->GetPosition() - posOffset, pWeapon->GetRotation() + rotOffset, vec3(weapon_w, weapon_h, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Kunai"));
    pWeapon2->CreateBody(false);
    pWeapon2->AddCircle(weapon_w, 1, PhysicsCategories_Player);
    AddGameObject(pWeapon2);*/


    //Create joint
    b2RevoluteJointDef joint;
    b2Vec2 anchor = b2Vec2((pWeapon->GetBody()->GetPosition().x + pWeapon2->GetBody()->GetPosition().x) / 2.0f, pWeapon->GetBody()->GetPosition().y);
    float randSpeed = (float)(rand() % 501 + 500);
    CreateRevoluteJoint(pWeapon, pWeapon2, anchor, true, randSpeed, randSpeed);
}


void GameEweScene::SpawnRotator()
{
    // static point
    GameObject* pSPoint = new GameObject(this, "StaticPoint", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Ground"));
    pSPoint->CreateBody(true);
    pSPoint->AddBox(vec2(1, 1), 1, PhysicsCategories_Environment);
    AddGameObject(pSPoint);
    //rotating body
    GameObject* pRotator = new GameObject(this, "Rotator",
        vec3(0, 2, 0), vec3(0, 0, 0), vec3(0.5, 0.5, 1),
        m_pResources->GetMesh("Box"), m_pResources->GetMaterial("Bone"));
    pRotator->CreateBody(false);
    pRotator->AddCircle(0.5f / 6, 50, PhysicsCategories_Environment);
    pRotator->GetBody()->SetActive(true);
    AddGameObject(pRotator);
    //Connect them using distance joint
    b2DistanceJointDef jointDef;
    jointDef.Initialize(pSPoint->GetBody(), pRotator->GetBody(), pSPoint->GetBody()->GetPosition(), pRotator->GetBody()->GetPosition());
    jointDef.collideConnected = true;
    this->GetBox2DManager()->GetWorld()->CreateJoint(&jointDef);
    // Calculate Tangent Vector
    b2Vec2 radius = pRotator->GetBody()->GetPosition() - pSPoint->GetBody()->GetPosition();
    b2Vec2 tangent = b2Vec2(radius.Skew().x* (-1.0f),radius.Skew().y* (-1.0f));// by default Skew will rotate counter clock wise, (*-1.0) make it start rotating clock wise
    tangent.Normalize();
    // Apply some force along tangent
    float acceleration = 10.0f;
    pRotator->GetBody()->ApplyForceToCenter(pRotator->GetBody()->GetMass()*acceleration*tangent, true); //applying force in the direction of the tangent
    /*float dampingVal=5;
    pRotator->GetBody()->SetLinearDamping(dampingVal);*/
}