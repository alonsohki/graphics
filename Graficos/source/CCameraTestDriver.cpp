/*
 * Práctica de Gráficos por computador.
 *
 * CCameraTestDriver.cpp
 * Objetivo:  Driver para probar el movimiento de la cámara.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CCameraTestDriver::CCameraTestDriver ( )
    : m_pCamera ( 0 )
{
}

CCameraTestDriver::~CCameraTestDriver ( )
{
    if ( m_pCamera )
    {
//        CApplication::GetSingleton ( ).StopPulses ( this );
    }
}

void CCameraTestDriver::Initialize ( )
{
    CController& controller = CController::GetSingleton ( );

	/* Teclas de dirección */
    controller.BindKey ( 'q', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'e', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'w', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 's', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'a', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'd', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );

	/* Teclas de rotación */
    controller.BindKey ( 'r', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'y', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 't', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'g', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'f', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
    controller.BindKey ( 'h', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );

    controller.BindKey ( 'p', CController::DIR_BOTH, CONTROLLER_CALLBACK ( &CCameraTestDriver::KeyboardEvent, this ) );
}

void CCameraTestDriver::SetCamera ( CCamera* pCamera )
{ 
    CApplication& app = CApplication::GetSingleton ( );

    if ( m_pCamera )
    {
        app.StopPulses ( this );
    }
    m_pCamera = pCamera;
    if ( m_pCamera )
    {
        app.RequestPulses ( this );
    }
}

void CCameraTestDriver::DoPulse ( )
{
    m_pCamera->Move ( m_vecMoveDirection );
    m_pCamera->Rotate ( m_vecRotation );
}

bool CCameraTestDriver::KeyboardEvent ( const SControllerEvent& evt )
{
    static float fMovementSpeed = 0.11f;
	static float fRotationSpeed = 0.11f;
    CVector vecMoveDirection;
	CVector vecRotation;

    switch ( evt.uiKey )
    {
		/* Teclas de dirección */
        case 'e':
        {
            vecMoveDirection.fZ += fMovementSpeed;
            break;
        }

        case 'q':
        {
            vecMoveDirection.fZ -= fMovementSpeed;
            break;
        }

        case 'w':
        {
            vecMoveDirection.fY += fMovementSpeed;
            break;
        }

        case 's':
        {
            vecMoveDirection.fY -= fMovementSpeed;
            break;
        }

        case 'd':
        {
            vecMoveDirection.fX += fMovementSpeed;
            break;
        }

        case 'a':
        {
            vecMoveDirection.fX -= fMovementSpeed;
            break;
        }

		/* Teclas de rotación */
		case 'y':
		{
			vecRotation.fZ -= fRotationSpeed;
			break;
		}

		case 'r':
		{
			vecRotation.fZ += fRotationSpeed;
			break;
		}

		case 't':
		{
			vecRotation.fX += fRotationSpeed;
			break;
		}

		case 'g':
		{
			vecRotation.fX -= fRotationSpeed;
			break;
		}

		case 'h':
		{
			vecRotation.fY -= fRotationSpeed;
			break;
		}

		case 'f':
		{
			vecRotation.fY += fRotationSpeed;
			break;
		}

        case 'p':
        {
            m_pCamera->SetTarget ( CVector ( 0.0f, 0.0f, 0.0f ) );
            break;
        }
    }

    if ( evt.uiDirection == CController::DIR_UP )
    {
        vecMoveDirection *= -1;
		vecRotation *= -1;
    }

    m_vecMoveDirection += vecMoveDirection;
	m_vecRotation += vecRotation;

    return true;
}
