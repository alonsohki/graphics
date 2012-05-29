/*
 * Práctica de Gráficos por computador.
 *
 * CGameScript.h
 * Objetivo:  Guión del juego.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CGameScript;

#include "CGame.h"
#include "IPulsable.h"

class CGameScript : public IPulsable
{
    friend class CGame;
    typedef void ( CGameScript::*PulseRef_pfn ) ( void );

private:
                    CGameScript         ( );
                    CGameScript         ( CGame* pGame );
public:
                    ~CGameScript        ( );
private:
    virtual void    DoPulse             ( );
    void            SetGame             ( CGame* pGame );
    void            Start               ( );
    void            Stop                ( );


private:
    /* Niveles */
    void            CreateLevel1        ( );
    void            DestroyLevel1       ( );
    void            PulseLevel1         ( );

    PulseRef_pfn    m_levelPulses [ 1 ];

private:
    CGame*          m_pGame;
    unsigned int    m_uiLevel;
};
