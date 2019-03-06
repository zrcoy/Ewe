#ifndef __SheepObject_H__
#define __SheepObject_H__

#include "GameObjects/GameObject.h"
#include "GameObjects/PlayerController.h"

class SheepObject : public GameObject
{
protected:
    bool m_CanJump;
    bool m_GotHit;
    bool m_IsDead;


    PlayerController* m_pController;

public:
    SheepObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~SheepObject();

    virtual void Reset() override;
    void SetController(PlayerController* pController);

    virtual void OnEvent(Event* pEvent) override;
    virtual void Update(float deltatime) override;

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);

    
};

#endif //__SheepObject_H__
