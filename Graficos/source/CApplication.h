/*
 * Práctica de Gráficos por computador.
 *
 * CApplication.h
 * Objetivo:  Instancia global para manejar la aplicación.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CApplication;

#include <list>
#include "main.h"
#include "CWorld.h"
#include "CGame.h"
#include "IRenderer.h"
#include "CController.h"
#include "IPulsable.h"
#include "CTimeCounter.h"

class CApplication
{
private:
                                CApplication            ( int iX = 300, int iY = 300, int iWidth = 500, int iHeight = 500 );
public:
							    ~CApplication           ( );

    void                        Initialize              ( int* argc, const char** argv );
    int                         Loop                    ( );

    inline void                 SetFPSLimit             ( unsigned int uiFPSLimit ) { m_uiFPSLimit = uiFPSLimit; }
    inline unsigned int         GetFPSLimit             ( ) const { return m_uiFPSLimit; }
    inline unsigned int         GetFPS                  ( ) const { return m_uiFPS; }

    void                        RequestPulses           ( IPulsable* pElement );
    void                        StopPulses              ( IPulsable* pElement );

    void                        SetVSync                ( bool bState ) const;

    inline void                 InvalidateScene         ( ) { m_bRedrawScene = true; }

    inline const IRenderer*     GetRenderer             ( ) const { return m_pRenderer; }

    inline CGame&               GetGame                 ( ) { return m_game; }

    inline int                  GetWidth                ( ) { return ( m_bFullscreen ? m_iWidthFS : m_iWidth ); }
    inline int                  GetHeight               ( ) { return ( m_bFullscreen ? m_iHeightFS : m_iHeight ); }

public:
    static CApplication&        GetSingleton            ( );
    static CApplication*        GetSingletonPtr         ( );

private:
    bool                        Exit                    ( const SControllerEvent& evt );
    bool                        ToggleFullscreen        ( const SControllerEvent& evt );
    bool                        ToggleRenderMode        ( const SControllerEvent& evt );

    /* Eventos de glut */
    static void                 staticDisplayFunc       ( );
    static void                 staticKeyboardFunc      ( unsigned char ucKey, int iX, int iY );
    static void                 staticKeyboardUpFunc    ( unsigned char ucKey, int iX, int iY );
    static void                 staticSpecialFunc       ( int iKey, int iX, int iY );
    static void                 staticSpecialUpFunc     ( int iKey, int iX, int iY );
    static void	                staticIdleFunc	        ( );
    static void                 staticReshapeFunc       ( int iWidth, int iHeight );
    static void                 staticMotionFunc        ( int iX, int iY );
    static void                 staticMouseFunc         ( int iButton, int iState, int iX, int iY );
    void                        DisplayFunc             ( );
    void                        KeyboardFunc            ( unsigned char ucKey, int iX, int iY );
    void                        KeyboardUpFunc          ( unsigned char ucKey, int iX, int iY );
    void                        SpecialFunc             ( int iKey, int iX, int iY );
    void                        SpecialUpFunc           ( int iKey, int iX, int iY );
    void                        IdleFunc                ( );
    void                        ReshapeFunc             ( int iWidth, int iHeight );
    void                        MotionFunc              ( int iX, int iY );
    void                        MouseFunc               ( int iButton, int iState, int iX, int iY );

    bool                        CheckFrameLimiter       ( );

private:
    unsigned long               m_ulLastTickCount;
    unsigned int                m_uiFPSLimit;
    unsigned int                m_uiFPS;
    unsigned int                m_uiNumFrames;
    bool                        m_bRedrawScene;
    CTimeCounter                m_FPSCounter;

    bool                        m_bFullscreen;
    int                         m_iX;
    int                         m_iY;
    int                         m_iWidth;
    int                         m_iHeight;
    int                         m_iWidthFS;
    int                         m_iHeightFS;

    IRenderer*                  m_pRenderer;
    std::list < IPulsable * >   m_listPulsableElements;

    CGame                       m_game;
};
