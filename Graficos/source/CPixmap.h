/*
 * Práctica de Gráficos por computador.
 *
 * CPixmap.h
 * Objetivo:  Clase que interpreta un fichero de imágen y almacena el mapa de pixels.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CPixmap;

#include "main.h"

class CPixmap
{
public:
                                CPixmap         ( );
                                CPixmap         ( const wstring& szFile );
                                CPixmap         ( const CPixmap& Right );
                                ~CPixmap        ( );

    bool                        operator=       ( const CPixmap& Right );

    void                        Load            ( const wstring& szFile );
private:
    void                        UnLoad          ( );
public:

    inline bool                 IsOk            ( ) const { return ( m_pPixels != 0 ); }

    inline int                  GetDepth        ( ) const { return m_iDepth; }
    inline int                  GetWidth        ( ) const { return m_iWidth; }
    inline int                  GetHeight       ( ) const { return m_iHeight; }
    inline unsigned char*       GetPixels       ( ) const { return m_pPixels; }

    inline unsigned int         GetID           ( ) const { return m_uiID; }

private:
    void                        ParseBMP        ( FILE* file );

private:
    static unsigned int         GetUniqueID     ( );

private:
    int                 m_iDepth;
    int                 m_iWidth;
    int                 m_iHeight;
    unsigned char*      m_pPixels;
    unsigned int        m_uiID;
};

