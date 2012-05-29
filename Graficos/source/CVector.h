/*
 * Práctica de Gráficos por computador.
 *
 * CVector.h
 * Objetivo:  Vectores 3D. Almacenamiento y operaciones vectoriales. También se utiliza
 *            para representar puntos.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CVector;

#include "CMatrix.h"

class CVector
{
public:
    float   fX;
    float   fY;
    float   fZ;

public:
    inline CVector ( float _fX = 0.0f, float _fY = 0.0f, float _fZ = 0.0f )
        : fX( _fX ),
          fY( _fY ),
          fZ( _fZ )
    {
    }

    inline CVector ( const CVector& Right )
    {
        fX = Right.fX;
        fY = Right.fY;
        fZ = Right.fZ;
    }

    inline bool IsNull ( ) const
    {
        return !( fX != 0.0f || fY != 0.0f || fZ != 0.0f );
    }

    inline float Module ( ) const
    {
        return sqrt ( fX*fX + fY*fY + fZ*fZ );
    }

    inline void Normalize ( )
    {
        if ( !IsNull ( ) )
        {
            float fModule = Module ( );
            fX /= fModule;
            fY /= fModule;
            fZ /= fModule;
        }
    }

    /* Comparación */
    inline bool operator== ( const CVector& Right ) const
    {
        return !( fX != Right.fX || fY != Right.fY || fZ != Right.fZ );
    }
    inline bool operator!= ( const CVector& Right ) const
    {
        return ( fX != Right.fX || fY != Right.fY || fZ != Right.fZ );
    }

    /* Operaciones ( vector, vector ) */
    inline CVector& operator+= ( const CVector& Right )
    {
        fX += Right.fX;
        fY += Right.fY;
        fZ += Right.fZ;
        return *this;
    }
    inline CVector operator+ ( const CVector& Right ) const
    {
        CVector temp;
        temp.fX = fX + Right.fX;
        temp.fY = fY + Right.fY;
        temp.fZ = fZ + Right.fZ;
        return temp;
    }


    inline CVector& operator-= ( const CVector& Right )
    {
        fX -= Right.fX;
        fY -= Right.fY;
        fZ -= Right.fZ;
        return *this;
    }
    inline CVector operator- ( const CVector& Right ) const
    {
        CVector temp;
        temp.fX = fX - Right.fX;
        temp.fY = fY - Right.fY;
        temp.fZ = fZ - Right.fZ;
        return temp;
    }


    inline float operator* ( const CVector& Right ) const
    {
        /* Producto escalar: v * v */
        return fX * Right.fX + fY * Right.fY + fZ * Right.fZ;
    }


    inline CVector& operator^= ( const CVector& Right )
    {
        /* Denotamos el producto vectorial como v ^ v */
        fX = fY * Right.fZ - fZ * Right.fY;
        fY = fZ * Right.fX - fX * Right.fZ;
        fZ = fX * Right.fY - fY * Right.fX;
        return *this;
    }
    inline CVector operator^ ( const CVector& Right ) const
    {
        CVector temp;
        temp.fX = fY * Right.fZ - fZ * Right.fY;
        temp.fY = fZ * Right.fX - fX * Right.fZ;
        temp.fZ = fX * Right.fY - fY * Right.fX;
        return temp;
    }

    /* Operaciones ( vector, escalar ) */
    inline CVector& operator*= ( float fValue )
    {
        fX *= fValue;
        fY *= fValue;
        fZ *= fValue;
        return *this;
    }
    inline CVector operator* ( float fValue ) const
    {
        CVector temp;
        temp.fX = fX * fValue;
        temp.fY = fY * fValue;
        temp.fZ = fZ * fValue;
        return temp;
    }


    inline CVector& operator/= ( float fValue )
    {
        fX /= fValue;
        fY /= fValue;
        fZ /= fValue;
        return *this;
    }
    inline CVector operator/ ( float fValue ) const
    {
        CVector temp;
        temp.fX = fX / fValue;
        temp.fY = fY / fValue;
        temp.fZ = fZ / fValue;
        return temp;
    }


    inline CVector& operator%= ( float fValue )
    {
        if ( fX > 0 )
            while ( fX >= fValue ) fX -= fValue;
        else
            while ( fX <= -fValue ) fX += fValue;
        if ( fY > 0 )
            while ( fY >= fValue ) fY -= fValue;
        else
            while ( fY <= -fValue ) fY += fValue;
        if ( fZ > 0 )
            while ( fZ >= fValue ) fZ -= fValue;
        else
            while ( fZ <= -fValue ) fZ += fValue;

        return *this;
    }
    inline CVector operator% ( float fValue ) const
    {
        CVector temp = *this;

        if ( temp.fX > 0 )
            while ( temp.fX >= fValue ) temp.fX -= fValue;
        else
            while ( temp.fX <= -fValue ) temp.fX += fValue;
        if ( temp.fY > 0 )
            while ( temp.fY >= fValue ) temp.fY -= fValue;
        else
            while ( temp.fY <= -fValue ) temp.fY += fValue;
        if ( temp.fZ > 0 )
            while ( temp.fZ >= fValue ) temp.fZ -= fValue;
        else
            while ( temp.fZ <= -fValue ) temp.fZ += fValue;

        return temp;
    }

    // Operaciones vector * matrix
    inline CVector& operator*= ( const CMatrix& mat )
    {
        CVector temp;
        const float* fMat = mat.GetMatrix ( );

        temp.fX = fX * fMat [ 0 ] + fY * fMat [ 4 ] + fZ * fMat [ 8 ]  + fMat [ 12 ];
        temp.fY = fX * fMat [ 1 ] + fY * fMat [ 5 ] + fZ * fMat [ 9 ]  + fMat [ 13 ];
        temp.fZ = fX * fMat [ 2 ] + fY * fMat [ 6 ] + fZ * fMat [ 10 ] + fMat [ 14 ];
        float fW =     fMat [ 3 ] +      fMat [ 7 ] +      fMat [ 11 ] + fMat [ 15 ];

        temp /= fW;
        *this = temp;

        return *this;
    }

    inline CVector operator* ( const CMatrix& mat ) const
    {
        CVector temp;
        const float* fMat = mat.GetMatrix ( );

        temp.fX = fX * fMat [ 0 ] + fY * fMat [ 4 ] + fZ * fMat [ 8 ]  + fMat [ 12 ];
        temp.fY = fX * fMat [ 1 ] + fY * fMat [ 5 ] + fZ * fMat [ 9 ]  + fMat [ 13 ];
        temp.fZ = fX * fMat [ 2 ] + fY * fMat [ 6 ] + fZ * fMat [ 10 ] + fMat [ 14 ];
        float fW =     fMat [ 3 ] +      fMat [ 7 ] +      fMat [ 11 ] + fMat [ 15 ];

        temp /= fW;

        return temp;
    }

    inline CVector operator- ( ) const
    {
        CVector temp;

        temp.fX = -fX;
        temp.fY = -fY;
        temp.fZ = -fZ;

        return temp;
    }
};
