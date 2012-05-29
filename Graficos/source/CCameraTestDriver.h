/*
 * Práctica de Gráficos por computador.
 *
 * CCameraTestDriver.h
 * Objetivo:  Driver para probar el movimiento de la cámara.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CCameraTestDriver;

#include "CCamera.h"
#include "IPulsable.h"

class CCameraTestDriver : public IPulsable
{
public:
                    CCameraTestDriver       ( );
                    ~CCameraTestDriver      ( );

    void            Initialize              ( );
    void            SetCamera               ( CCamera* pCamera );
    void            DoPulse                 ( );

private:
    bool            KeyboardEvent           ( const SControllerEvent& evt );

private:
    CCamera*        m_pCamera;
    CVector         m_vecMoveDirection;
	CVector			m_vecRotation;
};
