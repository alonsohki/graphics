/*
 * Práctica de Gráficos por computador.
 *
 * CProjectile.cpp
 * Objetivo:  Interfaz común de los proyectiles.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CProjectile::CProjectile ( const CModel* pModel,
                           const CVector& vecOrigin,
                           const CVector& vecVelocity,
                           const CColor& color,
                           unsigned int uiTTL,
                           CEntity* pCreator)
{
    Create ( pModel, vecOrigin, vecVelocity, color, uiTTL, pCreator );
}

void CProjectile::Create ( const CModel* pModel,
                           const CVector& vecOrigin,
                           const CVector& vecVelocity,
                           const CColor& color,
                           unsigned int uiTTL,
                           CEntity* pCreator )
{
    CEntity::SetModel ( pModel );
    CEntity::SetPosition ( vecOrigin );
    CPhysical::SetVelocity ( vecVelocity.Module ( ), vecVelocity );

    CEntityModel* pEntityModel = CEntity::GetEntityModel ( );
    if ( pEntityModel && pEntityModel->GetNumChildren ( ) > 0 )
    {
        pEntityModel = pEntityModel->GetChild ( 0 );
        pEntityModel->GetMaterial ( ).SetEmission ( color );
    }

    m_pCreator = pCreator;
    m_uiTTL = uiTTL;
    m_ttlCounter.Query ( );
}

CProjectile::~CProjectile ( )
{
}

void CProjectile::DoPulse ( )
{
    unsigned long ulMilisecondsSpent = m_ttlCounter.Query ( false ) / 1000;

    if ( ulMilisecondsSpent >= m_uiTTL )
    {
        CEntity::Destroy ( );
    }
    else
    {
        CPhysical::DoPulse ( );
    }
}
