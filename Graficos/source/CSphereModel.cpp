/*
 * Práctica de Gráficos por computador.
 *
 * CSphereModel.cpp
 * Objetivo:  Representa el modelo de una esfera de radio 1.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CSphereModel::CSphereModel ( const unsigned int slices, const unsigned int stacks )
{
    MakeModel ( slices, stacks );
}

void CSphereModel::MakeModel ( const unsigned int slices, const unsigned int stacks )
{
    unsigned int uiNumVertices = ( slices + 1 ) * ( stacks + 1 );
    CVertex* pVertices = new CVertex [ uiNumVertices ];
    unsigned int uiNumIndices = slices * stacks * 6;
    unsigned int* pIndices = new unsigned int [ uiNumIndices ];
    CMatrix matRotation;

    /* La esfera se genera "tumbada", así que aplicaremos una rotación a todos los vértices generados */
    CMatrixFactory::GetXRotationMatrix ( matRotation, PI/2 );

    /* Generamos los vértices de la esfera con la ecuación paramétrica de la esfera */
    for ( unsigned int i = 0; i <= stacks; ++i )
    {
        double vSin = sin ( i * PI / stacks );
        double vCos = cos ( i * PI / stacks );

        for ( unsigned int j = 0; j <= slices; ++j )
        {
            double uSin = sin ( j * PI2 / slices );
            double uCos = cos ( j * PI2 / slices );

            unsigned int uiCurVertex = j + i * ( slices + 1 );
            CVertex& cur = pVertices [ uiCurVertex ];

            CVector vecPosition = CVector ( uSin * vSin, uCos * vSin, vCos ) * matRotation;
            cur.SetPosition ( vecPosition );
            cur.SetNormal ( vecPosition );

            /* Asumimos que el mapeado de texturas de planetas será cilíndrico */
            cur.SetTexture ( CVector ( (float)j / slices, (float)i / stacks ) );
        }
    }

    /* Generamos los índices. La esfera se genera con quads, pero la triangulamos. */
    unsigned int uiCurIndex = 0;
    unsigned int v1, v2, v3;
    for ( unsigned int i = 0; i < stacks; ++i )
    {
        for ( unsigned int j = 0; j < slices; ++j )
        {
            unsigned int uiRing = i * ( slices + 1 );
            unsigned int uiNextRing = ( i + 1 ) * ( slices + 1 );

            /* Primer triángulo */
            v1 = j + uiRing;
            v2 = j + uiNextRing;
            v3 = 1 + j + uiNextRing;

            if ( pVertices [ v1 ].GetPosition ( ) != pVertices [ v2 ].GetPosition ( ) &&
                 pVertices [ v1 ].GetPosition ( ) != pVertices [ v3 ].GetPosition ( ) &&
                 pVertices [ v2 ].GetPosition ( ) != pVertices [ v3 ].GetPosition ( ) )
            {
                pIndices [ uiCurIndex++ ] = v3;
                pIndices [ uiCurIndex++ ] = v2;
                pIndices [ uiCurIndex++ ] = v1;
            }

            /* Segundo triángulo */
            v2 = 1 + j + uiNextRing;
            v3 = 1 + j + uiRing;
            if ( pVertices [ v1 ].GetPosition ( ) != pVertices [ v2 ].GetPosition ( ) &&
                 pVertices [ v1 ].GetPosition ( ) != pVertices [ v3 ].GetPosition ( ) &&
                 pVertices [ v2 ].GetPosition ( ) != pVertices [ v3 ].GetPosition ( ) )
            {
                pIndices [ uiCurIndex++ ] = v3;
                pIndices [ uiCurIndex++ ] = v2;
                pIndices [ uiCurIndex++ ] = v1;
            }
        }
    }

    /* Construímos el grupo de índices */
    SModelGroup group;
    group.material.SetAmbient ( CColor ( 1.0f, 1.0f, 1.0f, 1.0f ) );
    group.pIndices = pIndices;
    group.uiNumFaces = uiCurIndex / 3;
    group.szName = L"planet_mesh";

    CModel::Set ( L"planet", pVertices, uiNumVertices, &group, 1, 0 );

    delete [] pIndices;
    delete [] pVertices;
}