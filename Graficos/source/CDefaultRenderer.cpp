 /*
 * Práctica de Gráficos por computador.
 *
 * CDefaultRenderer.cpp
 * Objetivo:  Renderizador por defecto.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CDefaultRenderer::CDefaultRenderer ( )
    : m_eRenderingMode ( IRenderer::RENDER_TRIANGLES ),
      m_bVBOSupport ( false ),
      m_uiShaderProgram ( 0 ),
      m_bShaderSupport ( false ),
      m_bUseShaders ( true )
{
}

CDefaultRenderer::~CDefaultRenderer ( )
{
}

CDefaultRenderer* CDefaultRenderer::GetSingletonPtr ( )
{
	return &GetSingleton( );
}

CDefaultRenderer& CDefaultRenderer::GetSingleton ( )
{
	static CDefaultRenderer instance;
	return instance;
}

bool CDefaultRenderer::Initialize ( )
{
    GLenum eError = glewInit ( );
    if ( eError != GLEW_OK )
    {
        m_szErrorMessage.assign ( reinterpret_cast < const char * > ( glewGetErrorString ( eError ) ) );
        return false;
    }

    /* Bug en glew */
    if ( ! ( glGenBuffers == NULL || glBindBuffer == NULL || glBufferData == NULL ) )
    {
        m_bVBOSupport = true;
    }

    /* Activamos el test de profunidad y no dibujamos las caras ocultas */
    glEnable ( GL_DEPTH_TEST );
    glEnable ( GL_CULL_FACE );

    /* TODO: Test de iluminación, quitar cuanto antes */
    GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightModelfv ( GL_LIGHT_MODEL_AMBIENT, lmodel_ambient );

    if ( glCreateProgram != NULL && glCreateShader != NULL )
    {
        m_bShaderSupport = true;

        /* TODO: Esto debería cambiar... */
#ifdef WIN32
        LoadShaders ( L"../shaders" );
#else
        LoadShaders ( L"../../shaders" );
#endif
    }

    return true;
}

const char* CDefaultRenderer::GetErrorMessage ( )
{
    return m_szErrorMessage.c_str ( );
}

void CDefaultRenderer::SetRenderingMode ( IRenderer::RenderingMode eMode )
{
    m_eRenderingMode = eMode;
}

void CDefaultRenderer::BeginScene ( )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnableClientState ( GL_VERTEX_ARRAY );
    glEnableClientState ( GL_NORMAL_ARRAY );
    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

    glPolygonMode ( GL_FRONT, GL_FILL );
}

inline static GLenum GetNextLight ( GLenum eCurLight )
{
    switch ( eCurLight )
    {
        case GL_LIGHT0: return GL_LIGHT1;
        case GL_LIGHT1: return GL_LIGHT2;
        case GL_LIGHT2: return GL_LIGHT3;
        case GL_LIGHT3: return GL_LIGHT4;
        case GL_LIGHT4: return GL_LIGHT5;
        case GL_LIGHT5: return GL_LIGHT6;
        default: return GL_LIGHT7;
    }
}

inline void CDefaultRenderer::SetupLighting ( const CWorld& world )
{
    GLenum eCurLight = GL_LIGHT0;
    const std::vector < CLight >& vecLights = world.GetLights ( );

    const std::vector < CLight >::const_iterator& end = vecLights.end ( );
    for ( std::vector < CLight >::const_iterator i = vecLights.begin ( );
          i != end;
          ++i )
    {
        const CLight& light = ( *i );

        if ( light.IsEnabled ( ) )
        {
            glEnable  ( GL_LIGHTING );
            glEnable  ( eCurLight );
            glLightfv ( eCurLight, GL_AMBIENT,                  ( const GLfloat * )&( light.GetAmbient ( ) ) );
            glLightfv ( eCurLight, GL_DIFFUSE,                  ( const GLfloat * )&( light.GetDiffuse ( ) ) );
            glLightfv ( eCurLight, GL_SPECULAR,                 ( const GLfloat * )&( light.GetSpecular ( ) ) );
            glLightfv ( eCurLight, GL_POSITION,                 ( const GLfloat * )&( light.GetPosition ( ) ) );
            glLightfv ( eCurLight, GL_SPOT_DIRECTION,           ( const GLfloat * )&( light.GetSpotDirection ( ) ) );
            glLightf  ( eCurLight, GL_SPOT_EXPONENT,            light.GetSpotExponent ( ) );
            glLightf  ( eCurLight, GL_SPOT_CUTOFF,              light.GetSpotCutoff ( ) );
            glLightf  ( eCurLight, GL_CONSTANT_ATTENUATION,     light.GetConstantAttenuation ( ) );
            glLightf  ( eCurLight, GL_LINEAR_ATTENUATION,       light.GetLinearAttenuation ( ) );
            glLightf  ( eCurLight, GL_QUADRATIC_ATTENUATION,    light.GetQuadraticAttenuation ( ) );
            eCurLight = GetNextLight ( eCurLight );
        }
    }
}

inline void CDefaultRenderer::DrawCoordinateSystem ( )
{
#ifdef DRAW_COORDINATE_SYSTEM
    GLboolean bIsLightingActivated;
    glGetBooleanv ( GL_LIGHTING, &bIsLightingActivated );

    if ( bIsLightingActivated == GL_TRUE )
        glDisable ( GL_LIGHTING );

    /* Dibujamos el sistema de coordenadas */
    glBegin ( GL_LINES );
    {
        glColor3f ( 1.0f, 0.0f, 0.0f );
        glVertex3f ( 0.0f, 0.0f, 0.0f );
        glVertex3f ( 10000.0f, 0.0f, 0.0f );

        glColor3f ( 0.0f, 1.0f, 0.0f );
        glVertex3f ( 0.0f, 0.0f, 0.0f );
        glVertex3f ( 0.0f, 10000.0f, 0.0f );

        glColor3f ( 0.0f, 0.0f, 1.0f );
        glVertex3f ( 0.0f, 0.0f, 0.0f );
        glVertex3f ( 0.0f, 0.0f, -10000.0f );
    } glEnd ( );
    glColor3f ( 1.0f, 1.0f, 1.0f );

    if ( bIsLightingActivated == GL_TRUE )
        glEnable ( GL_LIGHTING );
#endif
}


inline void CDefaultRenderer::SetupCamera ( CCamera& camera )
{
	glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ( );
    glMultMatrixf ( camera.GetPerspectiveMatrix ( ).GetMatrix ( ) );

    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ( );
    glMultMatrixf ( camera.GetLookatMatrix ( ).GetMatrix ( ) );
}


void CDefaultRenderer::Render ( CWorld& world )
{
    /* Preparamos la iluminación */
    SetupLighting ( world );

    /* Preparamos la cámara */
    SetupCamera ( world.GetCamera ( ) );

    /* Dibujamos el sistema de coordenadas */
	DrawCoordinateSystem ( );

	/* Dibujamos todas las entidades del mundo */
    ActivateShaders ( true );
	DrawEntities ( world.GetEntities ( ) );
    ActivateShaders ( false );

    /* Dibujamos el crosshair ( apaño temporal ) */
    CApplication::GetSingleton ( ).GetGame ( ).GetCrosshair ( ).Render ( );
}

inline void CDefaultRenderer::ActivateShaders ( bool bActivate )
{
    if ( m_bUseShaders )
    {
        if ( m_bShaderSupport && m_uiShaderProgram != 0 )
        {
            if ( bActivate )
                glUseProgram ( m_uiShaderProgram );
            else
                glUseProgram ( 0 );
        }
    }
    else if ( m_bShaderSupport )
        glUseProgram ( 0 );
}

inline void CDefaultRenderer::DrawEntities ( const std::list < CEntity* >& entities )
{
    bool bTexturesEnabled;
    GLboolean bIsTexturesEnabled;
    GLboolean bIsLightingEnabled;
    glGetBooleanv ( GL_TEXTURE_2D, &bIsTexturesEnabled );
    glGetBooleanv ( GL_LIGHTING, &bIsLightingEnabled );

    bTexturesEnabled = bIsTexturesEnabled == GL_TRUE ? true : false;

    /* Iteramos por todas las entidades del mundo */
    const std::list < CEntity * >::const_iterator& end = entities.end ( );
    for ( std::list < CEntity * >::const_iterator i = entities.begin ( );
          i != end;
          ++i )
	{
        CEntity* pEntity = const_cast < CEntity * > ( *i );
        CEntityModel* pEntityModel = pEntity->GetEntityModel ( );

        if ( pEntity->IsVisible ( ) && pEntityModel )
        {
            if ( pEntity->UsesLighting ( ) == false && bIsLightingEnabled )
                glDisable ( GL_LIGHTING );

            glPushMatrix ( );
        
            const CModel* pModel = pEntityModel->GetModel ( );
            unsigned int uiModelID = pModel->GetID ( );

            /* Transformamos la entidad */
            glMultMatrixf ( pEntity->GetMatrix ( ).GetMatrix ( ) );


            /* Obtenemos los índices y vértices */
            const unsigned int* pIndices = pModel->GetIndices ( );
            const CVertex* pVertices = pModel->GetVertices ( );

            /* Comprobamos que tengamos soporte para VBOs */
            if ( m_bVBOSupport )
            {
                /* Comprobamos si existe un Vertex Buffer Object para este modelo.
                * De no ser así la creamos, en caso contrario usamos el anteriormente creado.
                */
                __VBORefs* VBORefs;
                unsigned int uiCurVectorSize = static_cast < unsigned int > ( m_vecVBOs.size ( ) );
                if ( uiCurVectorSize <= uiModelID || m_vecVBOs [ uiModelID ].uiVertexVBO == 0 )
                {
                    unsigned int uiNumVertices = pModel->GetNumVertices ( );
                    unsigned int uiNumIndices = pModel->GetNumFaces ( ) * 3;

                    /* Generamos el VBO para los vértices */
                    __VBORefs newRefs;
                    unsigned int uiNewVBOs [ 2 ];
                    glGenBuffers ( 2, uiNewVBOs );
                    newRefs.uiVertexVBO = uiNewVBOs [ 0 ];
                    newRefs.uiIndexVBO = uiNewVBOs [ 1 ];

                    /* Redimensionamos el vector de VBOs */
                    if ( uiCurVectorSize <= uiModelID )
                    {
                        m_vecVBOs.resize ( uiModelID + 1 );
                        for ( std::vector < __VBORefs >::iterator i = m_vecVBOs.begin ( ) + uiCurVectorSize;
                              i != m_vecVBOs.end ( );
                              ++i )
                        {
                            __VBORefs& cur = ( *i );
                            cur.uiVertexVBO = 0;
                            cur.uiIndexVBO = 0;
                        }
                    }

                    /* Copiamos el identificador del VBO al vector de VBOs */
                    m_vecVBOs [ uiModelID ] = newRefs;
                    VBORefs = &newRefs;

                    /* Copiamos los vértices al VBO recién creado */
                    glBindBuffer ( GL_ARRAY_BUFFER, newRefs.uiVertexVBO );
                    glBufferData ( GL_ARRAY_BUFFER, sizeof ( CVertex ) * uiNumVertices, pVertices, GL_STATIC_DRAW );

                    /* Copiamos los índices al VBO recién creado */
                    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, newRefs.uiIndexVBO );
                    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( unsigned int ) * uiNumIndices, pIndices, GL_STATIC_DRAW );
                    
                }
                else
                {
                    /* Ya existía un VBO para este modelo, la usamos */
                    VBORefs = &m_vecVBOs [ uiModelID ];
                }

                /* Inicialización ... */
                glBindBuffer ( GL_ARRAY_BUFFER, VBORefs->uiVertexVBO );
                glVertexPointer ( CVertex::POSITION_SIZE,
                                  GL_FLOAT,
                                  CVertex::POSITION_STRIDE,
                                  ((GLubyte *)NULL + (CVertex::POSITION_BEGIN)) );
                glNormalPointer ( GL_FLOAT,
                                  CVertex::NORMAL_STRIDE,
                                  ((GLubyte *)NULL + (CVertex::NORMAL_BEGIN)) );
                glTexCoordPointer ( CVertex::TEXTURE_SIZE,
                                    GL_FLOAT,
                                    CVertex::TEXTURE_STRIDE,
                                    ((GLubyte *)NULL + (CVertex::TEXTURE_BEGIN)) );

                glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, VBORefs->uiIndexVBO );
            }

            else
            {
                /* No tenemos soporte para VBOs */
                glVertexPointer ( CVertex::POSITION_SIZE,
                                  GL_FLOAT,
                                  CVertex::POSITION_STRIDE,
                                  ((GLubyte *)pVertices + (CVertex::POSITION_BEGIN)) );
                glNormalPointer ( GL_FLOAT,
                                  CVertex::NORMAL_STRIDE,
                                  ((GLubyte *)pVertices + (CVertex::NORMAL_BEGIN)) );
                glTexCoordPointer ( CVertex::TEXTURE_SIZE,
                                    GL_FLOAT,
                                    CVertex::TEXTURE_STRIDE,
                                    ((GLubyte *)pVertices + (CVertex::TEXTURE_BEGIN)) );
            }

            /* Dibujamos los submodelos */
            unsigned int uiNumChildren = pEntityModel->GetNumChildren ( );
            CEntityModel* pChildren = pEntityModel->GetChildren ( );

            switch ( m_eRenderingMode )
            {


            case IRenderer::RENDER_WIREFRAME:
                glPolygonMode ( GL_FRONT, GL_LINE );
                glDisable ( GL_LIGHTING );
            case IRenderer::RENDER_TRIANGLES:
            {
                /**
                ** Render de triángulos
                **/
                for ( unsigned int i = 0; i < uiNumChildren; ++i )
                {
                    CEntityModel* pCurEntityModel = &pChildren [ i ];
                    const CModel* pCurModel = pCurEntityModel->GetModel ( );

                    if ( ! pCurModel )
                        continue;

                    unsigned int uiNumFaces = pCurModel->GetNumFaces ( );
                    const unsigned int* puiIndices = pCurModel->GetIndices ( );
                    const CMaterial& material = pCurEntityModel->GetMaterial ( );

                    /* Cargamos los materiales */
                    glMaterialfv ( GL_FRONT, GL_AMBIENT, ( const GLfloat * )( &( material.GetAmbient ( ) ) ) );
                    glMaterialfv ( GL_FRONT, GL_DIFFUSE, ( const GLfloat * )( &( material.GetDiffuse ( ) ) ) );
                    glMaterialfv ( GL_FRONT, GL_SPECULAR, ( const GLfloat * )( &( material.GetSpecular ( ) ) ) );
                    glMaterialf  ( GL_FRONT, GL_SHININESS, material.GetShininess ( ) );
                    glMaterialfv ( GL_FRONT, GL_EMISSION, ( const GLfloat * )( &( material.GetEmission ( ) ) ) );

                    if ( material.HasTexture ( ) )
                    {
                        const CPixmap& texture = material.GetTexture ( );
                        unsigned int uiPixmapID = texture.GetID ( );

                        /* Habilitamos el uso de texturas */
                        if ( ! bTexturesEnabled )
                        {
                            bTexturesEnabled = true;
                            glEnable ( GL_TEXTURE_2D );
                        }

                        /* Comprobamos si existe una textura de OpenGL para este pixmap.
                         * Si no existe lo creamos.
                         */
                        unsigned int uiTextureID;
                        unsigned int uiTexturesSize = static_cast < unsigned int > ( m_vecTextures.size ( ) );

                        if ( uiTexturesSize <= uiPixmapID || m_vecTextures [ uiPixmapID ] == 0 )
                        {
                            /* No existe textura en OpenGL para este pixmap, así que generamos una */
                            glGenTextures ( 1, &uiTextureID );
                            glBindTexture( GL_TEXTURE_2D, uiTextureID );
                            glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
                            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
                            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                            
                            glTexImage2D ( GL_TEXTURE_2D, 0, 
                                           GL_RGB,
                                           texture.GetWidth ( ), texture.GetHeight ( ),
                                           0,
                                           GL_RGB, GL_UNSIGNED_BYTE, texture.GetPixels ( ) );

                            /* Redimensionamos el vector de IDs de textura si es necesario */
                            if ( uiTexturesSize <= uiPixmapID )
                            {
                                m_vecTextures.resize ( uiPixmapID + 1 );
                                for ( std::vector < unsigned int >::iterator i = m_vecTextures.begin ( ) + uiTexturesSize;
                                      i != m_vecTextures.end ( );
                                      ++i )
                                {
                                    unsigned int& cur = *i;
                                    cur = 0;
                                }
                            }

                            m_vecTextures [ uiPixmapID ] = uiTextureID;
                        }
                        else
                        {
                            glBindTexture ( GL_TEXTURE_2D, m_vecTextures [ uiPixmapID ] );
                        }
                    }
                    else if ( bTexturesEnabled )
                    {
                        bTexturesEnabled = false;
                        glDisable ( GL_TEXTURE_2D );
                    }

                    glPushMatrix ( );

                    /* Transformamos el submodelo */
                    glMultMatrixf ( pCurEntityModel->GetMatrix ( ).GetMatrix ( ) );

                    const GLvoid* pGLIndices;
                    if ( m_bVBOSupport )
                    {
                        pGLIndices = (const GLvoid *) ( (GLbyte *)puiIndices - (GLbyte *)pIndices );
                    }
                    else
                    {
                        pGLIndices = (const GLvoid *) puiIndices;
                    }

                    glDrawRangeElements ( GL_TRIANGLES,
                                          pCurModel->GetMinIndex ( ), pCurModel->GetMaxIndex ( ),
                                          uiNumFaces * 3,
                                          GL_UNSIGNED_INT,
                                          pGLIndices );

                    glPopMatrix ( );
                }

                break;
            }

            case IRenderer::RENDER_CLASSICAL:
            {
                /**
                 ** Render clásico: glBegin ( ) ... glEnd ( ) con líneas.
                 **/

                if ( bIsLightingEnabled == GL_TRUE )
                    glDisable ( GL_LIGHTING );

                for ( unsigned int i = 0; i < uiNumChildren; ++i )
                {
                    CEntityModel* pCurEntityModel = &pChildren [ i ];
                    const CModel* pCurModel = pCurEntityModel->GetModel ( );

                    if ( ! pCurModel )
                        continue;

                    unsigned int uiNumFaces = pCurModel->GetNumFaces ( );
                    const unsigned int* puiIndices = pCurModel->GetIndices ( );

                    glPushMatrix ( );

                    /* Transformamos el submodelo */
                    glMultMatrixf ( pCurEntityModel->GetMatrix ( ).GetMatrix ( ) );

                    for ( unsigned int j = 0; j < uiNumFaces; ++j )
                    {
                        glBegin ( GL_TRIANGLES );
                        {
                            for ( unsigned int k = 0; k < 3; ++k )
                            {
                                const CVertex& vertex = pVertices [ puiIndices [ ( j * 3 ) + k ] ];
                                const CVector& vecPosition = vertex.GetPosition ( );
                                const CVector& vecNormal = vertex.GetNormal ( );
                                glColor3f ( vecNormal.fX, vecNormal.fY, vecNormal.fZ );
                                glVertex3f ( vecPosition.fX, vecPosition.fY, vecPosition.fZ );
                            }
                        } glEnd ( );
                    }

                    glPopMatrix ( );
                }

                if ( bIsLightingEnabled )
                    glEnable ( GL_LIGHTING );

                break;
            }


            }

            glPopMatrix ( );

            if ( pEntity->UsesLighting ( ) == false && bIsLightingEnabled )
                glDisable ( GL_LIGHTING );
        }
    }


    /* Restauramos los estados anteriores */
    if ( bIsLightingEnabled )
        glEnable ( GL_LIGHTING );
    else
        glDisable ( GL_LIGHTING );
    if ( bIsTexturesEnabled )
        glEnable ( GL_TEXTURE_2D );
    else
        glDisable ( GL_TEXTURE_2D );
}

void CDefaultRenderer::EndScene ( )
{
    glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState ( GL_NORMAL_ARRAY );
    glDisableClientState ( GL_VERTEX_ARRAY );

    glDisable ( GL_LIGHTING );
    glDisable ( GL_LIGHT0 );
    glDisable ( GL_LIGHT1 );
    glDisable ( GL_LIGHT2 );
    glDisable ( GL_LIGHT3 );
    glDisable ( GL_LIGHT4 );
    glDisable ( GL_LIGHT5 );
    glDisable ( GL_LIGHT6 );
    glDisable ( GL_LIGHT7 );

    glDisable ( GL_TEXTURE_2D );

    glFlush ( );
}


/**
 ** Shaders
 **/
void CDefaultRenderer::LoadShaders ( const wstring& szDir )
{
    CDirectory dir ( szDir );

    /* Guardamos el directorio actual */
    wstring szCurDirectory = CDirectory::GetCurrentDirectory ( );
    CDirectory::SetCurrentDirectory ( szDir );

    m_uiShaderProgram = glCreateProgram ( );
    unsigned int uiShader;

    if ( dir.IsOk ( ) )
    {
        for ( CDirectory::CIterator entry = dir.Begin ( );
              entry != dir.End ( );
              ++entry )
        {
            if ( entry.GetType ( ) == CDirectory::ENTRY_FILE )
            {
                const wstring& szFileName = entry.GetName ( );
                size_t iExtPos = szFileName.rfind ( L"." );

                if ( iExtPos != wstring::npos )
                {
                    wstring szExt ( szFileName, iExtPos + 1 );

                    if ( szExt == L"vert" )
                    {
                        uiShader = glCreateShader ( GL_VERTEX_SHADER );
                    }
                    else if ( szExt == L"frag" )
                    {
                        uiShader = glCreateShader ( GL_FRAGMENT_SHADER );
                    }
                    else
                        continue;

                    if ( LoadShader ( szFileName, uiShader ) )
                    {
                        glAttachShader ( m_uiShaderProgram, uiShader );
                    }
                }
            }
        }
    }

    int iLinked;
    glLinkProgram ( m_uiShaderProgram );
    glGetProgramiv ( m_uiShaderProgram, GL_LINK_STATUS, &iLinked );
    if ( ! iLinked )
        m_uiShaderProgram = 0;

    CDirectory::SetCurrentDirectory ( szCurDirectory );
}

bool CDefaultRenderer::LoadShader ( const wstring& szFileName, unsigned int uiShader )
{
#ifdef WIN32
    std::fstream shaderFile ( szFileName.c_str( ), std::ios::in );
#else
    /* Linux no utiliza Unicode para acceder al sistema de ficheros, así que lo transformamos a ISO */
    std::string szFileName_iso ( szFileName.begin( ), szFileName.end( ) );
    szFileName_iso.assign ( szFileName.begin( ), szFileName.end( ) );
    std::fstream shaderFile ( szFileName_iso.c_str( ), std::ios::in );
#endif

    if ( shaderFile.fail ( ) )
        return false;

    char szSource [ 65536 ];
    shaderFile.read ( szSource, sizeof ( szSource ) );

    GLchar* pszSource[] = { szSource };

    glShaderSource ( uiShader, 1, (const GLchar **)pszSource, NULL );
    glCompileShader ( uiShader );

    int iCompiled;
    glGetShaderiv ( uiShader, GL_COMPILE_STATUS, &iCompiled );

    if ( ! iCompiled )
        return false;

    return true;
}
