/*
 * Práctica de Gráficos por computador.
 *
 * CController.cpp
 * Objetivo:  Capa de abstracción a los eventos de teclado de glut.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

/* Singleton */
CController* CController::GetSingletonPtr ( )
{
    static CController* pInstance = 0;

    if ( pInstance == 0 )
    {
        pInstance = new CController ( );
    }

    return pInstance;
}

CController& CController::GetSingleton ( )
{
    return *GetSingletonPtr ( );
}



CController::CController ( )
{
    for ( unsigned int i = 0; i < 256; ++i )
    {
        m_uiKeyStates [ i ] = CController::DIR_UP;
        m_uiSpecialKeyStates [ i ] = CController::DIR_UP;
        m_uiMouseStates [ i ] = CController::DIR_UP;
    }
}

CController::~CController ( )
{
    /* Eliminamos todos los callback creados */
    for ( unsigned int i = 0; i < 256; ++i )
    {
        if ( i < 5 )
        {
            for ( std::list < __ControllerMouseCallbacks >::iterator j = m_listMouseClickedCallbacks [ i ].begin ( );
                  j != m_listMouseClickedCallbacks [ i ].end ( );
                  ++j )
            {
                delete ( (*j).callback );
            }
        }

        for ( std::list < __ControllerKeyCallbacks >::iterator j = m_listKeyCallbacks [ i ].begin ( );
              j != m_listKeyCallbacks [ i ].end ( );
              ++j )
        {
            delete ( (*j).callback );
        }

        for ( std::list < __ControllerKeyCallbacks >::iterator j = m_listSpecialKeyCallbacks [ i ].begin ( );
              j != m_listSpecialKeyCallbacks [ i ].end ( );
              ++j )
        {
            delete ( (*j).callback );
        }
    }

    for ( std::list < __ControllerMotionCallbacks >::iterator j = m_listMouseMotionCallbacks.begin ( );
          j != m_listMouseMotionCallbacks.end ( );
          ++j )
    {
        delete ( (*j).callback );
    }
}

void CController::KeyPress ( unsigned int uiKey, unsigned int uiState )
{
    if ( uiKey > 255 )
        return;

    if ( m_uiKeyStates [ uiKey ] != uiState )
    {
        SControllerEvent evt;
        evt.uiKey = uiKey;
        evt.uiDirection = uiState;
        evt.eType = SControllerEvent::KEY;

        m_uiKeyStates [ uiKey ] = uiState;

        const std::list < __ControllerKeyCallbacks >::const_iterator& end = m_listKeyCallbacks [ uiKey ].end ( );
        for ( std::list < __ControllerKeyCallbacks >::const_iterator i = m_listKeyCallbacks [ uiKey ].begin ( );
              i != end;
              ++i )
        {
            const __ControllerKeyCallbacks& cur = (*i);
            if ( cur.uiDirections & uiState )
            {
                ( *cur.callback ) ( evt );
            }
        }
    }
}

void CController::SpecialKeyPress ( unsigned int uiKey, unsigned int uiState )
{
    if ( uiKey > 255 )
        return;

    if ( m_uiSpecialKeyStates [ uiKey ] != uiState )
    {
        SControllerEvent evt;
        evt.uiKey = uiKey;
        evt.uiDirection = uiState;
        evt.eType = SControllerEvent::SPECIAL;
        m_uiSpecialKeyStates [ uiKey ] = uiState;

        const std::list < __ControllerKeyCallbacks >::const_iterator& end = m_listSpecialKeyCallbacks [ uiKey ].end ( );
        for ( std::list < __ControllerKeyCallbacks >::const_iterator i = m_listSpecialKeyCallbacks [ uiKey ].begin ( );
              i != end;
              ++i )
        {
            const __ControllerKeyCallbacks& cur = (*i);
            if ( cur.uiDirections & uiState )
                ( *cur.callback ) ( evt );
        }
    }
}

void CController::MouseButtonPress ( unsigned int uiButton, unsigned int uiState )
{
    if ( uiButton > 5 )
        return;

    if ( m_uiMouseStates [ uiButton ] != uiState )
    {
        SControllerEvent evt;
        evt.uiButton = uiButton;
        evt.uiDirection = uiState;
        evt.eType = SControllerEvent::MOUSE;
        m_uiMouseStates [ uiButton ] = uiState;

        const std::list < __ControllerMouseCallbacks >::const_iterator& end = m_listMouseClickedCallbacks [ uiButton ].end ( );
        for ( std::list < __ControllerMouseCallbacks >::const_iterator i = m_listMouseClickedCallbacks [ uiButton ].begin ( );
              i != end;
              ++i )
        {
            const __ControllerMouseCallbacks& cur = (*i);
            if ( cur.uiDirections & uiState )
                ( *cur.callback ) ( evt );
        }
    }    
}

void CController::MouseMotion ( int iX, int iY )
{
    SControllerEvent evt;
    evt.iX = iX;
    evt.iY = iY;
    evt.eType = SControllerEvent::MOTION;

    const std::list < __ControllerMotionCallbacks >::const_iterator& end = m_listMouseMotionCallbacks.end ( );
    for ( std::list < __ControllerMotionCallbacks >::const_iterator i = m_listMouseMotionCallbacks.begin ( );
          i != end;
          ++i )
    {
        ( *(*i).callback ) ( evt );
    }
}


/* Funciones para añadir y borrar eventos */
void CController::BindKey ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback )
{
    if ( uiKey < 256 )
    {
        __ControllerKeyCallbacks newCallback;
        newCallback.callback = new CONTROLLER_CALLBACK ( callback );
        newCallback.uiDirections = uiDirections;

        m_listKeyCallbacks [ uiKey ].push_back ( newCallback );
    }
}

void CController::BindSpecialKey ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback )
{
    if ( uiKey < 256 )
    {
        __ControllerKeyCallbacks newCallback;
        newCallback.callback = new CONTROLLER_CALLBACK ( callback );
        newCallback.uiDirections = uiDirections;

        m_listSpecialKeyCallbacks [ uiKey ].push_back ( newCallback );
    }
}

void CController::UnbindKey ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback )
{
    if ( uiKey < 256 )
    {
        for ( std::list < __ControllerKeyCallbacks >::iterator i = m_listKeyCallbacks [ uiKey ].begin ( );
              i != m_listKeyCallbacks [ uiKey ].end ( );
              ++i )
        {
            const __ControllerKeyCallbacks& cur = (*i);
            if ( cur.uiDirections == uiDirections && *(cur.callback) == callback )
            {
                delete cur.callback;
                m_listKeyCallbacks [ uiKey ].erase ( i );
            }
        }
    }
}

void CController::UnbindSpecialKey ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback )
{
    if ( uiKey < 256 )
    {
        for ( std::list < __ControllerKeyCallbacks >::iterator i = m_listSpecialKeyCallbacks [ uiKey ].begin ( );
              i != m_listSpecialKeyCallbacks [ uiKey ].end ( );
              ++i )
        {
            const __ControllerKeyCallbacks& cur = (*i);
            if ( cur.uiDirections == uiDirections && *(cur.callback) == callback )
            {
                delete cur.callback;
                m_listSpecialKeyCallbacks [ uiKey ].erase ( i );
            }
        }
    }
}

void CController::BindMouseButton ( unsigned int uiButton, unsigned int uiDirections, CONTROLLER_CALLBACK callback )
{
    if ( uiButton < 5 )
    {
        __ControllerMouseCallbacks newCallback;
        newCallback.callback = new CONTROLLER_CALLBACK ( callback );
        newCallback.uiDirections = uiDirections;
        m_listMouseClickedCallbacks [ uiButton ].push_back ( newCallback );
    }
}

void CController::UnbindMouseButton ( unsigned int uiButton, unsigned int uiDirections, CONTROLLER_CALLBACK callback )
{
    if ( uiButton < 5 )
    {
        for ( std::list < __ControllerMouseCallbacks >::iterator i = m_listMouseClickedCallbacks [ uiButton ].begin ( );
              i != m_listMouseClickedCallbacks [ uiButton ].end ( );
              ++i )
        {
            const __ControllerMouseCallbacks& cur = (*i);
            if ( cur.uiDirections == uiDirections && *(cur.callback) == callback )
            {
                delete cur.callback;
                m_listMouseClickedCallbacks [ uiButton ].erase ( i );
            }
        }   
    }
}

void CController::BindMouseMotion ( CONTROLLER_CALLBACK callback )
{
    __ControllerMotionCallbacks newCallback;
    newCallback.callback = new CONTROLLER_CALLBACK ( callback );
    m_listMouseMotionCallbacks.push_back ( newCallback );
}

void CController::UnbindMouseMotion ( CONTROLLER_CALLBACK callback )
{
    for ( std::list < __ControllerMotionCallbacks >::iterator i = m_listMouseMotionCallbacks.begin ( );
          i != m_listMouseMotionCallbacks.end ( );
          ++i )
    {
        const __ControllerMotionCallbacks& cur = (*i);
        if ( *(cur.callback) == callback )
        {
            delete cur.callback;
            m_listMouseMotionCallbacks.erase ( i );
        }
    }
}
