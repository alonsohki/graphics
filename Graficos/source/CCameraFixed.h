/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CCameraFixed.h
 * Objetivo:  C�mara fija. Se establece una posici�n y rotaci�n y ah� se queda hasta
 *            que cambias estos par�metros. Coincide con las rutinas de la clase CCamera
 *            b�sica.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CCameraFixed;

#include "CCamera.h"

class CCameraFixed : public CCamera
{
/* Nada que hacer aqu�, ya que coincide con la c�mara normal */
public:
    virtual void        DoPulse         ( );
};
