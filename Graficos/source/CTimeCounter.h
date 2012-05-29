/*
 * Práctica de Gráficos por computador.
 *
 * CTimeCounter.h
 * Objetivo:  Obtener el tiempo que ha pasado entre dos comprobaciones con alta precisión (microsegundos).
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CTimeCounter;

#include "main.h"

class CTimeCounter
{
public:
    inline                      CTimeCounter    ( )
    {
        gettimeofday ( &m_last, 0 );
        gettimeofday ( &m_lastNow, 0 );
    }

    inline unsigned long   Query           ( bool bUpdate = true )
    {
        unsigned long ulResult;
        timeval last;
        timeval now;
        gettimeofday ( &now, 0 );

        m_lastNow = now;
        last = m_last;
        if ( bUpdate )
        {
            m_last = now;
        }

        ulResult = ( now.tv_sec - last.tv_sec ) * 1000000;
        if ( now.tv_usec < last.tv_usec )
        {
            ulResult -= last.tv_usec - now.tv_usec;
        }
        else
        {
            ulResult += now.tv_usec - last.tv_usec;
        }

        return ulResult;
    }

    inline void         Update          ( )
    {
        m_last = m_lastNow;
    }

private:
    timeval         m_last;
    timeval         m_lastNow;
};
