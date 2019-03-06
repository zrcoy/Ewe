#ifndef __MeteorObject_H__
#define __MeteorObject_H__

#include "GameObjects/GameObject.h"
#include "Game/ObjectPool.h"

class MeteorObject : public GameObject
{
protected:
    ObjectPool<GameObject*>* m_pPoolThisObjectCameFrom;

public:
    MeteorObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~MeteorObject();

    virtual void Reset() override;
    virtual bool RemoveFromScene() override;

    virtual void Update(float deltatime) override;

    void SetPool(ObjectPool<GameObject*>* pPool);

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);
};

#endif //__MeteorObject_H__
