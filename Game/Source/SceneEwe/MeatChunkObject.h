#ifndef __MeatChunkObject_H__
#define __MeatChunkObject_H__

#include "Game/ObjectPool.h"
#include "GameObjects/GameObject.h"

class MeatChunkObject : public GameObject
{
protected:
    ObjectPool<GameObject*>* m_pPoolThisObjectCameFrom;

public:
    MeatChunkObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~MeatChunkObject();

    virtual void Reset() override;
    virtual bool RemoveFromScene() override;

    virtual void Update(float deltatime) override;

    void SetPool(ObjectPool<GameObject*>* pPool);

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);
};

#endif //__MeatChunkObject_H__
