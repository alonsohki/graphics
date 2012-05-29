/*
 * Práctica de Gráficos por computador.
 *
 * CLight.h
 * Objetivo:  Contenedor de luces.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CLight;

#include "CColor.h"

class CLight
{
private:
    bool                m_bEnabled;
    CColor              m_colorAmbient;
    CColor              m_colorDiffuse;
    CColor              m_colorSpecular;
    CVector             m_vecPosition;
    CVector             m_vecSpotDirection;
    float               m_fSpotExponent;
    float               m_fSpotCutoff;
    float               m_fConstantAttenuation;
    float               m_fLinearAttenuation;
    float               m_fQuadraticAttenuation;

public:
    inline              CLight              ( )
        : m_bEnabled ( false ),
          m_colorAmbient ( 0.0f, 0.0f, 0.0f, 1.0f ),
          m_colorDiffuse ( 0.0f, 0.0f, 0.0f, 1.0f ),
          m_colorSpecular ( 0.0f, 0.0f, 0.0f, 1.0f ),
          m_vecPosition ( 0.0f, 0.0f, 1.0f ),
          m_vecSpotDirection ( 0.0f, 0.0f, -1.0f ),
          m_fSpotExponent ( 0.0f ),
          m_fSpotCutoff ( 180.0f ),
          m_fConstantAttenuation ( 1.0f ),
          m_fLinearAttenuation ( 0.0f ),
          m_fQuadraticAttenuation ( 0.0f )
    {
    }

    inline void         SetEnabled              ( bool bState = true ) { m_bEnabled = bState; }
    inline bool         IsEnabled               ( ) const { return m_bEnabled; }

    inline void         SetAmbient              ( const CColor& color ) { m_colorAmbient = color; }
    inline void         SetDiffuse              ( const CColor& color ) { m_colorDiffuse = color; }
    inline void         SetSpecular             ( const CColor& color ) { m_colorSpecular = color; }
    inline void         SetPosition             ( const CVector& vec ) { m_vecPosition = vec; }
    inline void         SetSpotDirection        ( const CVector& vec ) { m_vecSpotDirection = vec; }
    inline void         SetSpotExponent         ( float fSpotExponent ) { m_fSpotExponent = fSpotExponent; }
    inline void         SetSpotCutoff           ( float fAngle ) { m_fSpotCutoff = fAngle; }
    inline void         SetConstantAttenuation  ( float fAttenuation ) { m_fConstantAttenuation = fAttenuation; }
    inline void         SetLinearAttenuation    ( float fAttenuation ) { m_fLinearAttenuation = fAttenuation; }
    inline void         SetQuadraticAttenuation ( float fAttenuation ) { m_fQuadraticAttenuation = fAttenuation; }

    inline const CColor&    GetAmbient              ( ) const { return m_colorAmbient; }
    inline const CColor&    GetDiffuse              ( ) const { return m_colorDiffuse; }
    inline const CColor&    GetSpecular             ( ) const { return m_colorSpecular; }
    inline const CVector&   GetPosition             ( ) const { return m_vecPosition; }
    inline const CVector&   GetSpotDirection        ( ) const { return m_vecSpotDirection; }
    inline float            GetSpotExponent         ( ) const { return m_fSpotExponent; }
    inline float            GetSpotCutoff           ( ) const { return m_fSpotCutoff; }
    inline float            GetConstantAttenuation  ( ) const { return m_fConstantAttenuation; }
    inline float            GetLinearAttenuation    ( ) const { return m_fLinearAttenuation; }
    inline float            GetQuadraticAttenuation ( ) const { return m_fQuadraticAttenuation; }
};
