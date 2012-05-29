/*
 * Práctica de Gráficos por computador.
 *
 * CCrosshair.h
 * Objetivo:  Diana para apuntar a la hora de disparar.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

#define CROSSHAIR_X 0.60f
#define CROSSHAIR_Y 0.36f

class CCrosshair;

class CCrosshair
{
public:
                    CCrosshair      ( );
                    ~CCrosshair     ( );

    void            DoPulse         ( );
    void            Render          ( );


    bool            GetTarget       ( CVector& vecPosition, CEntity** ppEntity = 0 );

private:
    int             m_iScreenWidth;
    int             m_iScreenHeight;
    int             m_iCrosshairX;
    int             m_iCrosshairY;
    unsigned int    m_uiDisplayList;
};
