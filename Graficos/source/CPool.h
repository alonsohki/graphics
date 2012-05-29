/*
 * Práctica de Gráficos por computador.
 *
 * CPool.h
 * Objetivo:  Pool para reserva de memoria para estructuras del mismo tipo que
 *            están contínuamente reservándose y liberándose, para así poder
 *            evitar fragmentación de memoria.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

template < class T > class CPool;

#include <malloc.h>

template < class T >
class CPool
{
private:
    struct SPoolNode
    {
        void*       p;
        SPoolNode*  pNext;
        SPoolNode*  pNextFree;
    };

public:
    inline          CPool           ( unsigned int uiBucketSize = 1024 )
        : m_uiBucketSize ( uiBucketSize ),
          m_pBuckets ( 0 ),
          m_pFreeNodes ( 0 )
    {
        CreateBucket ( );
    }

    inline          ~CPool          ( )
    {
        SPoolNode* pNextBucket;

        for ( SPoolNode* pCurBucket = m_pBuckets;
              pCurBucket != 0;
              pCurBucket = pNextBucket )
        {
            pNextBucket = pCurBucket->pNext;
            free ( pCurBucket );
        }
    }

    inline void     CreateBucket    ( )
    {
        /* Reservamos toda la memoria en un sólo malloc para minimizar al máximo la fragmentación */
        SPoolNode* pNewNode = ( SPoolNode * ) malloc ( ( sizeof ( SPoolNode ) * ( m_uiBucketSize + 1 ) ) +
                                                       ( sizeof ( T ) * m_uiBucketSize ) );
        T* pElements = ( T * ) ( (BYTE *)pNewNode + sizeof ( SPoolNode ) * ( m_uiBucketSize + 1 ) );
        SPoolNode* pNodes = pNewNode + 1;

        /* Enlazamos este nuevo bucket de nodos */
        pNewNode->p = 0;
        pNewNode->pNext = m_pBuckets;
        pNewNode->pNextFree = 0;
        m_pBuckets = pNewNode;

        /* Creamos la lista de nodos */
        for ( unsigned int i = 0; i < m_uiBucketSize; ++i )
        {
            pNodes->p = &pElements [ m_uiBucketSize - i - 1 ];
            pNodes->pNextFree = m_pFreeNodes;
            pNodes->pNext = static_cast < SPoolNode * > ( pNewNode->p );
            pNewNode->p = pNodes;
            m_pFreeNodes = pNodes;

            ++pNodes;
        }
    }

    inline T*       Alloc           ( size_t size )
    {
        if ( size != sizeof ( T ) )
        {
            return ( T* ) malloc ( size );
        }
        else
        {
            if ( ! m_pFreeNodes )
            {
                CreateBucket ( );
            }

            T* pRet = static_cast < T* > ( m_pFreeNodes->p );
            m_pFreeNodes = m_pFreeNodes->pNextFree;

            return pRet;
        }
    }

    inline void     Free            ( T* p )
    {
        for ( SPoolNode* pCurrentBucket = m_pBuckets;
              pCurrentBucket != 0;
              pCurrentBucket = pCurrentBucket->pNext )
        {
            /* Como todos los elementos de este bucket se reservaron secuencialmente,
             * Podemos calcular diréctamente la posición dentro del bucket del nodo
             * que hace referencia al elemento que vamos a liberar.
             */
            T* pElementsBase = static_cast < T * > ( static_cast < SPoolNode * > ( pCurrentBucket->p )->p );
            if ( p >= pElementsBase && p < pElementsBase + m_uiBucketSize )
            {
                unsigned int uiPosition = static_cast < unsigned int > ( p - pElementsBase );
                SPoolNode* pNode = pCurrentBucket + m_uiBucketSize - uiPosition;

                pNode->pNextFree = m_pFreeNodes;
                m_pFreeNodes = pNode;
                return;
            }
        }

        /* El puntero a liberar no está dentro de nuestros buckets, así que lo liberamos diréctamente */
        free ( p );
    }

private:
    unsigned int        m_uiBucketSize;
    SPoolNode*          m_pBuckets;
    SPoolNode*          m_pFreeNodes;
};
