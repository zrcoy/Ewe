#ifndef __FloatingObject_H__
#define __FloatingObject_H__

#include "GameObjects/GameObject.h"

class FloatingObject : public GameObject
{
public:

public:
    FloatingObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~FloatingObject();

    virtual void Update(float deltatime);
};

#endif //__FloatingObject_H__
