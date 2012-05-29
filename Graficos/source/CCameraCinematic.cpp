/*
 * Práctica de Gráficos por computador.
 *
 * CCameraCinematic.cpp
 * Objetivo:  Cámara cinemática, mezcla de cámara fija y adosada. Se sitúa en un punto del espacio
 *            y mantiene su objetivo en una entidad.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"


CCameraCinematic::CCameraCinematic ( )
    : m_pTarget ( 0 )
{
}

CCameraCinematic::~CCameraCinematic ( )
{
}

CMatrix& CCameraCinematic::GetVectors ( )
{
    CMatrix& matVectors = CCamera::GetVectors ( );

    if ( m_pTarget )
    {
        CVector vecTarget = m_pTarget->GetPosition ( );
        float* fMatrix = matVectors.GetMatrix ( );

        if ( vecTarget.fX == fMatrix [ CCamera::EYE_X ] &&
             vecTarget.fY == fMatrix [ CCamera::EYE_Y ] &&
             vecTarget.fZ == fMatrix [ CCamera::EYE_Z ] )
        {
            /* La posición y el target de la cámara no pueden estar en el mismo punto */
            vecTarget.fZ += 1.0f;
        }

        fMatrix [ CCamera::TARGET_X ] = vecTarget.fX;
        fMatrix [ CCamera::TARGET_Y ] = vecTarget.fY;
        fMatrix [ CCamera::TARGET_Z ] = vecTarget.fZ;

        /* Calculamos el up vector manualmente para que no gire con la rotación de la entidad a la que enfoca */
        fMatrix [ CCamera::UP_X ] = 0.0f;
        fMatrix [ CCamera::UP_Y ] = 1.0f;
        fMatrix [ CCamera::UP_Z ] = 0.0f;

        /* Calculamos la matriz para el "look at" */
        const float* fVectors = fMatrix;
        CVector vecEye ( fVectors [ EYE_X ], fVectors [ EYE_Y ], fVectors [ EYE_Z ] );
        vecTarget = CVector ( fVectors [ TARGET_X ], fVectors [ TARGET_Y ], fVectors [ TARGET_Z ] );
        CVector vecUp ( fVectors [ UP_X ], fVectors [ UP_Y ], fVectors [ UP_Z ] );
        CMatrixFactory::GetLookatMatrix ( m_matLookat, vecEye, vecTarget, vecUp );
    }

    return matVectors;
}

CVector CCameraCinematic::GetTarget ( )
{
    if ( m_pTarget )
        return m_pTarget->GetPosition ( );
    else
        return CVector ( );
}

void CCameraCinematic::SetTarget ( CEntity* pEntity )
{
    m_pTarget = pEntity;

    if ( m_pTarget )
        DoPulse ( );
}
