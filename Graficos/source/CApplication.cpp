/*
 * Práctica de Gráficos por computador.
 *
 * CApplication.cpp
 * Objetivo:  Instancia global para manejar la aplicación.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

/* Singleton */
CApplication& CApplication::GetSingleton ( )
{
    return *GetSingletonPtr ( );
}

CApplication* CApplication::GetSingletonPtr ( )
{
    static CApplication* pSingleton = 0;

    if ( pSingleton == 0 )
    {
        pSingleton = new CApplication ( );
    }

    return pSingleton;
}


CApplication::CApplication ( int iX, int iY, int iWidth, int iHeight )
    : m_ulLastTickCount ( 0 ),
      m_uiFPSLimit ( 99999 ),
      m_uiFPS ( 0 ),
      m_uiNumFrames ( 0 ),
      m_bRedrawScene ( false ),
      m_bFullscreen ( false ),
      m_iX ( iX ),
      m_iY ( iY ),
      m_iWidth ( iWidth ),
      m_iHeight ( iHeight )
{
}

CApplication::~CApplication ( )
{
}

void CApplication::Initialize ( int* argc, const char** argv )
{
    /* Inicializar OpenGL */
    glutInit ( argc, const_cast < char** > ( argv ) );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize ( m_iWidth, m_iHeight );
    glutInitWindowPosition ( m_iX, m_iY );
    glutCreateWindow ( WINDOW_TITLE );

    /* Registrar las funciones que manejarán los eventos de glut */
    glutDisplayFunc ( CApplication::staticDisplayFunc );
    glutKeyboardFunc ( CApplication::staticKeyboardFunc );
    glutKeyboardUpFunc ( CApplication::staticKeyboardUpFunc );
    glutSpecialFunc ( CApplication::staticSpecialFunc );
    glutSpecialUpFunc ( CApplication::staticSpecialUpFunc );
    glutIdleFunc ( CApplication::staticIdleFunc );
    glutReshapeFunc ( CApplication::staticReshapeFunc );
    glutMotionFunc ( CApplication::staticMotionFunc );
    glutPassiveMotionFunc ( CApplication::staticMotionFunc );
    glutMouseFunc ( CApplication::staticMouseFunc );

    /* Ocultamos el puntero del ratón */
    glutSetCursor ( GLUT_CURSOR_NONE );
    
    /* No nos interesan las repeticiones de teclado al mantener teclas pulsadas */
    glutIgnoreKeyRepeat ( true );

    /* Establecer el color de borrado (negro) */
    glClearColor ( 0.03f, 0.03f, 0.03f, 1.0f );

    /* Desactivamos el V-Sync */
    SetVSync ( false );

    /* Inicializamos el renderer */
    m_pRenderer = CDefaultRenderer::GetSingletonPtr ( );
    if ( ! m_pRenderer->Initialize ( ) )
    {
        printf ( "Error inicializando el renderer: %s\n", m_pRenderer->GetErrorMessage ( ) );
        exit ( EXIT_FAILURE );
    }

    /* Introducimos el juego a la lista de elementos que pueden recibir pulsos */
    RequestPulses ( &m_game );


    CController& controller = CController::GetSingleton ( );

    /* Habilitamos el cierre de la aplicación usando la tecla ESC o ALT+F4 */
    controller.BindKey ( CController::KEY_ESCAPE, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::Exit, this ) );
    controller.BindSpecialKey ( CController::SPECIAL_F4, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::Exit, this ) );

    /* Habilitamos la (des)activación de la pantalla completa usando ALT+ENTER */
    controller.BindKey ( CController::KEY_ENTER, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::ToggleFullscreen, this ) );

    /* Habilitamos el cambio de modo de renderizado */
    controller.BindSpecialKey ( CController::SPECIAL_F1, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::ToggleRenderMode, this ) );
    controller.BindSpecialKey ( CController::SPECIAL_F2, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::ToggleRenderMode, this ) );
    controller.BindSpecialKey ( CController::SPECIAL_F3, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::ToggleRenderMode, this ) );
    controller.BindSpecialKey ( CController::SPECIAL_F11, CController::DIR_DOWN, CONTROLLER_CALLBACK ( &CApplication::ToggleRenderMode, this ) );

    /* Cargar los modelos
     * TODO: Huh... Esto debería cambiar
     */
#ifdef WIN32
    CModelManager::GetSingleton ( ).Initialize ( L"../models" );
    CSpaceshipPropsManager::GetSingleton ( ).Initialize ( L"../data" );
#else
    CModelManager::GetSingleton ( ).Initialize ( L"../../models" );
    CSpaceshipPropsManager::GetSingleton ( ).Initialize ( L"../../data" );
#endif

    /* Iniciar el juego */
    m_game.Start ( );
}

int CApplication::Loop ( )
{
    glutMainLoop ( );
    return EXIT_SUCCESS;
}


/* Funciones de eventos de glut */
void CApplication::DisplayFunc ( )
{
    /* Capturamos el ratón */
    glutWarpPointer ( m_iWidth / 2, m_iHeight / 2 );

    /* Realizamos un pulso a todos los elementos de la aplicación */
    const std::list < IPulsable * >::iterator& end = m_listPulsableElements.end ( );
    for ( std::list < IPulsable * >::iterator i = m_listPulsableElements.begin ( );
          i != end;
          ++i )
    {
        ( *i )->DoPulse ( );
    }

    /* Si el frame limiter no nos permite dibujar la escena, la función
     * de idle se encargará de invalidar el frame de nuevo, así que
     * retornar sin hacer nada. En caso contrario, marcar la escena para
     * que no sea redibujada (no es necesario).
     */
    if ( ! CheckFrameLimiter ( ) )
    {
        m_bRedrawScene = true;
        return;
    }
    m_bRedrawScene = false;
	
    m_pRenderer->BeginScene ( );
    m_pRenderer->Render ( m_game.GetWorld ( ) );
    m_pRenderer->EndScene ( );

    glutSwapBuffers ( );

    /* Actualizar el contador de frames por segundo */
    ++m_uiNumFrames;

    unsigned long ulMicrosecondsSpent = m_FPSCounter.Query ( false );

    if ( ulMicrosecondsSpent > 1000000 )
    {
        m_FPSCounter.Update ( );

        m_uiFPS = static_cast < unsigned int > ( ( 1000000 * m_uiNumFrames ) / ulMicrosecondsSpent );
        m_uiNumFrames = 0;

#ifdef SHOWFPS
        /* Actualizamos el título de la ventana con los FPS */
        char szTitle [ 2048 ];
#ifdef WIN32
        /* Jodido Visual Studio con sus _function ... */
        _snprintf_s ( szTitle, sizeof ( szTitle ), sizeof ( szTitle ), WINDOW_TITLE " - %u FPS", m_uiFPS );
#else
        snprintf ( szTitle, sizeof( szTitle ), WINDOW_TITLE " - %u FPS", m_uiFPS );
#endif
        glutSetWindowTitle ( szTitle );
#endif
    }
}

void CApplication::IdleFunc ( )
{
    /* Comprobamos si tenemos que redibujar la escena (la actual ya no es válida). */
    if ( true || m_bRedrawScene )
    {
        glutPostRedisplay ( );
    }
}

/* Eventos de teclado de glut */
void CApplication::KeyboardFunc ( unsigned char ucKey, int iX, int iY )
{
    CController::GetSingleton ( ).KeyPress ( static_cast < unsigned int > ( ucKey ), static_cast < unsigned int > ( CController::DIR_DOWN ) );
}

void CApplication::KeyboardUpFunc ( unsigned char ucKey, int iX, int iY )
{
    CController::GetSingleton ( ).KeyPress ( static_cast < unsigned int > ( ucKey ), static_cast < unsigned int > ( CController::DIR_UP ) );
}

void CApplication::SpecialFunc ( int iKey, int iX, int iY )
{
    CController::GetSingleton ( ).SpecialKeyPress ( static_cast < unsigned int > ( iKey ), static_cast < unsigned int > ( CController::DIR_DOWN ) );
}

void CApplication::SpecialUpFunc ( int iKey, int iX, int iY )
{
    CController::GetSingleton ( ).SpecialKeyPress ( static_cast < unsigned int > ( iKey ), static_cast < unsigned int > ( CController::DIR_UP ) );
}

/* Eventos de ratón */
void CApplication::MotionFunc ( int iX, int iY )
{
    iX -= m_iWidth / 2;
    iY -= m_iHeight / 2;

    if ( iX != 0 || iY != 0 )
    {
        CController::GetSingleton ( ).MouseMotion ( iX, iY );
    }
}

void CApplication::MouseFunc ( int iButton, int iState, int iX, int iY )
{
    if ( iState == GLUT_DOWN )
        iState = CController::DIR_DOWN;
    else
        iState = CController::DIR_UP;
    CController::GetSingleton ( ).MouseButtonPress ( static_cast < unsigned int > ( iButton ), static_cast < unsigned int > ( iState ) );
}

/* Eventos de ventana */
void CApplication::ReshapeFunc ( int iWidth, int iHeight )
{
    if ( !m_bFullscreen )
    {
        m_iWidth = iWidth;
        m_iHeight = iHeight;
    }
    else
    {
        m_iWidthFS = iWidth;
        m_iHeightFS = iHeight;
    }
    glViewport ( 0, 0, iWidth, iHeight );
    m_game.GetWorld ( ).GetCamera ( ).SetAspect ( iWidth / (float)iHeight );
}



bool CApplication::Exit ( const SControllerEvent& evt )
{
    if ( evt.uiKey == CController::KEY_ESCAPE ||
        ( evt.uiKey == CController::SPECIAL_F4 && glutGetModifiers ( ) & GLUT_ACTIVE_ALT ) )
    {
        exit ( EXIT_SUCCESS );
    }

    return true;
}

bool CApplication::ToggleFullscreen ( const SControllerEvent& )
{
    if ( glutGetModifiers ( ) & GLUT_ACTIVE_ALT )
    {
        m_bFullscreen = !m_bFullscreen;

        if ( !m_bFullscreen )
        {
            glutPositionWindow ( m_iX, m_iY );
            glutReshapeWindow ( m_iWidth, m_iHeight );
        }
        else
        {
            glutFullScreen ( );
        }
    }

    return true;
}

bool CApplication::ToggleRenderMode ( const SControllerEvent& evt )
{
    switch ( evt.uiKey )
    {
        case CController::SPECIAL_F1:
        {
            m_pRenderer->SetRenderingMode ( IRenderer::RENDER_TRIANGLES );
            break;
        }
        case CController::SPECIAL_F2:
        {
            m_pRenderer->SetRenderingMode ( IRenderer::RENDER_WIREFRAME );
            break;
        }
        case CController::SPECIAL_F3:
        {
            m_pRenderer->SetRenderingMode ( IRenderer::RENDER_CLASSICAL );
            break;
        }
        case CController::SPECIAL_F11:
        {
            CDefaultRenderer* pRenderer = dynamic_cast < CDefaultRenderer * > ( m_pRenderer );
            if ( pRenderer )
            {
                pRenderer->UseShaders ( ! pRenderer->IsUsingShaders ( ) );
            }
            break;
        }
    }

    return true;
}

/* Funciones estáticas interfaces de los eventos de glut */
void CApplication::staticDisplayFunc ( )
{
    CApplication::GetSingleton ( ).DisplayFunc ( );
}
void CApplication::staticKeyboardFunc ( unsigned char ucKey, int iX, int iY )
{
    CApplication::GetSingleton ( ).KeyboardFunc ( ucKey, iX, iY );
}
void CApplication::staticKeyboardUpFunc ( unsigned char ucKey, int iX, int iY )
{
    CApplication::GetSingleton ( ).KeyboardUpFunc ( ucKey, iX, iY );
}
void CApplication::staticSpecialFunc ( int iKey, int iX, int iY )
{
    CApplication::GetSingleton ( ).SpecialFunc ( iKey, iX, iY );
}
void CApplication::staticSpecialUpFunc ( int iKey, int iX, int iY )
{
    CApplication::GetSingleton ( ).SpecialUpFunc ( iKey, iX, iY );
}
void CApplication::staticIdleFunc ( )
{
	CApplication::GetSingleton ( ).IdleFunc ( );
}
void CApplication::staticReshapeFunc ( int iWidth, int iHeight )
{
    CApplication::GetSingleton ( ).ReshapeFunc ( iWidth, iHeight );
}
void CApplication::staticMotionFunc ( int iX, int iY )
{
    CApplication::GetSingleton ( ).MotionFunc ( iX, iY );
}
void CApplication::staticMouseFunc ( int iButton, int iState, int iX, int iY )
{
    CApplication::GetSingleton ( ).MouseFunc ( iButton, iState, iX, iY );
}



bool CApplication::CheckFrameLimiter ( )
{
#ifdef WIN32
	unsigned long ulTickCount = GetTickCount ( )
#else /* Posix */
	timespec ts;
	clock_gettime ( CLOCK_MONOTONIC, &ts );
	unsigned long ulTickCount = ( ts.tv_sec * 1000 ) + ( ts.tv_nsec / 1000000 );
#endif
        ;

    bool bCanDrawScene = false;

    /* Si el último "tick count" es mayor que el actual,
       es que el ciclo se ha reiniciado por no tener espacio
       para más en 32 bits. Esto ocurre más o menos cada 47 días
       desde el último reinicio. Al ser una situación excepcional
       simplemente dibujar el siguiente frame */
    if ( ulTickCount >= m_ulLastTickCount )
    {
        unsigned long ulMinFrameDelay = 1000 / m_uiFPSLimit; /* En milisegundos */

        if ( ( ulTickCount - m_ulLastTickCount ) >= ulMinFrameDelay )
        {
            m_ulLastTickCount = ulTickCount;
            bCanDrawScene = true;
        }
    }
    else
    {
        m_ulLastTickCount = ulTickCount;
        bCanDrawScene = true;
    }

    return bCanDrawScene;
}

void CApplication::SetVSync ( bool bSync ) const
{
#ifdef WIN32
    /* Extraído de http://www.gamedev.net/community/forums/topic.asp?topic_id=317721 */
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	if( wglSwapIntervalEXT )
		wglSwapIntervalEXT(bSync);
#endif
}

void CApplication::RequestPulses ( IPulsable* pElement )
{
    m_listPulsableElements.push_back ( pElement );
}

void CApplication::StopPulses ( IPulsable* pElement )
{
    m_listPulsableElements.remove ( pElement );
}
