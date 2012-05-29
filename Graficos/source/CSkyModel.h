/*
 * Práctica de Gráficos por computador.
 *
 * CSkyModel.h
 * Objetivo:  Modelo del cielo. Simplemente un cubo que rodeará a la nave.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CSkyModel;

#include "CModel.h"

class CSkyModel : public CModel
{
public:
                            CSkyModel           ( );
                            ~CSkyModel          ( );

    inline bool             IsDynamic           ( ) const { return true; }

public:
    static CSkyModel*       GetSingletonPtr     ( );
    static CSkyModel&       GetSingleton        ( );
};

