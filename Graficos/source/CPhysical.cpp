/*
 * Práctica de Gráficos por computador.
 *
 * CPhysical.cpp
 * Objetivo:  Físicas. No se deben instanciar objetos de esta clase diréctamente, sino
 *            objetos que hereden de ella y hagan uso de estas físicas.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CPhysical::CPhysical ( )
{
    m_bHasCollisions = true;
    m_bHasCollided = false;
    m_pColEntity = 0;
    m_iCollisionID = -1;
}

CPhysical::~CPhysical ( )
{
}

void CPhysical::ClearCollisions ( )
{
    m_bHasCollided = false;
    m_pColEntity = 0;
}

/* Aplicar las físicas dependientes del tiempo */
void CPhysical::ApplyPhysics ( )
{
    unsigned long long ullMicrosecondsSpent;
    float fTimeFactor;

    ullMicrosecondsSpent = m_timeCounter.Query ( );
    fTimeFactor = ullMicrosecondsSpent / (float)1000000.0f;

    ApplyVelocity ( fTimeFactor );
    ApplyTurnVelocity ( fTimeFactor );
    ApplyAcceleration ( fTimeFactor );
    ApplyTurnAcceleration ( fTimeFactor );

    ClearCollisions ( );
}

inline void CPhysical::ApplyAcceleration ( float fTimeFactor )
{
    m_vecVelocity += m_vecAcceleration * fTimeFactor;

    /* x' = x0 + v0*t + 0.5*a*t^2 */
    CEntity::Move ( m_vecAcceleration * ( float )( fTimeFactor * fTimeFactor / 2 ) );
}

inline void CPhysical::ApplyTurnAcceleration ( float fTimeFactor )
{
    m_vecTurnVelocity += m_vecTurnAcceleration * fTimeFactor;

    /* w' = w0 + v0*t + 0.5*a*t^2 */
    CEntity::Rotate ( ( m_vecTurnAcceleration * ( float )( fTimeFactor * fTimeFactor / 2 ) ) % PI2 );
}

inline void CPhysical::ApplyVelocity ( float fTimeFactor )
{
    CEntity::Move ( m_vecVelocity * fTimeFactor );
}

inline void CPhysical::ApplyTurnVelocity ( float fTimeFactor )
{
    CEntity::Rotate ( ( m_vecTurnVelocity * fTimeFactor ) % PI2 );
}


/* Aceleración lineal */
void CPhysical::SetAcceleration ( float fAccel, const CVector& vecDirection )
{
    if ( fAccel == 0.0f )
    {
        m_vecAcceleration = CVector ( );
    }

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecAcceleration.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }
    }
    else
    {
        m_vecAcceleration = vecDirection;
    }

    m_vecAcceleration.Normalize ( );
    m_vecAcceleration *= fAccel;
}

void CPhysical::AddAcceleration ( float fAccel, const CVector& vecDirection )
{
    CVector vecNewDirection;

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecAcceleration.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }

        vecNewDirection = m_vecAcceleration;
    }
    else
    {
        vecNewDirection = vecDirection;
    }

    vecNewDirection.Normalize ( );
    vecNewDirection *= fAccel;

    m_vecAcceleration += vecNewDirection;
}

CVector CPhysical::GetAcceleration ( ) const
{
    return m_vecAcceleration;
}


/* Velocidad lineal */
void CPhysical::SetVelocity ( float fVelocity, const CVector& vecDirection )
{
    if ( fVelocity == 0.0f )
    {
        m_vecVelocity = CVector ( );
        return;
    }

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecVelocity.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }
    }
    else
    {
        m_vecVelocity = vecDirection;
    }

    m_vecVelocity.Normalize ( );
    m_vecVelocity *= fVelocity;
}

void CPhysical::AddVelocity ( float fVelocity, const CVector& vecDirection )
{
    CVector vecNewDirection;

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecVelocity.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }

        vecNewDirection = m_vecVelocity;
    }
    else
    {
        vecNewDirection = vecDirection;
    }

    vecNewDirection.Normalize ( );
    vecNewDirection *= fVelocity;

    m_vecVelocity += vecNewDirection;
}

CVector CPhysical::GetVelocity ( ) const
{
    return m_vecVelocity;
}


/* Aceleración angular */
void CPhysical::SetTurnAcceleration ( float fAccel, const CVector& vecDirection )
{
    if ( fAccel == 0.0f )
    {
        m_vecTurnAcceleration = CVector ( );
        return;
    }

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecTurnAcceleration.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }
    }
    else
    {
        m_vecTurnAcceleration = vecDirection;
    }

    m_vecTurnAcceleration.Normalize ( );
    m_vecTurnAcceleration *= fAccel;
}

void CPhysical::AddTurnAcceleration ( float fAccel, const CVector& vecDirection )
{
    CVector vecNewDirection;

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecTurnAcceleration.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }

        vecNewDirection = m_vecTurnAcceleration;
    }
    else
    {
        vecNewDirection = vecDirection;
    }

    vecNewDirection.Normalize ( );
    vecNewDirection *= fAccel;

    m_vecTurnAcceleration += vecNewDirection;
}

CVector CPhysical::GetTurnAcceleration ( ) const
{
    return m_vecTurnAcceleration;
}


/* Velocidad angular */
void CPhysical::SetTurnVelocity ( float fVelocity, const CVector& vecDirection )
{
    if ( fVelocity == 0.0f )
    {
        m_vecTurnVelocity = CVector ( );
        return;
    }

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecTurnVelocity.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }
    }
    else
    {
        m_vecTurnVelocity = vecDirection;
    }

    m_vecTurnVelocity.Normalize ( );
    m_vecTurnVelocity *= fVelocity;
}

void CPhysical::AddTurnVelocity ( float fVelocity, const CVector& vecDirection )
{
    CVector vecNewDirection;

    /* Si no nos dan un vector de dirección, usamos la dirección actual */
    if ( vecDirection.IsNull ( ) )
    {
        if ( m_vecTurnVelocity.IsNull ( ) )
        {
            /* No nos han dado ningún vector válido, no hay nada que hacer */
            return;
        }

        vecNewDirection = m_vecTurnVelocity;
    }
    else
    {
        vecNewDirection = vecDirection;
    }

    vecNewDirection.Normalize ( );
    vecNewDirection *= fVelocity;

    m_vecTurnVelocity += vecNewDirection;
}

CVector CPhysical::GetTurnVelocity ( ) const
{
    return m_vecTurnVelocity;
}
