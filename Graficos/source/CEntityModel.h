/*
 * Práctica de Gráficos por computador.
 *
 * CEntityModel.h
 * Objetivo:  Representación de las transformaciones para las geometrías del modelo de una entidad.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CEntityModel;

#include "CVector.h"
#include "CModel.h"
#include "main.h"
#include "CPool.h"

class CEntityModel
{
    friend class CEntity;

public:
    enum
    {
        INVALID_CHILD_INDEX = ( unsigned int )-1
    };

public:
                                CEntityModel            ( const CModel* pModel = 0 );
                                ~CEntityModel           ( );

    inline void*                operator new            ( size_t size )
    {
        return CEntityModel::m_poolEntityModels.Alloc ( size );
    }

    inline void                 operator delete         ( void* p )
    {
        CEntityModel::m_poolEntityModels.Free ( static_cast < CEntityModel * > ( p ) );
    }

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
    inline const CMatrix&       GetTranslationMatrix    ( ) { return m_matTranslation; }
    inline const CMatrix&       GetRotationMatrix       ( ) { return m_matRotation; }
    inline const CMatrix&       GetScalingMatrix        ( ) { return m_matScaling; }
    const CMatrix&              GetMatrix               ( );

    void                        SetTranslationMatrix    ( const CMatrix& matrix );
    void                        SetRotationMatrix       ( const CMatrix& matrix );
    void                        SetScalingMatrix        ( const CMatrix& matrix );

    /* Modelos */
    inline const CModel*        GetModel                ( ) const { return m_pModel; }
    inline unsigned int         GetNumChildren          ( ) const { return m_uiNumChildren; }
    inline CEntityModel*        GetChildren             ( ) { return m_pChildren; }

    unsigned int                GetChildIndex           ( const wstring& szName );
    inline CEntityModel*        GetChild                ( const wstring& szName ) { return GetChild ( GetChildIndex ( szName ) ); }
    CEntityModel*               GetChild                ( unsigned int uiIndex );

    /* Material */
    inline CMaterial&           GetMaterial             ( ) { return m_material; }

private:
    void                        SetModel                ( const CModel* pModel );

private:
    const CModel*               m_pModel;
    unsigned int                m_uiNumChildren;
    CEntityModel*               m_pChildren;

    CVector                     m_vecPosition;
    CVector                     m_vecRotation;
    CVector                     m_vecScale;

    CMatrix                     m_matTranslation;
    CMatrix                     m_matRotation;
    CMatrix                     m_matScaling;
    CMatrix                     m_matrix;
    bool                        m_bOutdatedMatrix;

    CMaterial                   m_material;

private:
    static CPool < CEntityModel >   m_poolEntityModels;
};
