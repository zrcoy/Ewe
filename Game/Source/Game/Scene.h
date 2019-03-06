#ifndef __Scene_H__
#define __Scene_H__

class Game;
class GameObject;
class ResourceManager;
class Box2DManager;

class Scene
{
private:
    std::vector<GameObject*> m_pGameObjects;

protected:
    Game* m_pGame;
    ResourceManager* m_pResources;

    Box2DManager* m_pBox2DManager;

public:
    Scene(Game* pGame, ResourceManager* pResources);
    virtual ~Scene();

    virtual void LoadContent();

    virtual void Reset();

    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltatime);
    virtual void Draw();

    void AddGameObject(GameObject* pObject);
    bool RemoveGameObject(GameObject* pObject);
    GameObject* GetGameObjectByName(std::string name);

    Game* GetGame() { return m_pGame; }
    ResourceManager* GetResourceManager() { return m_pResources; }
    Box2DManager* GetBox2DManager() { return m_pBox2DManager; }
};

#endif //__Scene_H__
