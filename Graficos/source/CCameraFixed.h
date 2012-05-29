/*
 * Práctica de Gráficos por computador.
 *
 * CCameraFixed.h
 * Objetivo:  Cámara fija. Se establece una posición y rotación y ahí se queda hasta
 *            que cambias estos parámetros. Coincide con las rutinas de la clase CCamera
 *            básica.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CCameraFixed;

#include "CCamera.h"

class CCameraFixed : public CCamera
{
/* Nada que hacer aquí, ya que coincide con la cámara normal */
public:
    virtual void        DoPulse         ( );
};
