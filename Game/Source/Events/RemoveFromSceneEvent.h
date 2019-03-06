#ifndef __RemoveFromSceneEvent_H__
#define __RemoveFromSceneEvent_H__

#include "Events/GameEventTypes.h"
#include "../Framework/Source/Events/Event.h"

class GameObject;

class RemoveFromSceneEvent : public Event
{
protected:
    GameObject* m_pGameObject;

public:
    RemoveFromSceneEvent(GameObject* gameobject)
    {
        m_pGameObject = gameobject;
    }

    virtual EventTypes GetEventType() override { return (EventTypes)GameEventType_RemoveFromScene; }

    GameObject* GetGameObject() { return m_pGameObject; }
};

#endif //__RemoveFromSceneEvent_H__