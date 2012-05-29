/*
 * Práctica de Gráficos por computador.
 *
 * CDirectory_posix.h
 * Objetivo:  Clase que permite abrir e iterar directorios, para POSIX.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include <dirent.h>
#include <string.h>

class CDirectory
{
public:
    /* Funciones estáticas para movernos por el árbol
     * de directorios con independencia del sistema operativo.
     */
    static wstring      GetCurrentDirectory ( );
    static void         SetCurrentDirectory ( const wstring& szDirectoryPath );

    /* Tipos de entradas de directorios */
public:
    enum EntryType
    {
        ENTRY_DIRECTORY,
        ENTRY_FILE
    };



    /* Iterador de entradas de directorios */
public:
    class CIterator
    {
        friend class CDirectory;

    private:
        const CDirectory*           m_pDirectory;
        dirent*                     m_pDirent;

    private:
        inline                      CIterator           ( const CDirectory* pDirectory )
            : m_pDirectory ( pDirectory )
        {
            if ( m_pDirectory )
                m_pDirent = m_pDirectory->m_pDirent;
        }
    public:
        inline                      ~CIterator          ( ) { }

    public:
        /* Operadores */
        inline CDirectory::CIterator&       operator=           ( const CDirectory::CIterator& Right )
        {
            if ( &Right != this )
            {
                m_pDirectory = Right.m_pDirectory;
                m_pDirent = Right.m_pDirent;
            }
            return *this;
        }

        inline CDirectory::CIterator&       operator++          ( )
        {
            /* Preincremento (++iterator) */
            if ( m_pDirectory )
            {
                m_pDirent = readdir ( m_pDirectory->m_pHandle );
                if ( ! m_pDirent )
                    m_pDirectory = 0;
            }
            return *this;
        }

        inline CDirectory::CIterator        operator++          ( int )
        {
            /* Postincremento (iterator++) */
            CDirectory::CIterator tmp = *this;
            ++*this;
            return tmp;
        }

        inline bool                         operator==          ( const CDirectory::CIterator& Right ) const
        {
            return ( m_pDirectory == Right.m_pDirectory &&
                     (
                        m_pDirectory == 0 ||
                        !memcmp ( m_pDirent, Right.m_pDirent, sizeof( dirent ) )
                     )
                   );
        }

        inline bool                         operator!=          ( const CDirectory::CIterator& Right ) const
        {
            return ( m_pDirectory != Right.m_pDirectory ||
                     (
                        m_pDirectory != 0 &&
                        memcmp ( m_pDirent, Right.m_pDirent, sizeof( dirent ) )
                     )
                   );
        }


        /* Acceso a la información de la entrada de directorio */
        const wstring                       GetName             ( ) const;
        const CDirectory::EntryType         GetType             ( ) const;
    };


    /* Clase CDirectory */
    friend class CDirectory::CIterator;

public:
                            CDirectory          ( );
                            CDirectory          ( const wstring& szPath );
                            ~CDirectory         ( );

    bool                    Open                ( const wstring& szPath );
    bool                    IsOk                ( ) const;

    CDirectory::CIterator   Begin               ( );
    CDirectory::CIterator   End                 ( );

private:
    void                    Close               ( );

private:
    DIR*                    m_pHandle;
    dirent*                 m_pDirent;
};
