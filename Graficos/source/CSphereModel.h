/*
 * Práctica de Gráficos por computador.
 *
 * CSphereModel.h
 * Objetivo:  Representa el modelo de una esfera de radio 1.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */ 


#pragma once

class CSphereModel;

#include "CModel.h"

class CSphereModel : public CModel
{
public:
                            CSphereModel            ( const unsigned int slices, const unsigned int stacks );
                            ~CSphereModel           ( ) { };
        inline bool         IsDinamyc               ( ) const { return true; }

private:
        void                MakeModel               ( const unsigned int slices, const unsigned int stacks );
                            
                            
};