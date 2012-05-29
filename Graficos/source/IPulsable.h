/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * IPulsable.h
 * Objetivo:  Interfaz para los elementos de la aplicaci�n que recibir�n pulsos.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
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
