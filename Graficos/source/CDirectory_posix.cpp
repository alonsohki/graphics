/*
 * Práctica de Gráficos por computador.
 *
 * CDirectory_posix.cpp
 * Objetivo:  Clase que permite abrir e iterar directorios, para POSIX.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "CDirectory.h"

/* Funciones para navegar por directorios */
wstring CDirectory::GetCurrentDirectory ( )
{
    std::string szRet_iso;
    wstring szRet;
    char szCurDirectory [ 2048 ];

    getcwd ( szCurDirectory, sizeof ( szCurDirectory ) );
    szRet_iso.assign ( szCurDirectory );
    szRet.assign ( szRet_iso.begin ( ), szRet_iso.end ( ) );

    return szRet;
}

void CDirectory::SetCurrentDirectory ( const wstring& szDirectoryPath )
{
    std::string szDirectoryPath_iso ( szDirectoryPath.begin ( ), szDirectoryPath.end ( ) );
    szDirectoryPath_iso.assign ( szDirectoryPath.begin ( ), szDirectoryPath.end ( ) );

    chdir ( szDirectoryPath_iso.c_str ( ) );
}


/* Clase CDirectory */
CDirectory::CDirectory ( )
	: m_pHandle ( 0 )
{
}

CDirectory::CDirectory ( const wstring& szPath )
  : m_pHandle ( 0 )
{
    Open ( szPath );
}

CDirectory::~CDirectory ( )
{
    Close ( );
}

void CDirectory::Close ( )
{
    if ( m_pHandle != 0 )
        closedir ( m_pHandle );
}

bool CDirectory::Open ( const wstring& szPath )
{
    /* Por seguridad ... */
    Close ( );

    std::string szPath_iso ( szPath.begin( ), szPath.end( ) );
    szPath_iso.assign ( szPath.begin( ), szPath.end( ) );

    m_pHandle = opendir ( szPath_iso.c_str() );

    if ( m_pHandle == 0 )
    {
        return false;
    }

    m_pDirent = readdir ( m_pHandle );

    return true;
}

bool CDirectory::IsOk ( ) const
{
    return ( m_pHandle != 0 );
}

CDirectory::CIterator CDirectory::Begin ( )
{
    return CDirectory::CIterator ( this );
}

CDirectory::CIterator CDirectory::End ( )
{
    return CDirectory::CIterator ( 0 );
}

/* Iteradores */
const wstring CDirectory::CIterator::GetName ( ) const
{
    std::string szTemp;
    szTemp.assign ( m_pDirent->d_name );
    wstring szRet ( szTemp.begin(), szTemp.end() );
    szRet.assign ( szTemp.begin(), szTemp.end() );
    return szRet;
}

const CDirectory::EntryType CDirectory::CIterator::GetType ( ) const
{
    if ( m_pDirent->d_type == DT_DIR )
    {
        return ENTRY_DIRECTORY;
    }
    else
    {
        return ENTRY_FILE;
    }
}
