#ifndef __GameObject_H__
#define __GameObject_H__

class Game;
class Material;
class Scene;
class Mesh;
class b2Body;
class CameraObject;
#include "SceneEwe/GameEweScene.h"

class GameObject
{
protected:
    std::string m_Name;

    Scene* m_pScene;

    Mesh* m_pMesh;
    Material* m_pMaterial;

    vec3 m_InitialPosition;
    vec3 m_InitialRotation;

    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;

    b2Body* m_pBody;

public:
    GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Mesh* pMesh, Material* pMaterial);
    virtual ~GameObject();

    virtual void Reset();

    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltatime);
    virtual void Draw(CameraObject* pCamera);

    // Getters.
    std::string GetName() { return m_Name; }
    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    b2Body* GetBody() { return m_pBody; }

    // Setters.
    void SetPosition(vec3 pos);
    void SetRotation(vec3 rot);

    // Physics helpers.
    void CreateBody(bool isStatic);
    void AddFixture(b2Shape* pShape, float density, PhysicsCategories category);
    void AddBox(vec2 size, float density, PhysicsCategories category);
    void AddCircle(float radius, float density, PhysicsCategories category);

    void SyncPhysicsBodyToGameObject();

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);

    // Other helpers.
    virtual void ReturnToPool();
    virtual bool RemoveFromScene();
};

#endif //__GameObject_H__
