/*
 * Práctica de Gráficos por computador.
 *
 * CCameraAttached.cpp
 * Objetivo:  Cámara adosada a una entidad que se mueve junto a ella.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CCameraAttached::CCameraAttached ( )
    : m_pTarget ( 0 )
{
}

CCameraAttached::~CCameraAttached ( )
{
}

CVector CCameraAttached::GetTarget ( )
{
    if ( m_pTarget )
        return m_pTarget->GetPosition ( );
    else
        return CVector ( );
}

void CCameraAttached::SetTarget ( CEntity* pEntity )
{
    m_pTarget = pEntity;
    m_timeCounter.Query ( ); // Actualizamos el contador del tiempo para la interpolación
}

CMatrix& CCameraAttached::GetMatrix ( )
{
    if ( m_pTarget )
        m_matrix = m_pTarget->GetMatrix ( ) * CCamera::GetRotationMatrix ( ) * CCamera::GetTranslationMatrix ( );
    else
        m_matrix = CCamera::GetRotationMatrix ( ) * CCamera::GetTranslationMatrix ( );

    return m_matrix;
}

CMatrix& CCameraAttached::GetRotationMatrix ( )
{
    if ( m_pTarget )
        m_matRotation = m_pTarget->GetRotationMatrix ( ) * CCamera::GetRotationMatrix ( );
    else
        m_matRotation = CCamera::GetRotationMatrix ( );

    return m_matRotation;
}

CMatrix& CCameraAttached::GetTranslationMatrix ( )
{
    if ( m_pTarget )
        m_matTranslation = m_pTarget->GetTranslationMatrix ( ) * CCamera::GetTranslationMatrix ( );
    else
        m_matTranslation = CCamera::GetTranslationMatrix ( );
    return m_matTranslation;
}

void CCameraAttached::GetTargetVectors ( CMatrix& matDest )
{
    CMatrix& matrix = GetMatrix ( );

    float fMatrix [ 16 ] = {
        0.0f, 0.0f, 0.0f, 1.0f,     // Eye
        0.0f, 0.0f, 1.0f, 1.0f,     // Target
        0.0f, 1.0f, 0.0f, 0.0f,     // Up
        0.0f, 0.0f, 0.0f, 0.0f
    };

    matDest.Set ( fMatrix );
    matDest = matrix * matDest;
}

CMatrix& CCameraAttached::GetVectors ( )
{
    CMatrix matTarget;
    GetTargetVectors ( matTarget );

    float* fVectors = matTarget.GetMatrix ( );

    /* Forzamos el target a la posición de la entidad enfocada */
    if ( m_pTarget )
    {
        const CVector& vecTarget = m_pTarget->GetPosition ( );
        fVectors [ TARGET_X ] = vecTarget.fX;
        fVectors [ TARGET_Y ] = vecTarget.fY;
        fVectors [ TARGET_Z ] = vecTarget.fZ;
    }

    /* Interpolamos hacia la posición de destino en 250 milisegundos, para darle realismo */
    const float* fCurVectors = m_matVectors.GetMatrix ( );
    float fMilisecondsSpent = m_timeCounter.Query ( ) / 1000.0f;
    CVector vecCurPosition ( fCurVectors [ EYE_X ], fCurVectors [ EYE_Y ], fCurVectors [ EYE_Z ] );
    CVector vecTargetPosition ( fVectors [ EYE_X ], fVectors [ EYE_Y ], fVectors [ EYE_Z ] );

    vecCurPosition = vecCurPosition + ( vecTargetPosition - vecCurPosition ) * fMilisecondsSpent / 250.0f ;

    fVectors [ EYE_X ] = vecCurPosition.fX;
    fVectors [ EYE_Y ] = vecCurPosition.fY;
    fVectors [ EYE_Z ] = vecCurPosition.fZ;

    m_matVectors = matTarget;

    /* Calculamos la matriz para el "look at" */
    fVectors = m_matVectors.GetMatrix ( );
    CVector vecEye ( fVectors [ EYE_X ], fVectors [ EYE_Y ], fVectors [ EYE_Z ] );
    CVector vecTarget ( fVectors [ TARGET_X ], fVectors [ TARGET_Y ], fVectors [ TARGET_Z ] );
    CVector vecUp ( fVectors [ UP_X ], fVectors [ UP_Y ], fVectors [ UP_Z ] );
    CMatrixFactory::GetLookatMatrix ( m_matLookat, vecEye, vecTarget, vecUp );

    return m_matVectors;
}

void CCameraAttached::Update ( )
{
    /* Actualiza la posición de la cámara sin interpolar */
    CMatrix matTarget;
    GetTargetVectors ( m_matVectors );
}
