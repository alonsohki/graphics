/*
 * Práctica de Gráficos por computador.
 *
 * CEntity.h
 * Objetivo:  Entidades que poseen atributos comunes: posición, rotación, renderizables...
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CEntity;

#include "CVector.h"
#include "CModel.h"
#include "CEntityModel.h"
#include "CPool.h"
#include "IPulsable.h"

class CEntity : public IPulsable
{
    friend class CWorld;

public:
                                CEntity                 ( );
                                CEntity                 ( const CModel* model );
protected:
    virtual                     ~CEntity                ( );
public:
    virtual void                DoPulse                 ( ) { }

    void                        Destroy                 ( );
    bool                        IsValid                 ( ) { return !m_bIsDestroyed; }

    /* Modelos */
    void                        SetModel                ( const CModel* model );
    const CModel*               GetModel                ( ) const;
    CEntityModel*               GetEntityModel          ( ) const;

    /* Transformaciones */
    void                        SetPosition             ( const CVector& vecPosition );
    void                        Move                    ( const CVector& vec );
    const CVector&              GetPosition             ( );

    void                        SetRotation             ( const CVector& vecRotation );
    void                        Rotate                  ( const CVector& vec );
    const CVector&              GetRotation             ( );

    void                        SetScale                ( const CVector& vecScale );
    void                        Scale                   ( const CVector& vec );
    const CVector&              GetScale                ( );

    /* Matrices */
    virtual const CMatrix&      GetRotationMatrix       ( );
    virtual const CMatrix&      GetTranslationMatrix    ( );
    virtual const CMatrix&      GetScalingMatrix        ( );
    virtual const CMatrix&      GetMatrix               ( );

    virtual void                SetRotationMatrix       ( const CMatrix& matrix );
    virtual void                SetTranslationMatrix    ( const CMatrix& matrix );
    virtual void                SetScalingMatrix        ( const CMatrix& matrix );

    /* Atributos */
    inline virtual bool         IsVisible               ( ) const { return m_bIsVisible; }
    virtual void                SetVisible              ( bool bIsVisible );
    inline virtual bool         UsesLighting            ( ) const { return true; }

protected:
    CEntityModel*               m_pEntityModel;

private:
    static CPool < CEntity >    m_poolEntities;
    bool                        m_bIsVisible;
    bool                        m_bIsDestroyed;
};
