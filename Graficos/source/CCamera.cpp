/*
 * Práctica de Gráficos por computador.
 *
 * CCamera.cpp
 * Objetivo:  Control de la cámara.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CCamera::CCamera ( )
{
    m_bMatrixOutdated = false;
    m_bPerspectiveOutdated = true;

    /* Generamos la matriz de vectores */
    float fMatrix [ 16 ] = {
        0.0f,       0.0f,       0.0f,       1.0f,
        0.0f,       0.0f,       1.0f,       1.0f,
        0.0f,       1.0f,       0.0f,       0.0f,
        0.0f,       0.0f,       0.0f,       1.0f
    };
    m_matVectors.Set ( fMatrix );

    /* De momento dejamos unos valores estáticos para la proyección */
    m_fNear = 30.0f;
    m_fFar = 300000.0f;
    m_fViewAngle = PI / 3.0f;
    m_fAspect = 1.0f;
}

CCamera::~CCamera ( )
{
}

void CCamera::SetPosition ( const CVector& vecPosition )
{
    CMatrixFactory::GetTranslationMatrix ( m_matTranslation, vecPosition );
    m_bMatrixOutdated = true;
}

void CCamera::SetRotation ( const CVector& vecRotation )
{
    CMatrix temp;
    CMatrixFactory::GetYRotationMatrix ( m_matRotation, vecRotation.fY );
    CMatrixFactory::GetZRotationMatrix ( temp, vecRotation.fZ );
    m_matRotation *= temp;
    CMatrixFactory::GetXRotationMatrix ( temp, vecRotation.fX );
    m_matRotation *= temp;

    m_bMatrixOutdated = true;
}

CVector CCamera::GetPosition ( )
{
    const float* fMatrix = GetVectors ( ).GetMatrix ( );    
    return CVector ( fMatrix [ EYE_X ], fMatrix [ EYE_Y ], fMatrix [ EYE_Z ] );
}

CVector CCamera::GetRotation ( )
{
    return CVector ( );
}

CVector CCamera::GetTarget ( )
{
    const float* fMatrix = GetVectors ( ).GetMatrix ( );    
    return CVector ( fMatrix [ TARGET_X ], fMatrix [ TARGET_Y ], fMatrix [ TARGET_Z ] );
}

CVector CCamera::GetUpVector ( )
{
    const float* fMatrix = GetVectors ( ).GetMatrix ( );    
    return CVector ( fMatrix [ UP_X ], fMatrix [ UP_Y ], fMatrix [ UP_Z ] );
}

CMatrix& CCamera::GetMatrix ( )
{
    if ( m_bMatrixOutdated )
    {
        m_bMatrixOutdated = false;
        m_matrix = m_matTranslation * m_matRotation;
    }

    return m_matrix;
}

CMatrix& CCamera::GetVectors ( )
{
    if ( m_bMatrixOutdated )
    {
        CMatrix& matrix = GetMatrix ( );

        float fMatrix [ 16 ] = {
              0.0f,             0.0f,             0.0f,             1.0f,       // Eye
              0.0f,             0.0f,             1.0f,             1.0f,       // Target
              0.0f,             1.0f,             0.0f,             1.0f,       // Up vector
    	      0.0f,             0.0f,             0.0f,             1.0f
        };

        m_matVectors.Set ( fMatrix );
        m_matVectors = matrix * m_matVectors;

        /* Calculamos la matriz para el "look at" */
        const float* fVectors = m_matVectors.GetMatrix ( );
        CVector vecEye ( fVectors [ EYE_X ], fVectors [ EYE_Y ], fVectors [ EYE_Z ] );
        CVector vecTarget ( fVectors [ TARGET_X ], fVectors [ TARGET_Y ], fVectors [ TARGET_Z ] );
        CVector vecUp ( fVectors [ UP_X ], fVectors [ UP_Y ], fVectors [ UP_Z ] );
        CMatrixFactory::GetLookatMatrix ( m_matLookat, vecEye, vecTarget, vecUp );
    }

    return m_matVectors;
}

void CCamera::Move ( const CVector& vector )
{
    if ( ! vector.IsNull ( ) )
    {
        CMatrix temp;
        CMatrixFactory::GetTranslationMatrix ( temp, vector );
        m_matTranslation *= temp;

        m_bMatrixOutdated = true;
    }
}

void CCamera::Rotate ( const CVector& vector )
{    
    if ( ! vector.IsNull ( ) )
    {
        CMatrix temp;
        CMatrixFactory::GetYRotationMatrix ( temp, vector.fY );
        m_matRotation *= temp;
        CMatrixFactory::GetZRotationMatrix ( temp, vector.fZ );
        m_matRotation *= temp;
        CMatrixFactory::GetXRotationMatrix ( temp, vector.fX );
        m_matRotation *= temp;

        m_bMatrixOutdated = true;
    }
}

void CCamera::SetTarget ( const CVector& vecTarget )
{
    /* TODO: Calcular el ángulo que hay entre la posición actual de la cámara
     * y el target dado, y guardarlo en m_matRotation.
     */
    m_bMatrixOutdated = true;
}

void CCamera::SetTarget ( CEntity* pEntity )
{
    if ( pEntity )
        SetTarget ( pEntity->GetPosition ( ) );
}

CMatrix& CCamera::GetPerspectiveMatrix ( )
{
    if ( m_bPerspectiveOutdated )
    {
        m_bPerspectiveOutdated = false;
        CMatrixFactory::GetPerspectiveMatrix ( m_matPerspective, m_fViewAngle, m_fAspect, m_fNear, m_fFar );
    }

    return m_matPerspective;
}
