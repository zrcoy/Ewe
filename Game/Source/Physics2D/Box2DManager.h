#ifndef __Box2DManager_H__
#define __Box2DManager_H__

class b2World;
class Box2DContactListener;

class Box2DManager
{
protected:
    b2World* m_pWorld;
    Box2DContactListener* m_pContactListener;

public:
    Box2DManager();
    virtual ~Box2DManager();

    void Update(float deltatime);

    b2World* GetWorld() { return m_pWorld; }
};

#endif //__Box2DManager_H__
