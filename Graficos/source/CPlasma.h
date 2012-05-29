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

#pragma once

class CPlasma;

#include "CProjectile.h"

class CPlasma : public CProjectile
{
public:
    inline                  CPlasma         ( ) { }

                            CPlasma         ( const CVector& vecOrigin,
                                              const CVector& vecVelocity,
                                              const CColor& color = CColor ( 1.0f, 1.0f, 1.0f ),
                                              unsigned int uiTTL = 5000,
                                              CEntity* pCreator = 0 );
    virtual                 ~CPlasma        ( );

    void                    Create          ( const CVector& vecOrigin,
                                              const CVector& vecVelocity,
                                              const CColor& color = CColor ( 1.0f, 1.0f, 1.0f ),
                                              unsigned int uiTTL = 5000,
                                              CEntity* pCreator = 0 );


    virtual void            DoPulse         ( );

private:
    static const CModel*    s_pPlasmaModel;
};
