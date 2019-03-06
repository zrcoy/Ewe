#ifndef __GameEweScene_H__
#define __GameEweScene_H__

#include "Game/ObjectPool.h"
#include "Game/Scene.h"

enum PhysicsCategories
{
    PhysicsCategories_Default = 0x0001,
    PhysicsCategories_Environment = 0x0002,
    PhysicsCategories_Player = 0x0004,
    PhysicsCategories_Weapon = 0x0008,
    PhysicsCategories_Enemy = 0x0010,
};

class SheepObject;

class GameEweScene : public Scene
{
    static const float INITIAL_PROJECTILE_SPAWN_DELAY;
    static const float TIME_BETWEEN_PROJECTILE_SPAWNS;
    static const int NUM_PROJECTILES_IN_POOL;
    static const int NUM_MEAT_CHUNKS_IN_POOL;
    static const int NUM_WEAPONS_IN_POOL;


protected:
    ObjectPool<GameObject*> m_ProjectilePool;
    ObjectPool<GameObject*> m_MeatChunkPool;
    ObjectPool<GameObject*> m_WeaponPool;
    ObjectPool<GameObject*> m_WeaponPool2;

    float m_ProjectileSpawnTimer;

    SheepObject* m_pPlayer;
    bool m_GotWeapon;

    float m_RotatorMaxSpeed;

public:
    GameEweScene(Game* pGame, ResourceManager* pResources);
    virtual ~GameEweScene();

    virtual void LoadContent() override;

    virtual void Reset() override;
    virtual void OnEvent(Event* pEvent) override;
    virtual void Update(float deltatime) override;
    virtual void Draw() override;

    SheepObject* GetPlayer() { return m_pPlayer; }

    void SpawnMeatChunks(vec3 spawnPos, unsigned int numChunks);

    void ControlRotatorSpeed(GameObject* rotator);

    void SpawnUltimate(SheepObject* pPlayer);
    void CreateRevoluteJoint(GameObject* objA, GameObject* objB, b2Vec2 anchorPos, bool motor=false, float speed=30.0f, float torque=30.0f);

    void SpawnRotator();
};

#endif //__Scene_H__
