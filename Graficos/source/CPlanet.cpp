/*
 * Práctica de Gráficos por computador.
 *
 * CPlanet.cpp
 * Objetivo:  Planetas.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

#define PLANET_SLICES 45
#define PLANET_STACKS 45

CPlanet::CPlanet ( const wstring& szTextureName, float fRadius )
{
    CMatrixFactory::GetScaleMatrix ( m_matScaling, CVector ( fRadius, fRadius, fRadius ) );
    const CModel* pModel = new CSphereModel ( PLANET_SLICES, PLANET_STACKS );

    CEntity::SetModel ( pModel );
    CEntityModel* pEntityModel = CEntity::GetEntityModel ( );
    CEntityModel* pChild = pEntityModel->GetChild ( 0 );

    if ( pChild )
    {
        /* TODO: Rutas... */
#ifdef WIN32
        wstring szPath = wstring ( L"../data/planets/" ) + szTextureName + wstring ( L".bmp" );
#else
        wstring szPath = wstring ( L"../../data/planets/" ) + szTextureName + wstring ( L".bmp" );
#endif
        CPixmap texture ( szPath );
        pChild->GetMaterial ( ).SetTexture ( texture );
    }
}

CPlanet::~CPlanet ( )
{
}

const CMatrix& CPlanet::GetScalingMatrix ( )
{
    return m_matScaling;
}

const CMatrix& CPlanet::GetMatrix ( )
{
    m_matrix = CEntity::GetMatrix ( ) * m_matScaling;
    return m_matrix;
}
