/*
 * Práctica de Gráficos por computador.
 *
 * CCameraAttached.h
 * Objetivo:  Cámara adosada a una entidad que se mueve junto a ella.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CCameraAttached;

#include "CCamera.h"
#include "CTimeCounter.h"

class CCameraAttached : public CCamera
{
public:
                        CCameraAttached         ( );
    virtual             ~CCameraAttached        ( );

    CMatrix&            GetVectors              ( );
    CMatrix&            GetMatrix               ( );
    CMatrix&            GetRotationMatrix       ( );
    CMatrix&            GetTranslationMatrix    ( );
    inline CMatrix&     GetLookatMatrix         ( ) { GetVectors ( ); return m_matLookat; }

    void                SetTarget               ( CEntity* pEntity );
    CVector             GetTarget               ( );
    inline CEntity*     GetTargetEntity         ( ) { return m_pTarget; }

    void                Update                  ( );

private:
    void                GetTargetVectors        ( CMatrix& dest );

private:
    CEntity*            m_pTarget;
    
    CMatrix             m_matTranslation;
    CMatrix             m_matRotation;
    CMatrix             m_matrix;
    CMatrix             m_matLookat;

    CMatrix             m_matVectors;
    CTimeCounter        m_timeCounter;
};
