/*
 * Práctica de Gráficos por computador.
 *
 * CDirectory_win32.cpp
 * Objetivo:  Clase que permite abrir e iterar directorios para Win32.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

/* Funciones para navegar por directorios */
wstring CDirectory::GetCurrentDirectory ( )
{
    wstring szRet;
    wchar_t szCurDirectory [ 2048 ];
    ::GetCurrentDirectory ( sizeof ( szCurDirectory ), szCurDirectory );
    szRet.assign ( szCurDirectory );
    return szRet;
}

void CDirectory::SetCurrentDirectory ( const wstring& szDirectoryPath )
{
    ::SetCurrentDirectory ( szDirectoryPath.c_str ( ) );
}

/* Clase CDirectory */
CDirectory::CDirectory ( )
    : m_pHandle ( INVALID_HANDLE_VALUE )
{
}

CDirectory::CDirectory ( const wstring& szPath )
{
    Open ( szPath );
}

CDirectory::~CDirectory ( )
{
    Close ( );
}

void CDirectory::Close ( )
{
    if ( m_pHandle != INVALID_HANDLE_VALUE )
        FindClose ( m_pHandle );
}

bool CDirectory::Open ( const wstring& szPath )
{
    /* Por seguridad ... */
    Close ( );

    m_pHandle = FindFirstFile ( wstring( szPath + L"\\*" ).c_str(), &m_ffd );

    if ( m_pHandle == INVALID_HANDLE_VALUE )
    {
        return false;
    }

    return true;
}

bool CDirectory::IsOk ( ) const
{
    return ( m_pHandle != INVALID_HANDLE_VALUE );
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
    return wstring ( m_ffd.cFileName );
}

const CDirectory::EntryType CDirectory::CIterator::GetType ( ) const
{
    if ( m_ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
        return ENTRY_DIRECTORY;
    }
    else
    {
        return ENTRY_FILE;
    }
}
