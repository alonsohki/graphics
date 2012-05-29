/*
 * Práctica de Gráficos por computador.
 *
 * CPixmap.cpp
 * Objetivo:  Clase que interpreta un fichero de imágen y almacena el mapa de pixels.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

unsigned int CPixmap::GetUniqueID ( )
{
    static unsigned int uiID = 0;

    ++uiID;
    return uiID;
}

CPixmap::CPixmap ( )
    : m_pPixels ( 0 )
{
    m_uiID = GetUniqueID ( );
    UnLoad ( );
}

CPixmap::CPixmap ( const wstring& szFile )
    : m_pPixels ( 0 )
{
    m_uiID = GetUniqueID ( );
    Load ( szFile );
}

CPixmap::CPixmap ( const CPixmap& Right )
{
    m_iDepth = Right.m_iDepth;
    m_iWidth = Right.m_iWidth;
    m_iHeight = Right.m_iHeight;
    m_uiID = Right.m_uiID;

    int size = m_iWidth * m_iHeight * m_iDepth;
    if ( Right.m_pPixels != 0 && size > 0 )
    {
        m_pPixels = new unsigned char [ size ];
        memcpy ( m_pPixels, Right.m_pPixels, size * sizeof ( unsigned char ) );
    }
    else
        m_pPixels = 0;
}

bool CPixmap::operator= ( const CPixmap& Right )
{
    UnLoad ( );

    m_iDepth = Right.m_iDepth;
    m_iWidth = Right.m_iWidth;
    m_iHeight = Right.m_iHeight;
    m_uiID = Right.m_uiID;

    int size = m_iWidth * m_iHeight * m_iDepth;
    if ( Right.m_pPixels != 0 && size > 0 )
    {
        m_pPixels = new unsigned char [ size ];
        memcpy ( m_pPixels, Right.m_pPixels, size * sizeof ( unsigned char ) );
    }

    return true;
}

CPixmap::~CPixmap ( )
{
    UnLoad ( );
}

void CPixmap::UnLoad ( )
{
    if ( m_pPixels )
        delete [] m_pPixels;
    m_pPixels = 0;
    m_iDepth = 0;
    m_iWidth = 0;
    m_iHeight = 0;
}

void CPixmap::Load ( const wstring& szFile )
{
    UnLoad ( );

    /* Obtenemos la extensión del archivo para "conocer" su formato */
    size_t iExtPos = szFile.rfind ( L"." );
    if ( iExtPos != wstring::npos )
    {
        wstring szExt ( szFile, iExtPos + 1 );

        std::string szFile_iso ( szFile.begin( ), szFile.end( ) );
        szFile_iso.assign ( szFile.begin( ), szFile.end( ) );

        FILE* file;
#ifdef WIN32
        fopen_s ( &file, szFile_iso.c_str ( ), "rb" );
#else
        file = fopen ( szFile_iso.c_str ( ), "rb" );
#endif

        if ( file != NULL )
        {
            if ( szExt == L"bmp" )
                ParseBMP ( file );

            fclose ( file );
        }
    }
}

/* BMP, intérprete donado por Rayco Hernandez García */
const unsigned short BMPmagic = 0x4D42;

#pragma pack(1)
typedef struct
{
  int infoHeaderSize;
  int width;
  int height;
  short planes;
  short depth;
  int compression;
  int imageSize;
  int xRes;
  int yRes;
  int nColors;
  int importantColors;
} BMPinfoHeader;

typedef struct
{
  short id;
  int   fileSize;
  int   reserved;
  int   dataOffset;
} BMPfileHeader;

void CPixmap::ParseBMP ( FILE* fd )
{
    BMPfileHeader fh;
    BMPinfoHeader ih;
    size_t offset;
    int bpp;
    int rowsize, rowspace;
    int size;
    unsigned char * pixels;
    unsigned char * data;

    // Lee el FileHeader
    offset = fread ( &fh, 1, sizeof ( BMPfileHeader ), fd );
    if ( offset < (int)sizeof ( fh ) || fh.id != BMPmagic || fh.dataOffset < 30 )
        return;

    // Lee el InfoHeader
    offset += fread ( &ih, 1, sizeof ( ih ), fd );
    fseek ( fd, (long)( fh.dataOffset - offset ), SEEK_CUR );
  
    // Si hubo algun error en la lectura de las cabeceras termina
    if ( feof( fd ) || ferror ( fd ) )
        return;
    if ( ih.width < 1 || ih.height < 1 || ih.depth < 1 )
        return;
    if (ih.compression != 0 || ih.depth != 24 )
        return;

    bpp = ih.depth / 8;
    rowsize = (bpp * ih.width);
    rowspace = 4 - rowsize % 4;
    size = ih.height * rowsize;

    // Copia los pixels
    pixels = new unsigned char [ size ];
    data = pixels;
    if ( rowspace < 4 )
    {
        for ( int i = 0; i < ih.height; ++i, data += rowsize )
        {
            fread ( data, rowsize, 1, fd );
            fseek ( fd, rowspace, SEEK_CUR );
        }
    }
    else
    {
        for ( int i = 0; i < ih.height; ++i, data += rowsize )
        {
            fread ( data, rowsize, 1, fd );
        }
    }

    /* Convertimos los pixels de BGR a RGB */
    size = ih.width * ih.height;
    data = pixels;
    for ( int i = 0; i < size; ++i )
    {
        unsigned char b = data [ 0 ];
        data [ 0 ] = data [ 2 ];
        data [ 2 ] = b;

        data += 3;
    }

    m_iDepth = ih.depth / 8;
    m_iWidth = ih.width;
    m_iHeight = ih.height;
    m_pPixels = pixels;
}

