/*
 * Práctica de Gráficos por computador.
 *
 * CProjectile.h
 * Objetivo:  Interfaz común de los proyectiles.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CProjectile;

#include "CEntity.h"
#include "CPhysical.h"
#include "CTimeCounter.h"

class CProjectile : public CPhysical
{
public:
                        CProjectile     ( ) { }

                        CProjectile     ( const CModel* pModel,
                                          const CVector& vecOrigin,
                                          const CVector& vecVelocity,
                                          const CColor& color = CColor ( 1.0f, 1.0f, 1.0f ),
                                          unsigned int uiTTL = 5000,
                                          CEntity* pCreator = 0 );
    virtual             ~CProjectile    ( );

    void                Create          ( const CModel* pModel,
                                          const CVector& vecOrigin,
                                          const CVector& vecVelocity,
                                          const CColor& color = CColor ( 1.0f, 1.0f, 1.0f ),
                                          unsigned int uiTTL = 5000,
                                          CEntity* pCreator = 0 );

    virtual void        DoPulse         ( );

    inline CEntity*     GetCreator      ( ) { return m_pCreator; }

private:
    CEntity*            m_pCreator;
    CTimeCounter        m_ttlCounter;
    unsigned int        m_uiTTL;
};
