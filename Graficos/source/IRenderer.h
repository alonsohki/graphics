/*
 * Práctica de Gráficos por computador.
 *
 * IRenderer.h
 * Objetivo:  Interfaz para los renderers.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class IRenderer;

#include "CWorld.h"

class IRenderer
{
public:
    enum RenderingMode
    {
        RENDER_TRIANGLES,
        RENDER_WIREFRAME,
        RENDER_CLASSICAL
    };

public:
    inline virtual          ~IRenderer          ( ) { }

    virtual bool            Initialize          ( ) = 0;
    virtual const char*     GetErrorMessage     ( ) = 0;

    virtual void            SetRenderingMode    ( RenderingMode eMode ) = 0;

    virtual void            BeginScene          ( ) = 0;
    virtual void            Render              ( CWorld& world ) = 0;
    virtual void            EndScene            ( ) = 0;
};
