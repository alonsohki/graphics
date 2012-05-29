/*
 * Práctica de Gráficos por computador.
 *
 * IPulsable.h
 * Objetivo:  Interfaz para los elementos de la aplicación que recibirán pulsos.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class IPulsable;

class IPulsable
{
public:
    inline virtual      ~IPulsable  ( ) { }

    virtual void        DoPulse     ( ) = 0;
};
