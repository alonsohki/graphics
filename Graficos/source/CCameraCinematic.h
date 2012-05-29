/*
 * Práctica de Gráficos por computador.
 *
 * CCameraCinematic.h
 * Objetivo:  Cámara cinemática, mezcla de cámara fija y adosada. Se sitúa en un punto del espacio
 *            y mantiene su objetivo en una entidad.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CCameraCinematic;

#include "CCamera.h"

class CCameraCinematic : public CCamera
{
public:
                            CCameraCinematic    ( );
    virtual                 ~CCameraCinematic   ( );

    void                    SetTarget           ( CEntity* pEntity );
    CVector                 GetTarget           ( );
    inline CEntity*         GetTargetEntity     ( ) { return m_pTarget; }

    CMatrix&                GetVectors          ( );
    inline CMatrix&         GetLookatMatrix     ( ) { GetVectors ( ); return m_matLookat; }

private:
    CMatrix                 m_matVectors;
    CMatrix                 m_matLookat;
    CEntity*                m_pTarget;
};
