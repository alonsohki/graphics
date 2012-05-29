/*
 * Práctica de Gráficos por computador.
 *
 * CGameScript.cpp
 * Objetivo:  Guión del juego.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CGameScript::CGameScript ( )
    : m_uiLevel ( 0 )
{
}

CGameScript::CGameScript ( CGame* pGame )
{
    SetGame ( pGame );
}

CGameScript::~CGameScript ( )
{
}

void CGameScript::DoPulse ( )
{
    if ( m_uiLevel > 0 )
    {
        PulseRef_pfn pulse = m_levelPulses [ m_uiLevel - 1 ];
        (this->*pulse) ( );
    }
}

void CGameScript::SetGame ( CGame* pGame )
{
    m_pGame = pGame;
}

void CGameScript::Start ( )
{
    if ( m_pGame )
    {
        CreateLevel1 ( );
    }
}

void CGameScript::Stop ( )
{
    m_uiLevel = 0;
}


/* Nivel 1 */
void CGameScript::CreateLevel1 ( )
{
    m_uiLevel = 1;
    m_levelPulses [ 0 ] = &CGameScript::PulseLevel1;

    CSpaceship* pPlayerShip = m_pGame->CreatePlayerSpaceship ( L"daedalus" );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 200.0f, 0.0f, 1250.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( -200.0f, 0.0f, -1250.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 0.0f, 400.0f, 850.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( -500.0f, -500.0f, -100.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 800.0f, 100.0f, -100.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 1200.0f, -100.0f, 750.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 10000.0f, 0.0f, 0.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 0.0f, 10000.0f, 0.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 0.0f, 0.0f, -10000.0f ) );
    m_pGame->CreateSpaceship ( L"hatak" )->SetPosition ( CVector ( 6000.0f, 0.0f, 850.0f ) );

    CPlanet* pPlanet = new CPlanet ( L"earth", 5000.0f );
    pPlanet->Move ( CVector ( 0.0f, 0.0f, 9000.0f ) );
    pPlanet->SetTurnVelocity ( PI/50, CVector ( 0.0f, 1.0f, 0.0f ) );

    if ( pPlayerShip )
    {
        CWorld& world = m_pGame->GetWorld ( );
        world.SetCameraMode ( CWorld::CAMERA_ATTACHED );

        m_pGame->SetCameraZoom ( CGame::ZOOM_MEDIUM );

        try
        {
            /* Evitamos la interpolación de la posición de la cámara */
            CCameraAttached& camera = dynamic_cast < CCameraAttached& > ( m_pGame->GetCamera ( ) );
            camera.SetTarget ( pPlayerShip );
            camera.Update ( );
        }
        catch ( ... ) { }
    }
}

void CGameScript::DestroyLevel1 ( )
{
    m_uiLevel = 0;
}

void CGameScript::PulseLevel1 ( )
{
}
