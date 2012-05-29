/*
 * Práctica de Gráficos por computador.
 *
 * CCrosshair.cpp
 * Objetivo:  Diana para apuntar a la hora de disparar.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CCrosshair::CCrosshair ( )
{
    m_iScreenWidth = 0;
    m_iScreenHeight = 0;
    m_uiDisplayList = 0;
}

CCrosshair::~CCrosshair ( )
{
}

void CCrosshair::DoPulse ( )
{
    CApplication& app = CApplication::GetSingleton ( );
    int iWidth = app.GetWidth ( );
    int iHeight = app.GetHeight ( );

    if ( m_iScreenWidth != iWidth || m_iScreenHeight != iHeight )
    {
        /* Recalculamos la posición del crosshair en pantalla */
        m_iScreenWidth = iWidth;
        m_iScreenHeight = iHeight;

        m_iCrosshairX = ( int )( iWidth * CROSSHAIR_X );
        m_iCrosshairY = ( int )( iHeight * ( 1.0f - CROSSHAIR_Y ) );


        /* Generamos la display list (macro) para el dibujado del crosshair ( esto no debería ir aquí ) */
        if ( m_uiDisplayList != 0 )
        {
            glDeleteLists ( m_uiDisplayList, 1 );
        }
        m_uiDisplayList = glGenLists ( 1 );

        glNewList ( m_uiDisplayList, GL_COMPILE );
        glDisable ( GL_LIGHTING );
        glDisable ( GL_DEPTH_TEST );

        glMatrixMode ( GL_PROJECTION );
        glPushMatrix ( );
        glLoadIdentity ( );
        glOrtho ( 0.0f, m_iScreenWidth, 0.0f, m_iScreenHeight, 0.0f, 1.0f );

        glMatrixMode ( GL_MODELVIEW );
        glPushMatrix ( );
        glLoadIdentity ( );

        glColor3f ( 1.0f, 1.0f, 1.0f );
        glBegin ( GL_LINES );
        {
            glVertex2i ( m_iCrosshairX - 4, m_iCrosshairY );
            glVertex2i ( m_iCrosshairX - 8, m_iCrosshairY );

            glVertex2i ( m_iCrosshairX + 4, m_iCrosshairY );
            glVertex2i ( m_iCrosshairX + 8, m_iCrosshairY );

            glVertex2i ( m_iCrosshairX, m_iCrosshairY - 4 );
            glVertex2i ( m_iCrosshairX, m_iCrosshairY - 8 );

            glVertex2i ( m_iCrosshairX, m_iCrosshairY + 4 );
            glVertex2i ( m_iCrosshairX, m_iCrosshairY + 8 );
        } glEnd ( );

        glPopMatrix ( );

        glMatrixMode ( GL_PROJECTION );
        glPopMatrix ( );
        glMatrixMode ( GL_MODELVIEW );

        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_LIGHTING );

        glEndList ( );
    }
}

void CCrosshair::Render ( )
{
    if ( glIsList ( m_uiDisplayList ) )
        glCallList ( m_uiDisplayList );
}

bool CCrosshair::GetTarget ( CVector& vecPosition, CEntity** ppEntity )
{
    CCamera& camera = CApplication::GetSingleton ( ).GetGame ( ).GetCamera ( );
    float* fProjection = camera.GetPerspectiveMatrix ( ).GetMatrix ( );
    float* fModelview = camera.GetLookatMatrix ( ).GetMatrix ( );

    /* Obtenemos la profundidad de lo que esté apuntando el crosshair ( debería hacerse con colisiones cuando estén soportadas ) */
    float fWinZ;
    glReadPixels( m_iCrosshairX, m_iCrosshairY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fWinZ );

    int iViewport [ 4 ] = { 0, 0, m_iScreenWidth, m_iScreenHeight };
    double dModelview [ 16 ];
    double dProjection [ 16 ];
    double worldX, worldY, worldZ;

    for ( unsigned int i = 0; i < 16; ++i )
    {
        dModelview [ i ] = fModelview [ i ];
        dProjection [ i ] = fProjection [ i ];
    }

    bool bCanUnproject = gluUnProject ( m_iCrosshairX, m_iCrosshairY, fWinZ, dModelview, dProjection, iViewport, &worldX, &worldY, &worldZ ) ? true : false;
    if ( !bCanUnproject )
        return false;

    vecPosition.fX = worldX;
    vecPosition.fY = worldY;
    vecPosition.fZ = worldZ;

    /* Cuando tengamos soporte para colisiones, estableceremos ppEntity */
    if ( ppEntity != 0 )
        *ppEntity = 0;

    return true;
}
