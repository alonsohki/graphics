/*
 * Práctica de Gráficos por computador.
 *
 * CPlanet.h
 * Objetivo:  Planetas.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CPlanet;

#include "CPhysical.h"

class CPlanet : public CPhysical
{
public:
                        CPlanet             ( const wstring& szTextureName, float fRadius );
                        ~CPlanet            ( );

    const CMatrix&      GetScalingMatrix    ( );
    const CMatrix&      GetMatrix           ( );

private:
    CMatrix             m_matScaling;
    CMatrix             m_matrix;
};
