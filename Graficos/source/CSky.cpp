/*
 * Práctica de Gráficos por computador.
 *
 * CSky.cpp
 * Objetivo:  Representación del cielo.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CSky::CSky ( )
{
    const CModel* pModel = CSkyModel::GetSingletonPtr ( );
    CEntity::SetModel ( pModel );
}

CSky::~CSky ( )
{
}

const CMatrix& CSky::GetTranslationMatrix ( )
{
    return CApplication::GetSingleton ( ).GetGame ( ).GetCamera ( ).GetTranslationMatrix ( );
}

const CMatrix& CSky::GetScalingMatrix ( )
{
    float fFarHalf = CApplication::GetSingleton ( ).GetGame ( ).GetCamera ( ).GetFar ( ) / 2.0f;
    CMatrixFactory::GetScaleMatrix ( m_matScaling, CVector ( fFarHalf, fFarHalf, fFarHalf ) );
    return m_matScaling;
}

const CMatrix& CSky::GetMatrix ( )
{
    m_matrix = GetTranslationMatrix ( ) * GetScalingMatrix ( );
    return m_matrix;
}

