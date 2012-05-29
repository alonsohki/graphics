/*
 * Práctica de Gráficos por computador.
 *
 * CCamera.h
 * Objetivo:  Control de la cámara.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CCamera;

#include "CEntity.h"
#include "CVector.h"
#include "IPulsable.h"
#include "CMatrixFactory.h"
#include "main.h"

class CCamera : public IPulsable
{
public:
    /* Identificadores de la posición en la matriz para cada vector */
    enum
    {
        EYE_X       = 0,
        EYE_Y       = 1,
        EYE_Z       = 2,
        TARGET_X    = 4,
        TARGET_Y    = 5,
        TARGET_Z    = 6,
        UP_X        = 8,
        UP_Y        = 9,
        UP_Z        = 10
    };

public:
                                    CCamera                 ( );
    virtual                         ~CCamera                ( );

public:
    virtual void                    DoPulse                 ( ) { }

    virtual void                    SetPosition             ( const CVector& vecPosition );
    virtual void                    SetRotation             ( const CVector& vecRotation );

    virtual void                    Move                    ( const CVector& vector );
    virtual void                    Rotate                  ( const CVector& vector );

    virtual void                    SetTarget               ( const CVector& vecTarget );
    virtual void                    SetTarget               ( CEntity* pEntity );

    virtual CVector                 GetPosition             ( );
    virtual CVector                 GetRotation             ( );
    virtual CVector                 GetTarget               ( );
    virtual inline const CEntity*   GetTargetEntity         ( ) const { return 0; }
    virtual CVector                 GetUpVector             ( );

    virtual CMatrix&                GetVectors              ( );
    virtual CMatrix&                GetMatrix               ( );
    virtual inline CMatrix&         GetRotationMatrix       ( ) { return m_matRotation; }
    virtual inline CMatrix&         GetTranslationMatrix    ( ) { return m_matTranslation; }
    virtual inline CMatrix&         GetLookatMatrix         ( ) { GetVectors ( ); return m_matLookat; }


    inline float                    GetAspect               ( ) const { return m_fAspect; }
    inline float                    GetViewAngle            ( ) const { return m_fViewAngle; }
    inline float                    GetNear                 ( ) const { return m_fNear; }
    inline float                    GetFar                  ( ) const { return m_fFar; }

    inline virtual void             SetAspect               ( float fAspect ) { m_bPerspectiveOutdated = true; m_fAspect = fAspect; }
    inline virtual void             SetViewAngle            ( float fViewAngle ) { m_bPerspectiveOutdated = true; m_fViewAngle = fViewAngle; }
    inline virtual void             SetNear                 ( float fNear ) { m_bPerspectiveOutdated = true; m_fNear = fNear; }
    inline virtual void             SetFar                  ( float fFar ) { m_bPerspectiveOutdated = true; m_fFar = fFar; }
    virtual CMatrix&                GetPerspectiveMatrix    ( );

protected:
    CMatrix                 m_matVectors;
    CMatrix                 m_matRotation;
    CMatrix                 m_matTranslation;
    CMatrix                 m_matrix;
    bool                    m_bMatrixOutdated;
    CMatrix                 m_matLookat;

    float                   m_fAspect;
    float                   m_fViewAngle;
    float                   m_fNear;
    float                   m_fFar;
    CMatrix                 m_matPerspective;
    bool                    m_bPerspectiveOutdated;
};
