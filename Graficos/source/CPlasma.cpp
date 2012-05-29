/*
 * Práctica de Gráficos por computador.
 *
 * CPlasma.h
 * Objetivo:  Rayos de plasma, el arma más típica.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

const CModel* CPlasma::s_pPlasmaModel = 0;

CPlasma::CPlasma ( const CVector& vecOrigin,
                   const CVector& vecVelocity,
                   const CColor& color,
                   unsigned int uiTTL,
                   CEntity* pCreator )
{
    Create ( vecOrigin, vecVelocity, color, uiTTL, pCreator );
}

CPlasma::~CPlasma ( )
{
}

void CPlasma::Create ( const CVector& vecOrigin,
                       const CVector& vecVelocity,
                       const CColor& color,
                       unsigned int uiTTL,
                       CEntity* pCreator )
{
    if ( s_pPlasmaModel == 0 )
    {
        s_pPlasmaModel = CModelManager::GetSingleton ( ).GetModel ( L"plasma" );
    }

    if ( s_pPlasmaModel )
    {
        CProjectile::Create ( s_pPlasmaModel, vecOrigin, vecVelocity, color, uiTTL, pCreator );
    }
}

void CPlasma::DoPulse ( )
{
    CProjectile::DoPulse ( );

    if ( CPhysical::HasCollided ( ) && CPhysical::GetColEntity ( ) != CProjectile::GetCreator ( ) )
    {
        CEntity::Destroy ( );
    }
}

