#ifndef __SheepTitleCard_H__
#define __SheepTitleCard_H__

#include "GameObjects/GameObject.h"

class SheepTitleCard : public GameObject
{
protected:
    float m_TimeAlive;

public:
    SheepTitleCard(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~SheepTitleCard();

    virtual void Reset() override;

    virtual void Update(float deltatime) override;

    void Display();
};

#endif //__SheepTitleCard_H__
