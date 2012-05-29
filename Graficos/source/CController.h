/*
 * Práctica de Gráficos por computador.
 *
 * CController.h
 * Objetivo:  Capa de abstracción a los eventos de teclado de glut.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CController;

/* Estructura para el parámetro de los callback del controlador */
struct SControllerEvent
{
    enum EventType
    {
        KEY,
        SPECIAL,
        MOUSE,
        MOTION
    };

    SControllerEvent ( )
        : uiKey ( 0 ),
          uiButton ( 0 ),
          uiDirection ( 0 ),
          iX ( 0 ),
          iY ( 0 )
    {
    }

    EventType    eType;
    unsigned int uiKey;
    unsigned int uiButton;
    unsigned int uiDirection;
    int          iX;
    int          iY;
};


/* Macro para construír eventos para el controlador */
#define CONTROLLER_CALLBACK CCallback < bool, const SControllerEvent& >


class CController
{
public:
    static CController*         GetSingletonPtr     ( );
    static CController&         GetSingleton        ( );

public:
    enum
    {
        DIR_DOWN            = 1,
        DIR_UP              = 2,
        DIR_BOTH            = 3,


        SPECIAL_F1          = 1,
        SPECIAL_F2          = 2,
        SPECIAL_F3          = 3,
        SPECIAL_F4          = 4,
        SPECIAL_F5          = 5,
        SPECIAL_F6          = 6,
        SPECIAL_F7          = 7,
        SPECIAL_F8          = 8,
        SPECIAL_F9          = 9,
        SPECIAL_F10         = 10,
        SPECIAL_F11         = 11,
        SPECIAL_F12         = 12,

        SPECIAL_ARROW_LEFT  = 100,
        SPECIAL_ARROW_UP    = 101,
        SPECIAL_ARROW_RIGHT = 102,
        SPECIAL_ARROW_DOWN  = 103,

        SPECIAL_PAGE_UP     = 104,
        SPECIAL_PAGE_DOWN   = 105,
        SPECIAL_START       = 106,
        SPECIAL_END         = 107,
        SPECIAL_INSERT      = 108,

        KEY_SPACE           = 10,
        KEY_ENTER           = 13,
        KEY_ESCAPE          = 27,
    };

private:
                                CController             ( );
public:
                                ~CController            ( );

    void                        KeyPress                ( unsigned int uiKey, unsigned int uiDirection );
    void                        SpecialKeyPress         ( unsigned int uiKey, unsigned int uiDirection );
    void                        MouseButtonPress        ( unsigned int uiButton, unsigned int uiDirection );
    void                        MouseMotion             ( int iX, int iY );

    void                        BindKey                 ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback );
    void                        BindSpecialKey          ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback );
    void                        UnbindKey               ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback );
    void                        UnbindSpecialKey        ( unsigned int uiKey, unsigned int uiDirections, CONTROLLER_CALLBACK callback );
    void                        BindMouseButton         ( unsigned int uiButton, unsigned int uiDirections, CONTROLLER_CALLBACK callback );
    void                        UnbindMouseButton       ( unsigned int uiButton, unsigned int uiDirections, CONTROLLER_CALLBACK callback );
    void                        BindMouseMotion         ( CONTROLLER_CALLBACK callback );
    void                        UnbindMouseMotion       ( CONTROLLER_CALLBACK callback );

private:
    /* Estructuras para almacenar los callbacks */
    struct __ControllerKeyCallbacks
    {
        unsigned int                                uiDirections;
        CONTROLLER_CALLBACK*                        callback;
    };
    struct __ControllerMouseCallbacks
    {
        unsigned int                                uiDirections;
        CONTROLLER_CALLBACK*                        callback;
    };
    struct __ControllerMotionCallbacks
    {
        CONTROLLER_CALLBACK*                        callback;
    };



    std::list < __ControllerKeyCallbacks >                  m_listKeyCallbacks [ 256 ];
    std::list < __ControllerKeyCallbacks >                  m_listSpecialKeyCallbacks [ 256 ];
    std::list < __ControllerMouseCallbacks >                m_listMouseClickedCallbacks [ 5 ];
    std::list < __ControllerMotionCallbacks >               m_listMouseMotionCallbacks;
    unsigned int                                            m_uiKeyStates [ 256 ];
    unsigned int                                            m_uiSpecialKeyStates [ 256 ];
    unsigned int                                            m_uiMouseStates [ 256 ];
};
