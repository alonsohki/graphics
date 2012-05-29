/*
 * Práctica de Gráficos por computador.
 *
 * CSky.h
 * Objetivo:  Representación del cielo.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CSky;

#include "CEntity.h"
#include "CSkyModel.h"

class CSky : public CEntity
{
public:
                            CSky            ( );
                            ~CSky           ( );

    inline bool             UsesLighting    ( ) const { return false; }

    const CMatrix&          GetTranslationMatrix    ( );
    const CMatrix&          GetScalingMatrix        ( );
    const CMatrix&          GetMatrix               ( );

private:
    CMatrix                 m_matScaling;
    CMatrix                 m_matrix;
};

