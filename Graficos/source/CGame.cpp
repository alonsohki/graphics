/*
 * Práctica de Gráficos por computador.
 *
 * CGame.cpp
 * Objetivo:  Clase que maneja el juego, las naves que aparecen en él, la dificultad, el control del usuario...
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"


CGame::CGame ( )
    : m_pPlayerShip ( 0 ),
      m_pScript ( 0 )
{
    /* Asignamos las teclas para controlar la nave */
    CController& controller = CController::GetSingleton ( );
    controller.BindKey ( 'q', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 'w', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 'e', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 'a', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 's', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 'd', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 'r', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindKey ( 'f', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );

    controller.BindMouseButton ( 0, CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindMouseButton ( 2, CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CGame::ControlShip, this ) );
    controller.BindMouseMotion ( CONTROLLER_CALLBACK ( &CGame::ControlCamera, this ) );

    /* Asignamos las teclas para controlar el zoom de la cámara */
    controller.BindSpecialKey ( CController::SPECIAL_START, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CGame::ControlCameraZoom, this ) );

    m_eZoom = ZOOM_MEDIUM;
}

CGame::~CGame ( )
{
}

/* Control de la nave y la cámara */
bool CGame::ControlShip ( const SControllerEvent& evt )
{
    bool bDown = evt.uiDirection == CController::DIR_DOWN ? true : false;

    if ( m_pPlayerShip )
    {
        if ( evt.eType == SControllerEvent::KEY )
        {
            switch ( evt.uiKey )
            {
                case 'f':
                    bDown = !bDown;
                case 'r':
                    m_pPlayerShip->Accelerate ( bDown );
                    break;
                case 'e':
                    bDown = !bDown;
                case 'q':
                    m_pPlayerShip->TurnYaw ( bDown );
                    break;
                case 's':
                    bDown = !bDown;
                case 'w':
                    m_pPlayerShip->TurnPitch ( bDown );
                    break;
                case 'a':
                    bDown = !bDown;
                case 'd':
                    m_pPlayerShip->TurnRoll ( bDown );
                    break;
            }
        }
        else if ( evt.eType == SControllerEvent::MOUSE )
        {
            switch ( evt.uiButton )
            {
                case 0:
                    m_pPlayerShip->Shoot ( bDown );
                    break;
                case 2:
                    m_pPlayerShip->SpecialShoot ( bDown );
                    break;
            }
        }
    }

    return true;
}

bool CGame::ControlCamera ( const SControllerEvent& evt )
{
    static float fPI4 = PI/4;

    try
    {
        CCameraAttached& camera = dynamic_cast < CCameraAttached& > ( CGame::GetWorld ( ).GetCamera ( ) );

        m_vecCameraRotation.fY -= evt.iX / 500.0f;
        m_vecCameraRotation.fX += evt.iY / 500.0f;

        if ( m_vecCameraRotation.fX > fPI4 )
            m_vecCameraRotation.fX = fPI4;
        else if ( m_vecCameraRotation.fX < -fPI4 )
            m_vecCameraRotation.fX = -fPI4;

        camera.SetRotation ( m_vecCameraRotation );
    }
    catch ( ... ) { }

    return true;
}

bool CGame::ControlCameraZoom ( const SControllerEvent& evt )
{
    if ( m_pPlayerShip )
    {
        float fRadius = m_pPlayerShip->GetModel ( )->GetRadius ( );

        try
        {
            CCameraAttached& camera = dynamic_cast < CCameraAttached& > ( CGame::GetWorld ( ).GetCamera ( ) );

            switch ( m_eZoom )
            {
                case ZOOM_NEAR:
                {
                    camera.SetPosition ( CVector ( 0.0f, fRadius / 1.8f, fRadius * -1.85f ) );
                    m_eZoom = ZOOM_MEDIUM;
                    break;
                }

                case ZOOM_MEDIUM:
                {
                    camera.SetPosition ( CVector ( 0.0f, fRadius / 1.8f, fRadius * -3.1f ) );
                    m_eZoom = ZOOM_FAR;
                    break;
                }

                case ZOOM_FAR:
                {
                    camera.SetPosition ( CVector ( 0.0f, fRadius / 1.8f, fRadius * -1.2f ) );
                    m_eZoom = ZOOM_NEAR;
                    break;
                }
            }
        }
        catch ( ... ) { }
    }

    return true;
}

void CGame::SetCameraZoom ( eZoomDistance eDistance )
{
    /* Procedemos a establecer el zoom simulando que pulsan la tecla para cambiarlo */
    switch ( eDistance )
    {
        case ZOOM_NEAR: m_eZoom = ZOOM_FAR; break;
        case ZOOM_MEDIUM: m_eZoom = ZOOM_NEAR; break;
        case ZOOM_FAR: m_eZoom = ZOOM_MEDIUM; break;
    }

    SControllerEvent evt;
    ControlCameraZoom ( evt );
}

void CGame::DoPulse ( )
{
    m_crosshair.DoPulse ( );
    if ( m_pPlayerShip && m_pPlayerShip->IsShooting ( ) )
    {
        /* Calculamos la dirección a la que debe disparar */
        CVector vecTarget;
        if ( m_crosshair.GetTarget ( vecTarget, 0 ) )
            m_pPlayerShip->SetShootTarget ( vecTarget );
    }

    m_world.DoPulse ( );

    if ( m_pScript != 0 )
    {
        m_pScript->DoPulse ( );
    }
}

void CGame::Start ( )
{
    if ( m_pScript == 0 )
    {
        m_pScript = new CGameScript ( this );
        m_pScript->Start ( );
    }

    m_vecCameraRotation = CVector ( );
}

void CGame::Stop ( )
{
    if ( m_pScript != 0 )
    {
        m_pScript->Stop ( );
        delete m_pScript;
        m_pScript = 0;
    }

    if ( m_pPlayerShip != 0 )
    {
        m_pPlayerShip = 0;
    }

    m_world.Reset ( );
}



/* Funciones de interfaz para el script */
CSpaceship* CGame::CreateSpaceship ( const CModel* pModel )
{
    CSpaceship* pShip = new CSpaceship ( pModel );
    if ( ! pShip->IsOk ( ) )
    {
        pShip->Destroy ( );
        return 0;
    }

    return pShip;
}

CSpaceship* CGame::CreateSpaceship ( const wstring& szModelName )
{
    const CModel* pModel = CModelManager::GetSingleton ( ).GetModel ( szModelName );
    return CreateSpaceship ( pModel );
}

CSpaceship* CGame::CreatePlayerSpaceship ( const CModel* pModel )
{
    DeletePlayerSpaceship ( );
    m_pPlayerShip = CreateSpaceship ( pModel );
    return m_pPlayerShip;
}

CSpaceship* CGame::CreatePlayerSpaceship ( const wstring& szModelName )
{
    DeletePlayerSpaceship ( );
    m_pPlayerShip = CreateSpaceship ( szModelName );
    return m_pPlayerShip;
}

CSpaceship* CGame::GetPlayerSpaceship ( ) const
{
    return m_pPlayerShip;
}

void CGame::DeletePlayerSpaceship ( )
{
    if ( m_pPlayerShip )
    {
        m_world.UnregisterEntity ( m_pPlayerShip );
        m_pPlayerShip = 0;
    }
}
