/*
 * Práctica de Gráficos por computador.
 *
 * CSkyModel.cpp
 * Objetivo:  Modelo del cielo. Simplemente un cubo que rodeará a la nave.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CSkyModel* CSkyModel::GetSingletonPtr ( )
{
    static CSkyModel* instance = 0;

    if ( instance == 0 )
    {
        instance = new CSkyModel ( );
    }

    return instance;
}

CSkyModel& CSkyModel::GetSingleton ( )
{
    return *GetSingletonPtr ( );
}

CSkyModel::CSkyModel ( )
{
    CVertex vertices [ 24 ];

    /* Cara izquierda */
    vertices [ 0 ].SetPosition ( CVector ( 1.0f, -1.0f, -1.0f ) );
    vertices [ 1 ].SetPosition ( CVector ( 1.0f, -1.0f, 1.0f ) );
    vertices [ 2 ].SetPosition ( CVector ( 1.0f, 1.0f, 1.0f ) );
    vertices [ 3 ].SetPosition ( CVector ( 1.0f, 1.0f, -1.0f ) );
    vertices [ 0 ].SetTexture ( CVector ( 0.0f, 0.0f ) );
    vertices [ 1 ].SetTexture ( CVector ( 1.0f, 0.0f ) );
    vertices [ 2 ].SetTexture ( CVector ( 1.0f, 1.0f ) );
    vertices [ 3 ].SetTexture ( CVector ( 0.0f, 1.0f ) );

    /* Cara frontal */
    vertices [ 4 ].SetPosition ( CVector ( 1.0f, -1.0f, 1.0f ) );
    vertices [ 5 ].SetPosition ( CVector ( -1.0f, -1.0f, 1.0f ) );
    vertices [ 6 ].SetPosition ( CVector ( -1.0f, 1.0f, 1.0f ) );
    vertices [ 7 ].SetPosition ( CVector ( 1.0f, 1.0f, 1.0f ) );
    vertices [ 4 ].SetTexture ( CVector ( 0.0f, 0.0f ) );
    vertices [ 5 ].SetTexture ( CVector ( 1.0f, 0.0f ) );
    vertices [ 6 ].SetTexture ( CVector ( 1.0f, 1.0f ) );
    vertices [ 7 ].SetTexture ( CVector ( 0.0f, 1.0f ) );

    /* Cara derecha */
    vertices [ 8 ].SetPosition ( CVector ( -1.0f, -1.0f, 1.0f ) );
    vertices [ 9 ].SetPosition ( CVector ( -1.0f, -1.0f, -1.0f ) );
    vertices [ 10 ].SetPosition ( CVector ( -1.0f, 1.0f, -1.0f ) );
    vertices [ 11 ].SetPosition ( CVector ( -1.0f, 1.0f, 1.0f ) );
    vertices [ 8 ].SetTexture ( CVector ( 0.0f, 0.0f ) );
    vertices [ 9 ].SetTexture ( CVector ( 1.0f, 0.0f ) );
    vertices [ 10 ].SetTexture ( CVector ( 1.0f, 1.0f ) );
    vertices [ 11 ].SetTexture ( CVector ( 0.0f, 1.0f ) );

    /* Cara trasera */
    vertices [ 12 ].SetPosition ( CVector ( -1.0f, -1.0f, -1.0f ) );
    vertices [ 13 ].SetPosition ( CVector ( 1.0f, -1.0f, -1.0f ) );
    vertices [ 14 ].SetPosition ( CVector ( 1.0f, 1.0f, -1.0f ) );
    vertices [ 15 ].SetPosition ( CVector ( -1.0f, 1.0f, -1.0f ) );
    vertices [ 12 ].SetTexture ( CVector ( 0.0f, 0.0f ) );
    vertices [ 13 ].SetTexture ( CVector ( 1.0f, 0.0f ) );
    vertices [ 14 ].SetTexture ( CVector ( 1.0f, 1.0f ) );
    vertices [ 15 ].SetTexture ( CVector ( 0.0f, 1.0f ) );

    /* Cara superior */
    vertices [ 16 ].SetPosition ( CVector ( 1.0f, 1.0f, 1.0f ) );
    vertices [ 17 ].SetPosition ( CVector ( -1.0f, 1.0f, 1.0f ) );
    vertices [ 18 ].SetPosition ( CVector ( -1.0f, 1.0f, -1.0f ) );
    vertices [ 19 ].SetPosition ( CVector ( 1.0f, 1.0f, -1.0f ) );
    vertices [ 16 ].SetTexture ( CVector ( 0.0f, 0.0f ) );
    vertices [ 17 ].SetTexture ( CVector ( 1.0f, 0.0f ) );
    vertices [ 18 ].SetTexture ( CVector ( 1.0f, 1.0f ) );
    vertices [ 19 ].SetTexture ( CVector ( 0.0f, 1.0f ) );

    /* Cara inferior */
    vertices [ 20 ].SetPosition ( CVector ( 1.0f, -1.0f, -1.0f ) );
    vertices [ 21 ].SetPosition ( CVector ( -1.0f, -1.0f, -1.0f ) );
    vertices [ 22 ].SetPosition ( CVector ( -1.0f, -1.0f, 1.0f ) );
    vertices [ 23 ].SetPosition ( CVector ( 1.0f, -1.0f, 1.0f ) );
    vertices [ 20 ].SetTexture ( CVector ( 0.0f, 0.0f ) );
    vertices [ 21 ].SetTexture ( CVector ( 1.0f, 0.0f ) );
    vertices [ 22 ].SetTexture ( CVector ( 1.0f, 1.0f ) );
    vertices [ 23 ].SetTexture ( CVector ( 0.0f, 1.0f ) );

    /* Cada cara tiene su propia textura y por tanto su propio material */
    SModelGroup groups [ 6 ];
    unsigned int indices [ 36 ];

    /* Cargamos las texturas */
    CPixmap textures [ 6 ];

    /* TODO: Misma chapuza de siempre con las rutas relativas... */
#ifdef WIN32
    textures [ 0 ].Load ( L"../data/sky_left.bmp" );
    textures [ 1 ].Load ( L"../data/sky_front.bmp" );
    textures [ 2 ].Load ( L"../data/sky_right.bmp" );
    textures [ 3 ].Load ( L"../data/sky_back.bmp" );
    textures [ 4 ].Load ( L"../data/sky_top.bmp" );
    textures [ 5 ].Load ( L"../data/sky_bottom.bmp" );
#else
    textures [ 0 ].Load ( L"../../data/sky_left.bmp" );
    textures [ 1 ].Load ( L"../../data/sky_front.bmp" );
    textures [ 2 ].Load ( L"../../data/sky_right.bmp" );
    textures [ 3 ].Load ( L"../../data/sky_back.bmp" );
    textures [ 4 ].Load ( L"../../data/sky_top.bmp" );
    textures [ 5 ].Load ( L"../../data/sky_bottom.bmp" );
#endif

    /* Cargamos los índices (triangulando cada quad que forma cada cara) */
    for ( unsigned int i = 0, j = 0; i < 24; i += 4, j += 6 )
    {
        for ( int k = 0; k < 6; ++k ) indices [ j + k ] = 0;
        indices [ j ] = i;
        indices [ j + 1 ] = i + 1;
        indices [ j + 2 ] = i + 2;
        indices [ j + 3 ] = i;
        indices [ j + 4 ] = i + 2;
        indices [ j + 5 ] = i + 3;
    }

    /* Cara izquierda */
    groups [ 0 ].szName = L"sky_left";
    groups [ 0 ].material.SetTexture ( textures [ 0 ] );
    groups [ 0 ].uiNumFaces = 2;
    groups [ 0 ].pIndices = &indices [ 0 ];

    /* Cara frontal */
    groups [ 1 ].szName = L"sky_front";
    groups [ 1 ].material.SetTexture ( textures [ 1 ] );
    groups [ 1 ].uiNumFaces = 2;
    groups [ 1 ].pIndices = &indices [ 6 ];

    /* Cara derecha */
    groups [ 2 ].szName = L"sky_right";
    groups [ 2 ].material.SetTexture ( textures [ 2 ] );
    groups [ 2 ].uiNumFaces = 2;
    groups [ 2 ].pIndices = &indices [ 12 ];

    /* Cara trasera */
    groups [ 3 ].szName = L"sky_back";
    groups [ 3 ].material.SetTexture ( textures [ 3 ] );
    groups [ 3 ].uiNumFaces = 2;
    groups [ 3 ].pIndices = &indices [ 18 ];

    /* Cara superior */
    groups [ 4 ].szName = L"sky_top";
    groups [ 4 ].material.SetTexture ( textures [ 4 ] );
    groups [ 4 ].uiNumFaces = 2;
    groups [ 4 ].pIndices = &indices [ 24 ];

    /* Cara inferior */
    groups [ 5 ].szName = L"sky_bottom";
    groups [ 5 ].material.SetTexture ( textures [ 5 ] );
    groups [ 5 ].uiNumFaces = 2;
    groups [ 5 ].pIndices = &indices [ 30 ];

    CModel::Set ( L"sky", vertices, 24, groups, 6, 0 );
}

CSkyModel::~CSkyModel ( )
{
}

