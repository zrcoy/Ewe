#include "GamePCH.h"

#include "ShakeCameraObject.h"
#include "Game/Scene.h"
#include "GameObjects/GameObject.h"
#include "Mesh/Mesh.h"
#include "Mesh/Material.h"

ShakeCameraObject::ShakeCameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale)
    : CameraObject(pScene, name, pos, rot, scale)
{
    m_ShakeOffset.Set(0, 0, 0);

    m_ShakeIntensity = 0;
    m_TimeLeftToShake = 0;
    m_totaltime = 0;
    m_shakeDir = 0;
}

ShakeCameraObject::~ShakeCameraObject()
{
}

void ShakeCameraObject::Shake(float intensity)
{
    m_ShakeIntensity += intensity;
    m_TimeLeftToShake = 0.05f;
}

void ShakeCameraObject::Update(float deltatime)
{
    CameraObject::Update(deltatime);

    // TODO_ICA1: Make the camera shake based on the value in m_ShakeIntensity, which is set when a meteor hits something.
    //            Make sure the camera doesn't stray from the default view of the game.
    
    ImplementShaking(deltatime);


     // Setup view matrix.
    m_ViewMatrix.CreateSRT(vec3(1), vec3(0), m_Position + m_ShakeOffset);
    m_ViewMatrix.Inverse();
}

void ShakeCameraObject::ImplementShaking(float deltatime)
{
    m_totaltime += deltatime;
    
    if (m_totaltime >= m_TimeLeftToShake)
    {
        // shake to left or right depends on shakeDir
        m_ShakeIntensity = (m_shakeDir % 2 == 0) ? m_ShakeIntensity : (-m_ShakeIntensity);
        m_shakeDir++;
        float shakeOffset = 0.05f;
        m_ShakeOffset.Set(m_ShakeIntensity*shakeOffset, m_ShakeIntensity*shakeOffset, m_ShakeIntensity*shakeOffset);
        m_ShakeIntensity = 0;
        m_totaltime = 0;

    }
}
