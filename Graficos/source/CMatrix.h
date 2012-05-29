/*
 * Práctica de Gráficos por computador.
 *
 * CMatrix.h
 * Objetivo:  Matrices de transformación ( 4x4 ).
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CMatrix;

#include <string.h>

static const float __fIdentityMatrix [ 16 ] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

#define MATRIX_CELL(m,r,c) (m)[((r) << 2) | (c)]

class CMatrix
{
public:
    inline CMatrix ( const float fMatrix [ 16 ] = __fIdentityMatrix )
    {
        Set ( fMatrix );
    }

    inline CMatrix ( const CMatrix& Right )
    {
        memcpy ( m_fMatrix, Right.m_fMatrix, sizeof ( float ) * 16 );
    }

    inline float* GetMatrix ( )
    {
        return m_fMatrix;
    }

    inline const float* GetMatrix ( ) const
    {
        return m_fMatrix;
    }

    inline void Transpose ( )
    {
        float fMR [ 16 ];

        for ( unsigned int i = 0; i < 4; ++i )
        {
            for ( unsigned int j = 0; j < 4; ++j )
            {
                MATRIX_CELL ( fMR, i, j ) = MATRIX_CELL ( m_fMatrix, j, i );
            }
        }

        memcpy ( m_fMatrix, fMR, sizeof ( float ) * 16 );
    }

    inline void LoadIdentity ( )
    {
        memcpy ( m_fMatrix, __fIdentityMatrix, sizeof ( float ) * 16 );
    }

    inline void Set ( const float fMatrix [ 16 ] )
    {
        memcpy ( m_fMatrix, fMatrix, sizeof ( float ) * 16 );
    }


    /* M * M */
    inline CMatrix operator* ( const CMatrix& Right ) const
    {
        CMatrix ret;

        for ( unsigned int row = 0; row < 4; ++row )
        {
            for ( unsigned int col = 0; col < 16; col += 4 )
            {
                float fSum = 0.0f;
                for ( unsigned int k = 0; k < 4; ++k )
                    fSum += m_fMatrix [ row + k*4 ] * Right.m_fMatrix [ k + col ];
                ret.m_fMatrix [ row + col ] = fSum;
            }
        }

        return ret;
    }

    inline CMatrix& operator*= ( const CMatrix& Right )
    {
        float fMR [ 16 ];

        for ( unsigned int row = 0; row < 4; ++row )
        {
            for ( unsigned int col = 0; col < 16; col += 4 )
            {
                float fSum = 0.0f;
                for ( unsigned int k = 0; k < 4; ++k )
                    fSum += m_fMatrix [ row + k*4 ] * Right.m_fMatrix [ k + col ];
                fMR [ row + col ] = fSum;
            }
        }

        memcpy ( m_fMatrix, fMR, sizeof ( float ) * 16 );

        return *this;
    }

    /* M + M */
    inline CMatrix operator+ ( const CMatrix& Right ) const
    {
        CMatrix ret;

        for ( unsigned int i = 0; i < 4; ++i )
        {
            for ( unsigned int j = 0; j < 4; ++j )
            {
                MATRIX_CELL ( ret.m_fMatrix, i, j ) = MATRIX_CELL ( m_fMatrix, i, j ) + MATRIX_CELL ( Right.m_fMatrix, i, j );
            }
        }

        return ret;
    }

    inline CMatrix& operator+= ( const CMatrix& Right )
    {
        for ( unsigned int i = 0; i < 4; ++i )
        {
            for ( unsigned int j = 0; j < 4; ++j )
            {
                MATRIX_CELL ( m_fMatrix, i, j ) += MATRIX_CELL ( Right.m_fMatrix, i, j );
            }
        }

        return *this;
    }


    /* k * M */
    inline CMatrix operator* ( float fValue ) const
    {
        CMatrix ret;

        for ( unsigned int i = 0; i < 4; ++i )
        {
            for ( unsigned int j = 0; j < 4; ++j )
            {
                MATRIX_CELL ( ret.m_fMatrix, i, j ) = MATRIX_CELL ( m_fMatrix, i, j ) * fValue;
            }
        }

        return ret;
    }

    inline CMatrix& operator*= ( float fValue )
    {
        for ( unsigned int i = 0; i < 4; ++i )
        {
            for ( unsigned int j = 0; j < 4; ++j )
            {
                MATRIX_CELL ( m_fMatrix, i, j ) *= fValue;
            }
        }

        return *this;
    }

private:
    float m_fMatrix [ 16 ];
};
