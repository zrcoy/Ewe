#pragma once

#include "Game/ObjectPool.h"
#include "GameObjects/GameObject.h"


class Weapon : public GameObject
{
protected:
    bool m_GotWeapon;
    ObjectPool<GameObject*>* m_pPoolThisObjectCameFrom;

public:
    Weapon(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~Weapon();

    virtual void Reset() override;


    virtual void OnEvent(Event* pEvent) override;
    virtual void Update(float deltatime) override;
    virtual void Draw(CameraObject* pCamera) override;

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);

    bool GetWeaponBool() { return m_GotWeapon; }

    bool RemoveFromScene();

    void SetPool(ObjectPool<GameObject*>* pPool);
};


