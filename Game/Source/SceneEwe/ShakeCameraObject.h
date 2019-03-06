#ifndef __ShakeCameraObject_H__
#define __ShakeCameraObject_H__

#include "GameObjects/CameraObject.h"

class ShakeCameraObject : public CameraObject
{
protected:
    vec3 m_ShakeOffset;
    float m_ShakeIntensity;
    float m_TimeLeftToShake;
    float m_totaltime;
    int m_shakeDir;
public:
    ShakeCameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale);
    virtual ~ShakeCameraObject();

    virtual void Update(float deltatime) override;

    void ImplementShaking(float deltatime);

    void Shake(float intensity);
};

#endif //__ShakeCameraObject_H__
