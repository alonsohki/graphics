/*
 * Práctica de Gráficos por computador.
 *
 * CGame.h
 * Objetivo:  Clase que maneja el juego, las naves que aparecen en él, la dificultad, el control del usuario...
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CGame;

#include "main.h"
#include "CWorld.h"
#include "CController.h"
#include "CSpaceship.h"
#include "IPulsable.h"
#include "CGameScript.h"
#include "CCrosshair.h"

class CGame : public IPulsable
{
    friend class CGameScript;

public:
    enum eZoomDistance
    {
        ZOOM_NEAR,
        ZOOM_MEDIUM,
        ZOOM_FAR
    };

public:
                                CGame                   ( );
    virtual                     ~CGame                  ( );

    void                        Start                   ( );
    void                        Stop                    ( );
    virtual void                DoPulse                 ( );

    inline CWorld&              GetWorld                ( ) { return m_world; }
    inline CCamera&             GetCamera               ( ) { return m_world.GetCamera ( ); }
    inline CCrosshair&          GetCrosshair            ( ) { return m_crosshair; }

private:
    /* Eventos para el control de la nave y la cámara */
    bool                        ControlShip             ( const SControllerEvent& evt );
    bool                        ControlCamera           ( const SControllerEvent& evt );
    bool                        ControlCameraZoom       ( const SControllerEvent& evt );

private:
    /* Funciones de interfaz para el script */
    CSpaceship*                 CreateSpaceship         ( const wstring& szModelName );
    CSpaceship*                 CreateSpaceship         ( const CModel* pModel );
    CSpaceship*                 CreatePlayerSpaceship   ( const wstring& szModelName );
    CSpaceship*                 CreatePlayerSpaceship   ( const CModel* pModel );
    void                        DeletePlayerSpaceship   ( );
public:
    CSpaceship*                 GetPlayerSpaceship      ( ) const;

    void                        SetCameraZoom           ( eZoomDistance eDistance );

private:
    CWorld                      m_world;
    CSpaceship*                 m_pPlayerShip;
    CGameScript*                m_pScript;
    CCrosshair                  m_crosshair;

    CVector                     m_vecCameraRotation;
    eZoomDistance               m_eZoom;
};
