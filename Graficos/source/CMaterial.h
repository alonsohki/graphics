/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CMaterial.h
 * Objetivo:  Contenedor de materiales.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CMaterial;

#include "CColor.h"
#include "CPixmap.h"

class CMaterial
{
private:
    CColor      m_colorAmbient;
    CColor      m_colorDiffuse;
    CColor      m_colorSpecular;
    float       m_fShininess;
    CColor      m_colorEmission;
    bool        m_bHasTexture;
    CPixmap     m_texture;

public:
    inline      CMaterial   ( const CColor& colorAmbient = CColor ( 0.2f, 0.2f, 0.2f, 1.0f ),
                              const CColor& colorDiffuse = CColor ( 0.8f, 0.8f, 0.8f, 1.0f ),
                              const CColor& colorSpecular = CColor ( 0.0f, 0.0f, 0.0f, 1.0f ),
                              float fShininess = 0.0f,
                              const CColor& colorEmission = CColor ( 0.0f, 0.0f, 0.0f, 1.0f )
                            )
        : m_colorAmbient ( colorAmbient ),
          m_colorDiffuse ( colorDiffuse ),
          m_colorSpecular ( colorSpecular ),
          m_fShininess ( fShininess ),
          m_colorEmission ( colorEmission ),
          m_bHasTexture ( false )
    {
    }


    inline void     Set     ( const CColor& colorAmbient,
                              const CColor& colorDiffuse,
                              const CColor& colorSpecular,
                              float fShininess,
                              const CColor& colorEmission
                            )
    {
        m_colorAmbient = colorAmbient;
        m_colorDiffuse = colorDiffuse;
        m_colorSpecular = colorSpecular;
        m_fShininess = fShininess;
        m_colorEmission = colorEmission;
    }

    inline void     SetAmbient      ( const CColor& colorAmbient ) { m_colorAmbient = colorAmbient; }
    inline void     SetDiffuse      ( const CColor& colorDiffuse ) { m_colorDiffuse = colorDiffuse; }
    inline void     SetSpecular     ( const CColor& colorSpecular ) { m_colorSpecular = colorSpecular; }
    inline void     SetShininess    ( float fShininess ) { m_fShininess = fShininess; }
    inline void     SetEmission     ( const CColor& colorEmission ) { m_colorEmission = colorEmission; }

    inline void     SetAlpha        ( float fAlpha )
    {
        m_colorAmbient.SetAlpha ( fAlpha );
        m_colorDiffuse.SetAlpha ( fAlpha );
        m_colorSpecular.SetAlpha ( fAlpha );
        m_colorEmission.SetAlpha ( fAlpha );
    }

    inline void     SetTexture      ( const CPixmap& texture )
    {
        if ( texture.IsOk ( ) )
        {
            m_bHasTexture = true;
            m_texture = texture;
        }
    }

    inline const CPixmap&   GetTexture      ( ) const { return m_texture; }
    inline bool             HasTexture      ( ) const { return m_bHasTexture; }
    inline const CColor&    GetAmbient      ( ) const { return m_colorAmbient; }
    inline const CColor&    GetDiffuse      ( ) const { return m_colorDiffuse; }
    inline const CColor&    GetSpecular     ( ) const { return m_colorSpecular; }
    inline float            GetShininess    ( ) const { return m_fShininess; }
    inline const CColor&    GetEmission     ( ) const { return m_colorEmission; }
};

