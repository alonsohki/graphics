/*
 * Práctica de Gráficos por computador.
 *
 * CWorld.h
 * Objetivo:  Clase que maneja el mundo virtual y todos los elementos que en él se tienen.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */
#pragma once

class CWorld;

#include <list>
#include "main.h"
#include "CEntity.h"
#include "CPhysical.h"
#include "CCamera.h"
#include "CCameraFixed.h"
#include "CCameraAttached.h"
#include "CCameraCinematic.h"
#include "CLight.h"
#include "IPulsable.h"
#include "CSky.h"

class CWorld : public IPulsable
{
public:
    enum CameraMode
    {
        CAMERA_FIXED        = 0,
        CAMERA_ATTACHED     = 1,
        CAMERA_CINEMATIC    = 2,
    };

public:
                                    CWorld              ( );
                                    ~CWorld             ( );

    void                            DoPulse             ( );

    void                            Reset               ( );
    void                            RegisterEntity      ( CEntity* pEntity );
    void                            UnregisterEntity    ( CEntity* pEntity );

	const std::list< CEntity * >&   GetEntities         ( ) const;

    inline CCamera&                 GetCamera           ( ) { return *m_pActiveCamera; }
    inline const CCamera&           GetCamera           ( ) const { return *m_pActiveCamera; }
    inline void                     SetCameraMode       ( CameraMode eMode ) { m_pActiveCamera = m_pCameras [ eMode ]; }

    /* Iluminación */
    const std::vector < CLight >&   GetLights           ( ) const;
    CLight*                         CreateLight         ( );
    CLight*                         GetLight            ( unsigned int uiIndex );
    inline unsigned int             GetLightCount       ( ) const { return static_cast < unsigned int > ( m_lights.size ( ) ); }

private:
    CCamera*                        m_pCameras [ 3 ];
    CCamera*                        m_pActiveCamera;
    std::list < CEntity * >         m_listEntities;
    std::list < CPhysical * >       m_listPhysical;
    std::list < CEntity * >         m_listEntitiesToRemove;
    std::list < CEntity * >         m_listEntitiesToAdd;
    std::vector < CLight >          m_lights;
    CSky*                           m_pSky;

    /* Detección de colisiones */
    std::vector < CPhysical * >     m_vecEntityColIDs;
    VCollide                        m_vc;
};
