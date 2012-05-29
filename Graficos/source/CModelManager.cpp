/*
 * Práctica de Gráficos por computador.
 *
 * CModelManager.cpp
 * Objetivo:  Gestor de modelos.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

/* Singleton */
CModelManager& CModelManager::GetSingleton ( )
{
    return *GetSingletonPtr ( );
}

CModelManager* CModelManager::GetSingletonPtr ( )
{
    static CModelManager* pSingleton = 0;

    if ( pSingleton == 0 )
    {
        pSingleton = new CModelManager ( );
    }

    return pSingleton;
}


CModelManager::CModelManager ( )
{
}

CModelManager::~CModelManager ( )
{
    for ( std::list< CModel* >::iterator i = m_modelsList.begin ( );
          i != m_modelsList.end ( );
          i++ )
    {
        delete ( *i );
    }
}

void CModelManager::Initialize ( const wstring& szModelsPath )
{
    /* Iterar el directorio de los modelos y cargar cada uno de ellos */
    CDirectory dir ( szModelsPath );

    /* Guardamos el directorio actual */
    wstring szCurDirectory = CDirectory::GetCurrentDirectory ( );

    if ( dir.IsOk ( ) )
    {
        for ( CDirectory::CIterator entry = dir.Begin ( );
              entry != dir.End ( );
              ++entry )
        {
            if ( entry.GetType ( ) == CDirectory::ENTRY_FILE )
            {
                const wstring& szFileName = entry.GetName ( );
                size_t iExtPos = szFileName.rfind ( L"." );

                if ( iExtPos != wstring::npos )
                {
                    if ( szFileName.substr ( iExtPos ) == L".obj" )
                    {
                        CDirectory::SetCurrentDirectory ( szModelsPath );

                        CModel* pNewModel = new CModel ( szFileName );
                        m_modelsList.push_back ( pNewModel );

                        CDirectory::SetCurrentDirectory ( szCurDirectory );
                    }
                }
            }
        }
    }
}

const CModel* CModelManager::GetModel ( const wstring& szModelName )
{
    for ( std::list< CModel* >::const_iterator i = m_modelsList.begin ( );
          i != m_modelsList.end ( );
          ++i )
    {
        if ( szModelName == ( *i )->GetName ( ) )
        {
            return ( *i );
        }
    }

    return 0;
}

unsigned int CModelManager::GetFreeModelID ( )
{
    static unsigned int uiModelID = 0;
    return uiModelID++;
}
