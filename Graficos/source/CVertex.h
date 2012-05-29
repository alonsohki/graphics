/*
 * Práctica de Gráficos por computador.
 *
 * CVertex.h
 * Objetivo:  Contenedor de vértices.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CVertex;

#include "CVector.h"

class CVertex
{
    friend class CModel;

private:
    CVector                     m_vecPosition;
    CVector                     m_vecTexture;
    CVector                     m_vecNormal;

public:
    enum
    {
        POSITION_SIZE       = 3,
        POSITION_BEGIN      = 0,
        POSITION_STRIDE     = 36,
        TEXTURE_SIZE        = 2,
        TEXTURE_BEGIN       = 12,
        TEXTURE_STRIDE      = 36,
        NORMAL_SIZE         = 3,
        NORMAL_BEGIN        = 24,
        NORMAL_STRIDE       = 36
    };

public:
    inline                      CVertex         ( )
    {
    }

    inline                      CVertex         ( const CVertex& Right )
        : m_vecPosition ( Right.m_vecPosition ),
          m_vecTexture ( Right.m_vecTexture ),
          m_vecNormal ( Right.m_vecNormal )
    {
    }

    inline const CVector&       GetPosition     ( ) const { return m_vecPosition; }
    inline const CVector&       GetTexture      ( ) const { return m_vecTexture; }
    inline const CVector&       GetNormal       ( ) const { return m_vecNormal; }

    inline void                 SetPosition     ( const CVector& vecPosition ) { m_vecPosition = vecPosition; }
    inline void                 SetTexture      ( const CVector& vecTexture ) { m_vecTexture = vecTexture; }
    inline void                 SetNormal       ( const CVector& vecNormal ) { m_vecNormal = vecNormal; }
};
