/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CCameraTestDriver.h
 * Objetivo:  Driver para probar el movimiento de la c�mara.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
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
