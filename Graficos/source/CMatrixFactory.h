/*
 * Práctica de Gráficos por computador.
 *
 * CMatrixFactory.h
 * Objetivo:  Funciones de rotación, translación y escalado.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CMatrixFactory;

#include "CMatrix.h"
#include "CVector.h"

class CMatrixFactory
{
private:
    inline CMatrixFactory ( ) { };
    inline ~CMatrixFactory ( ) { };

public:

    inline static void GetXRotationMatrix ( CMatrix& matrix, float fAngle )
    {
        float angleSin = sin ( fAngle );
        float angleCos = cos ( fAngle );
        CMatrix auxMatrix;
        
        float rotateXArray [ 16 ] = {
        1.0f,          0.0f,     0.0f, 0.0f,
        0.0f,      angleCos, angleSin, 0.0f,
        0.0f,     -angleSin, angleCos, 0.0f,
        0.0f,          0.0f,     0.0f, 1.0f };
        
        matrix.Set ( rotateXArray );
    }

    inline static void GetYRotationMatrix ( CMatrix& matrix, float fAngle )
    {
        float angleSin = sin ( fAngle );
        float angleCos = cos ( fAngle );
        
        float rotateYArray [ 16 ] = {
            angleCos, 0.0f, -angleSin, 0.0f,
                0.0f, 1.0f,      0.0f, 0.0f,
            angleSin, 0.0f,  angleCos, 0.0f,
                0.0f, 0.0f,      0.0f, 1.0f };
            
        matrix.Set ( rotateYArray );
    }

    inline static void GetZRotationMatrix ( CMatrix& matrix, float fAngle )
    {
        float angleSin = sin ( fAngle );
        float angleCos = cos ( fAngle );
        
        float rotateZArray [ 16 ] = {
             angleCos, angleSin, 0.0f, 0.0f,
            -angleSin, angleCos, 0.0f, 0.0f,
                 0.0f,     0.0f, 1.0f, 0.0f,
                 0.0f,     0.0f, 0.0f, 1.0f };
        
        matrix.Set ( rotateZArray );
    }

    inline static void GetTranslationMatrix ( CMatrix& matrix, const CVector& vec )
    {
        float translateArray [ 16 ] = {
          1.0f,   0.0f,   0.0f, 0.0f,
          0.0f,   1.0f,   0.0f, 0.0f,
          0.0f,   0.0f,   1.0f, 0.0f,
        vec.fX, vec.fY, vec.fZ, 1.0f };
        
        matrix.Set ( translateArray );
    }

    inline static void GetScaleMatrix ( CMatrix& matrix, const CVector& vec )
    {
        
        float scaleArray [ 16 ] = {
        vec.fX,   0.0f,   0.0f, 0.0f,
          0.0f, vec.fY,   0.0f, 0.0f,
          0.0f,   0.0f, vec.fZ, 0.0f,
          0.0f,   0.0f,   0.0f, 1.0f };

        matrix.Set ( scaleArray );
    }

    inline static void GetPerspectiveMatrix ( CMatrix& matrix, float fFOV, float fAspect, float fNear, float fFar )
    {
        float fOffset = fNear - fFar;
        float f = 1.0f / tan ( fFOV / 2 );

        float fMatrix [ 16 ] = {
            f / fAspect,    0.0f,       0.0f,                               0.0f,
            0.0f,           f,          0.0f,                               0.0f,
            0.0f,           0.0f,       ( fFar + fNear ) / fOffset,         -1.0f,
            0.0f,           0.0f,       ( 2 * fFar * fNear ) / fOffset,     0.0f
        };

        matrix.Set ( fMatrix );
    }

    inline static void GetLookatMatrix ( CMatrix& matrix, const CVector& vecEye, const CVector& vecTarget, const CVector& vecUp )
    {
        CVector f ( vecTarget - vecEye );
        CVector up ( vecUp );

        f.Normalize ( );

        CVector s = f ^ up;
        CVector u = s ^ f;

        s.Normalize ( );
        u.Normalize ( );

        float fMatrix [ 16 ] = {
            s.fX,       u.fX,       -f.fX,      0.0f,
            s.fY,       u.fY,       -f.fY,      0.0f,
            s.fZ,       u.fZ,       -f.fZ,      0.0f,
            0.0f,       0.0f,       0.0f,       1.0f
        };
        matrix.Set ( fMatrix );

        CMatrix matTranslation;
        GetTranslationMatrix ( matTranslation, -vecEye );

        matrix *= matTranslation;
    }

    /* Matriz de orientación. Básicamente la inversa de una matriz lookat, sin translación */
    inline static void GetOrientationMatrix ( CMatrix& matrix, const CVector& vecDirection, const CVector& vecUp = CVector ( 0.0f, 1.0f, 0.0f ) )
    {
        CVector f ( vecDirection );
        CVector up ( vecUp );

        f.Normalize ( );
        up.Normalize ( );

        CVector s = f ^ up;
        CVector u = s ^ f;

        s.Normalize ( );
        u.Normalize ( );

        float fMatrix [ 16 ] = {
            s.fX,       s.fY,       s.fZ,      0.0f,
            u.fX,       u.fY,       u.fZ,      0.0f,
           -f.fX,      -f.fY,      -f.fZ,      0.0f,
            0.0f,       0.0f,       0.0f,      1.0f
        };

        matrix.Set ( fMatrix );
    }
};
