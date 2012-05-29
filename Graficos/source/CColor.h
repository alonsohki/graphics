/*
 * Práctica de Gráficos por computador.
 *
 * CColor.h
 * Objetivo:  Contenedor de colores.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CColor;

class CColor
{
private:
    float       m_fRed;
    float       m_fGreen;
    float       m_fBlue;
    float       m_fAlpha;

public:
    inline          CColor      ( float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f, float fAlpha = 1.0f )
        : m_fRed ( fRed ),
          m_fGreen ( fGreen ),
          m_fBlue ( fBlue ),
          m_fAlpha ( fAlpha )
    {
    }

    inline          CColor      ( const char* szColor )
        : m_fRed ( 1.0f ),
          m_fGreen ( 1.0f ),
          m_fBlue ( 1.0f ),
          m_fAlpha ( 1.0f )
    {
        Set ( szColor );
    }

    inline float    GetRed      ( ) const { return m_fRed; }
    inline float    GetGreen    ( ) const { return m_fGreen; }
    inline float    GetBlue     ( ) const { return m_fBlue; }
    inline float    GetAlpha    ( ) const { return m_fAlpha; }

    inline void     Set         ( float fRed, float fGreen, float fBlue, float fAlpha )
    {
        m_fRed = fRed;
        m_fGreen = fGreen;
        m_fBlue = fBlue;
        m_fAlpha = fAlpha;
    }
    inline void     Set         ( const char* szColor )
    {
        if ( *szColor == '#' )
            ++szColor;
        unsigned int uiLength = static_cast < unsigned int > ( strlen ( szColor ) );

        if ( uiLength > 2 )
        {
            if ( uiLength < 5 )
            {
                // #RGB
                m_fRed = hex2dec ( &szColor [ 0 ], 1 ) / 15.0f;
                m_fGreen = hex2dec ( &szColor [ 1 ], 1 ) / 15.0f;
                m_fBlue = hex2dec ( &szColor [ 2 ], 1 ) / 15.0f;

                if ( uiLength == 4 )
                {
                    // #RGBA
                    m_fAlpha = hex2dec ( &szColor [ 3 ], 1 ) / 15.0f;
                }
            }
            else if ( uiLength > 5 && uiLength < 9 )
            {
                // #RRGGBB
                m_fRed = hex2dec ( &szColor [ 0 ], 2 ) / 255.0f;
                m_fGreen = hex2dec ( &szColor [ 2 ], 2 ) / 255.0f;
                m_fBlue = hex2dec ( &szColor [ 4 ], 2 ) / 255.0f;

                if ( uiLength == 8 )
                {
                    // #RRGGBBAA
                    m_fAlpha = hex2dec ( &szColor [ 6 ], 2 ) / 255.0f;
                }
            }
        }
    }
    inline void     SetRed      ( float fRed ) { m_fRed = fRed; }
    inline void     SetGreen    ( float fGreen ) { m_fGreen = fGreen; }
    inline void     SetBlue     ( float fBlue ) { m_fBlue = fBlue; }
    inline void     SetAlpha    ( float fAlpha ) { m_fAlpha = fAlpha; }

    inline unsigned long    GetRGB      ( ) const
    {
        return ( ( ((unsigned long)( m_fRed * 255 )      << 16 ) & 0x00FF0000UL ) |
                 ( ((unsigned long)( m_fGreen * 255 )    <<  8 ) & 0x0000FF00UL ) |
                 ( ((unsigned long)( m_fBlue * 255 )           ) & 0x000000FFUL ) ); 
    }

    inline unsigned long    GetARGB     ( ) const
    {
        return ( ( ((unsigned long)( m_fAlpha * 255 )    << 24 ) & 0xFF000000UL ) |
                 ( ((unsigned long)( m_fRed * 255 )      << 16 ) & 0x00FF0000UL ) |
                 ( ((unsigned long)( m_fGreen * 255 )    <<  8 ) & 0x0000FF00UL ) |
                 ( ((unsigned long)( m_fBlue * 255 )           ) & 0x000000FFUL ) ); 
    }

    inline unsigned long    GetRGBA     ( ) const
    {
        return ( ( ((unsigned long)( m_fRed * 255 )      << 24 ) & 0xFF000000UL ) |
                 ( ((unsigned long)( m_fGreen * 255 )    << 16 ) & 0x00FF0000UL ) |
                 ( ((unsigned long)( m_fBlue * 255 )     <<  8 ) & 0x0000FF00UL ) |
                 ( ((unsigned long)( m_fAlpha * 255 )          ) & 0x000000FFUL ) ); 
    }

    inline unsigned long    GetBGR      ( ) const
    {
        return ( ( ((unsigned long)( m_fBlue * 255 )     << 16 ) & 0x00FF0000UL ) |
                 ( ((unsigned long)( m_fGreen * 255 )    <<  8 ) & 0x0000FF00UL ) |
                 ( ((unsigned long)( m_fRed * 255 )            ) & 0x000000FFUL ) ); 
    }

    inline unsigned long    GetABGR     ( ) const
    {
        return ( ( ((unsigned long)( m_fAlpha * 255 )    << 24 ) & 0xFF000000UL ) |
                 ( ((unsigned long)( m_fBlue * 255 )     << 16 ) & 0x00FF0000UL ) |
                 ( ((unsigned long)( m_fGreen * 255 )    <<  8 ) & 0x0000FF00UL ) |
                 ( ((unsigned long)( m_fRed * 255 )            ) & 0x000000FFUL ) ); 
    }

    inline unsigned long    GetBGRA     ( ) const
    {
        return ( ( ((unsigned long)( m_fBlue * 255 )     << 24 ) & 0xFF000000UL ) |
                 ( ((unsigned long)( m_fGreen * 255 )    << 16 ) & 0x00FF0000UL ) |
                 ( ((unsigned long)( m_fRed * 255 )      <<  8 ) & 0x0000FF00UL ) |
                 ( ((unsigned long)( m_fAlpha * 255 )          ) & 0x000000FFUL ) ); 
    }
};
