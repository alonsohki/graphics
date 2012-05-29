/*
 * Práctica de Gráficos por computador.
 *
 * CEntityModel.cpp
 * Objetivo:  Representación de las transformaciones para las geometrías del modelo de una entidad.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CPool < CEntityModel > CEntityModel::m_poolEntityModels ( 4096 );

CEntityModel::CEntityModel ( const CModel* pModel )
    : m_uiNumChildren ( 0 ),
      m_pChildren ( 0 ),
      m_bOutdatedMatrix ( false )
{
    SetModel ( pModel );
}

void CEntityModel::SetModel ( const CModel* pModel )
{
    if ( m_pModel )
    {
        delete [] m_pChildren;
        m_pChildren = 0;
        m_uiNumChildren = 0;
    }

    m_pModel = pModel;

    if ( m_pModel )
    {
        m_material = m_pModel->GetMaterial ( );

        unsigned int uiNumChildren = m_pModel->GetNumChildren ( );
        if ( uiNumChildren > 0 )
        {
            const CModel* pModelChildren = m_pModel->GetChildren ( );
            m_uiNumChildren = uiNumChildren;

            m_pChildren = new CEntityModel [ uiNumChildren ] ( );
            for ( unsigned int i = 0; i < uiNumChildren; ++i )
            {
                m_pChildren [ i ].SetModel ( &pModelChildren [ i ] );
            }
        }
    }
}

CEntityModel::~CEntityModel ( )
{
    if ( m_pChildren )
    {
        delete [] m_pChildren;
        m_uiNumChildren = 0;
    }
}

unsigned int CEntityModel::GetChildIndex ( const wstring& szName )
{
    if ( m_pChildren )
    {
        for ( unsigned int i = 0; i < m_uiNumChildren; ++i )
        {
            const CModel* pModel = m_pChildren [ i ].GetModel ( );
            if ( pModel && pModel->GetName ( ) == szName )
            {
                return i;
            }
        }
    }

    return CEntityModel::INVALID_CHILD_INDEX;
}

CEntityModel* CEntityModel::GetChild ( unsigned int uiIndex )
{
    if ( m_pChildren && uiIndex < m_uiNumChildren )
        return &m_pChildren [ uiIndex ];
    return 0;
}

/* Transformaciones */
void CEntityModel::SetPosition ( const CVector& vecPosition )
{
    CMatrixFactory::GetTranslationMatrix ( m_matTranslation, vecPosition );
    m_bOutdatedMatrix = true;
}

void CEntityModel::Move ( const CVector& vec )
{
    CMatrix temp;
    CMatrixFactory::GetTranslationMatrix ( temp, vec );
    m_matTranslation *= temp;
    m_bOutdatedMatrix = true;
}

const CVector& CEntityModel::GetPosition ( )
{
    GetMatrix ( );
    return m_vecPosition;
}

void CEntityModel::SetTranslationMatrix ( const CMatrix& matrix )
{
    m_matTranslation = matrix;
    m_bOutdatedMatrix = true;
}



void CEntityModel::SetRotation ( const CVector& vecRotation )
{
    CMatrix temp;
    CMatrixFactory::GetYRotationMatrix ( m_matRotation, vecRotation.fY );
    CMatrixFactory::GetZRotationMatrix ( temp, vecRotation.fZ );
    m_matRotation *= temp;
    CMatrixFactory::GetXRotationMatrix ( temp, vecRotation.fX );
    m_matRotation *= temp;
    m_bOutdatedMatrix = true;
}

void CEntityModel::Rotate ( const CVector& vec )
{
    CMatrix temp;
    CMatrixFactory::GetYRotationMatrix ( temp, vec.fY );
    m_matRotation *= temp;
    CMatrixFactory::GetZRotationMatrix ( temp, vec.fZ );
    m_matRotation *= temp;
    CMatrixFactory::GetXRotationMatrix ( temp, vec.fX );
    m_matRotation *= temp;
    m_bOutdatedMatrix = true;
}

const CVector& CEntityModel::GetRotation ( )
{
    GetMatrix ( );
    return m_vecRotation;
}

void CEntityModel::SetRotationMatrix ( const CMatrix& matrix )
{
    m_matRotation = matrix;
    m_bOutdatedMatrix = true;
}



void CEntityModel::SetScale ( const CVector& vecScale )
{
    CMatrixFactory::GetScaleMatrix ( m_matScaling, vecScale );
    m_bOutdatedMatrix = true;
}

void CEntityModel::Scale ( const CVector& vec )
{
    CMatrix temp;
    CMatrixFactory::GetScaleMatrix ( temp, vec );
    m_matScaling *= temp;
    m_bOutdatedMatrix = true;
}

const CVector& CEntityModel::GetScale ( )
{
    GetMatrix ( );
    return m_vecScale;
}

void CEntityModel::SetScalingMatrix ( const CMatrix& matrix )
{
    m_matScaling = matrix;
    m_bOutdatedMatrix = true;
}




/* Matrices */
const CMatrix& CEntityModel::GetMatrix ( )
{
    if ( m_bOutdatedMatrix )
    {
        CMatrix matCentroidTranslation;
        m_bOutdatedMatrix = false;

        if ( m_pModel )
            CMatrixFactory::GetTranslationMatrix ( matCentroidTranslation, m_pModel->GetCentroid ( ) * -1 );

        m_matrix = m_matTranslation * m_matRotation * m_matScaling * matCentroidTranslation;

        /* Actualizamos los datos de posición, rotación y escalado */
        m_vecPosition = CVector ( 0.0f, 0.0f, 0.0f ) * m_matTranslation;
        //m_vecRotation = 
        m_vecScale = CVector ( 1.0f, 1.0f, 1.0f ) * m_matScaling;
    }
    return m_matrix;
}
