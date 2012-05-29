/*
 * Práctica de Gráficos por computador.
 *
 * CDefaultRenderer.h
 * Objetivo:  Renderizador por defecto.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CDefaultRenderer;

#include <vector>
#include "CWorld.h"
#include "IRenderer.h"
#include "CModel.h"
#include "CMatrix.h"
#include "CSky.h"

class CDefaultRenderer : public IRenderer
{
private:
    struct __VBORefs
    {
        unsigned int    uiVertexVBO;
        unsigned int    uiIndexVBO;
    };

private:
                                    CDefaultRenderer        ( );
public:
                                    ~CDefaultRenderer       ( );

    bool                            Initialize              ( );
    const char*                     GetErrorMessage         ( );

    void                            SetRenderingMode        ( IRenderer::RenderingMode eMode );
    inline void                     UseShaders              ( bool bUse ) { m_bUseShaders = bUse; }
    inline bool                     IsUsingShaders          ( ) const { return m_bUseShaders; }

    void                            BeginScene              ( );
    void                            Render                  ( CWorld& world );
    void                            EndScene                ( );

private:
    void                            SetupLighting           ( const CWorld& world );
	void							SetupCamera				( CCamera& camera );
    void							DrawCoordinateSystem	( );
	void							DrawEntities			( const std::list < CEntity * >& entities );

    /* Shaders */
    void                            LoadShaders             ( const wstring& szDir );
    bool                            LoadShader              ( const wstring& szFileName, unsigned int uiShader );
    void                            ActivateShaders         ( bool bActivate );

public:
    static CDefaultRenderer&        GetSingleton            ( );
    static CDefaultRenderer*        GetSingletonPtr         ( );

private:
    IRenderer::RenderingMode        m_eRenderingMode;
    std::string                     m_szErrorMessage;
    bool                            m_bVBOSupport;
    std::vector < __VBORefs >       m_vecVBOs;
    std::vector < unsigned int >    m_vecTextures;

    /* Shaders */
    unsigned int                    m_uiShaderProgram;
    bool                            m_bShaderSupport;
    bool                            m_bUseShaders;
};
