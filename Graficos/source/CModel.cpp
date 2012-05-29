/*
 * Práctica de Gráficos por computador.
 *
 * CModel.cpp
 * Objetivo:  Clase que procesa los ficheros de los modelos disponibles y crea
 *            una interfaz para ellos, además de dibujarlos a demanda.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

/**
 ** Constructor.
 ** Extrae el nombre del modelo a partir del fichero que se va a interpretar,
 ** y acto seguido interpreta el fichero.
 **/
CModel::CModel ( const wstring& szModelPath )
    : m_szModelName ( L"" ),
      m_pColModel ( 0 )
{
    /* Asignamos un ID único a este modelo */
    m_uiModelID = CModelManager::GetFreeModelID ( );

    /* Extraemos el nombre del modelo a partir del nombre del fichero */
    size_t iExtPos = szModelPath.rfind ( L".obj" );
    if ( iExtPos != wstring::npos )
    {
        m_szModelName = szModelPath.substr ( 0, iExtPos );
    }
    else
    {
        m_szModelName = szModelPath;
    }

    /* Cargamos el modelo */
    LoadModel ( szModelPath );
}

CModel::CModel ( )
    : m_szModelName ( L"" ),
      m_pChildModels ( 0 ),
      m_pVertices ( 0 ),
      m_pColModel ( 0 )
{
}

CModel::~CModel ( )
{
    free ( m_pVertices );
    if ( m_pChildModels )
        delete [] m_pChildModels;
    if ( m_pColModel )
        delete m_pColModel;
}


/* Macro para saltar los espacios en una cadena. Se usará para el intérprete */
#define JUMP_SPACES(p) while ( ( *(p) == ' ' || *(p) == '\r' || *(p) == '\n' || *(p) == '\t' ) && *(p) != '\0' ) ++(p)


/* Función para cargar información de los materiales del modelo */
void CModel::LoadMaterials ( const wstring& szMaterialsPath, std::map < std::string, CMaterial >& mapMaterials )
{
    char line [ 2048 ];
    char* p;
    char* p2;
    CMaterial* pCurMaterial = 0;

    /* Abrimos el fichero .mtl para interpretarlo */
#ifdef WIN32
    std::fstream materialsFile ( szMaterialsPath.c_str( ), std::ios::in );
#else
    /* Linux no utiliza Unicode para acceder al sistema de ficheros, así que lo transformamos a ISO */
    std::string szMaterialsPath_iso ( szMaterialsPath.begin( ), szMaterialsPath.end( ) );
    szMaterialsPath_iso.assign ( szMaterialsPath.begin( ), szMaterialsPath.end( ) );
    std::fstream materialsFile ( szMaterialsPath_iso.c_str( ), std::ios::in );
#endif

#define IS_COMMAND(l,c) strlen(l) > strlen(c)+1 && !strncmp(l, c, strlen(c)) && l[strlen(c)] == ' '

    if ( ! materialsFile.fail ( ) )
    {
        while ( ! materialsFile.eof ( ) )
        {
            /* Lo leemos línea a línea */
            materialsFile.getline ( line, sizeof ( line ) );

            if ( *line != '#' ) /* No leemos comentarios */
            {
                /* Saltamos espacios en blanco */
                p = line;
                JUMP_SPACES(p);

                p2 = p + strlen ( p ) - 1;
                while ( p2 >= p && ( *p2 == '\r' || *p2 == '\n' ) )
                {
                    *p2 = '\0';
                    --p2;
                }
                
                if ( *p == '\0' )
                    continue;

                if ( IS_COMMAND(p, "newmtl") )
                {
                    /* Definición de un nuevo material, extraemos el nombre y creamos una nueva entrada */
                    p += 7;
                    JUMP_SPACES(p);

                    std::string szMaterialName ( p );
                    std::map < std::string, CMaterial >::iterator it = mapMaterials.find ( szMaterialName );
                    if ( it == mapMaterials.end ( ) )
                    {
                        /* Creamos un nuevo material */
                        mapMaterials.insert ( std::map < std::string, CMaterial >::value_type ( szMaterialName, CMaterial ( ) ) );
                        it = mapMaterials.find ( szMaterialName );
                    }
                    pCurMaterial = &( ( *it ).second );
                }

                else if ( IS_COMMAND(p, "Ka") )
                {
                    /* Leemos la componente ambiental */
                    if ( pCurMaterial )
                    {
                        CColor temp;
                        float fTemp;
                        p += 3;
                        JUMP_SPACES(p);

                        if ( *p != '\0' )
                        {
                            fTemp = atof ( p );
                            temp.SetRed ( fTemp );

                            p = strchr ( p, ' ' );
                            if ( p && *p != '\0' )
                            {
                                JUMP_SPACES(p);
                                fTemp = atof ( p );
                                temp.SetGreen ( fTemp );

                                p = strchr ( p, ' ' );
                                if ( p && *p != '\0' )
                                {
                                    JUMP_SPACES(p);
                                    fTemp = atof ( p );
                                    temp.SetBlue ( fTemp );
                                }
                            }
                        }

                        pCurMaterial->SetAmbient ( temp );
                    }
                }

                else if ( IS_COMMAND(p, "Kd") )
                {
                    /* Leemos la componente difusa */
                    if ( pCurMaterial )
                    {
                        CColor temp;
                        float fTemp;
                        p += 3;
                        JUMP_SPACES(p);

                        if ( *p != '\0' )
                        {
                            fTemp = atof ( p );
                            temp.SetRed ( fTemp );

                            p = strchr ( p, ' ' );
                            if ( p && *p != '\0' )
                            {
                                JUMP_SPACES(p);
                                fTemp = atof ( p );
                                temp.SetGreen ( fTemp );

                                p = strchr ( p, ' ' );
                                if ( p && *p != '\0' )
                                {
                                    JUMP_SPACES(p);
                                    fTemp = atof ( p );
                                    temp.SetBlue ( fTemp );
                                }
                            }
                        }

                        pCurMaterial->SetDiffuse ( temp );
                    }
                }

                else if ( IS_COMMAND(p, "Ks") )
                {
                    /* Leemos la componente especular */
                    if ( pCurMaterial )
                    {
                        CColor temp;
                        float fTemp;
                        p += 3;
                        JUMP_SPACES(p);

                        if ( *p != '\0' )
                        {
                            fTemp = atof ( p );
                            temp.SetRed ( fTemp );

                            p = strchr ( p, ' ' );
                            if ( p && *p != '\0' )
                            {
                                JUMP_SPACES(p);
                                fTemp = atof ( p );
                                temp.SetGreen ( fTemp );

                                p = strchr ( p, ' ' );
                                if ( p && *p != '\0' )
                                {
                                    JUMP_SPACES(p);
                                    fTemp = atof ( p );
                                    temp.SetBlue ( fTemp );
                                }
                            }
                        }

                        pCurMaterial->SetSpecular ( temp );
                    }
                }

                else if ( IS_COMMAND(p, "Ke") )
                {
                    /* Leemos la componente emisiva */
                    if ( pCurMaterial )
                    {
                        CColor temp;
                        float fTemp;
                        p += 3;
                        JUMP_SPACES(p);

                        if ( *p != '\0' )
                        {
                            fTemp = atof ( p );
                            temp.SetRed ( fTemp );

                            p = strchr ( p, ' ' );
                            if ( p && *p != '\0' )
                            {
                                JUMP_SPACES(p);
                                fTemp = atof ( p );
                                temp.SetGreen ( fTemp );

                                p = strchr ( p, ' ' );
                                if ( p && *p != '\0' )
                                {
                                    JUMP_SPACES(p);
                                    fTemp = atof ( p );
                                    temp.SetBlue ( fTemp );
                                }
                            }
                        }

                        pCurMaterial->SetEmission ( temp );
                    }
                }

                else if ( IS_COMMAND(p, "Ns") )
                {
                    /* Leemos el exponente de brillo de este material */
                    if ( pCurMaterial )
                    {
                        float fShininess;
                        p += 3;
                        JUMP_SPACES(p);

                        if ( *p != '\0' )
                        {
                            fShininess = atof ( p );
                            pCurMaterial->SetShininess ( fShininess );
                        }
                    }
                }

                else if ( IS_COMMAND(p, "d") || IS_COMMAND(p, "Tr") )
                {
                    /* Leemos la información de transparencia */
                    if ( pCurMaterial )
                    {
                        float fAlpha;
                        if ( *p == 'd' )
                            p += 2;
                        else
                            p += 3;
                        JUMP_SPACES(p);

                        if ( *p != '\0' )
                        {
                            fAlpha = atof ( p );
                            pCurMaterial->SetAlpha ( fAlpha );
                        }
                    }
                }

                else if ( IS_COMMAND(p, "map_Kd" ) )
                {
                    /* Leemos la textura */
                    if ( pCurMaterial )
                    {
                        p += 7;
                        std::string szTemp ( p );
                        wstring szFileName ( szTemp.begin ( ), szTemp.end ( ) );
                        szFileName.assign ( szTemp.begin ( ), szTemp.end ( ) );

                        CPixmap texture ( szFileName );
                        if ( texture.IsOk ( ) )
                            pCurMaterial->SetTexture ( texture );
                    }
                }
            }
        }
    }

#undef IS_COMMAND
}


/* Función para interpretar un modelo (.obj) */
void CModel::LoadModel ( const wstring& szModelPath )
{
    char line [ 2048 ];
    char* p;
    char* p2;

    /* Abrimos el fichero del modelo para interpretarlo */
#ifdef WIN32
    std::fstream modelFile ( szModelPath.c_str( ), std::ios::in );
#else
    /* Linux no utiliza Unicode para acceder al sistema de ficheros, así que lo transformamos a ISO */
    std::string szModelPath_iso ( szModelPath.begin( ), szModelPath.end( ) );
    szModelPath_iso.assign ( szModelPath.begin( ), szModelPath.end( ) );
    std::fstream modelFile ( szModelPath_iso.c_str( ), std::ios::in );
#endif

    if ( ! modelFile.fail ( ) )
    {
        /* Declaramos las estructuras temporales que usaremos para volcar la información
         * en un formato fácilmente legible en memoria.
         */
        std::vector < CVector > vecVertices;
        std::vector < CVector > vecTextures;
        std::vector < CVector > vecNormals;
        std::vector < unsigned int > vecVertexIndices;
        std::vector < unsigned int > vecTextureIndices;
        std::vector < unsigned int > vecNormalIndices;
        std::map < std::string, CMaterial > mapMaterials;

        std::vector < __GeometryData > vecGeometries;

        /* pCurGeometry apuntará a la geometría para la que se están definiendo caras en este momento */
        __GeometryData* pCurGeometry = 0;

        /**
         ** Interpretamos el fichero por líneas.
         **/
        while ( ! modelFile.eof ( ) )
        {
            modelFile.getline ( line, sizeof ( line ) );

            if ( *line != '#' ) /* No leemos comentarios */
            {
                /* Saltamos espacios en blanco */
                p = line;
                JUMP_SPACES(p);

                p2 = p + strlen ( p ) - 1;
                while ( p2 >= p && ( *p2 == '\r' || *p2 == '\n' ) )
                {
                    *p2 = '\0';
                    --p2;
                }
                
                if ( *p == '\0' )
                    continue;

                switch ( *p )
                {
                    /**
                     ** Vértices
                     **/
                    case 'v':
                    {
                        ++p;

                        /* Hay distintos tipos de información para vértices:
                         * v ( posición )
                         * vt ( datos de la textura )
                         * vn ( normal )
                         */
                        switch ( *p )
                        {
                            case ' ':
                            {
                                /* Vértices */
                                CVector temp;
                                ++p;
                                JUMP_SPACES(p);

                                if ( *p == '\0' )
                                {
                                    vecVertices.push_back ( temp );
                                    break;
                                }

                                temp.fX = atof ( p );
                                p = strchr ( p, ' ' );
                                if ( p != 0 ) JUMP_SPACES(p);
                                if ( !p || *p == '\0' ) break;

                                temp.fY = atof ( p );
                                p = strchr ( p, ' ' );
                                if ( p != 0 ) JUMP_SPACES(p);
                                if ( !p || *p == '\0' ) break;

                                temp.fZ = atof ( p );

                                vecVertices.push_back ( temp );

                                break;
                            }

                            case 't':
                            {
                                /* Texturas */
                                CVector temp;
                                ++p;
                                JUMP_SPACES(p);

                                if ( *p == '\0' )
                                {
                                    vecTextures.push_back ( temp );
                                    break;
                                }

                                temp.fX = atof ( p );
                                p = strchr ( p, ' ' );
                                if ( p != 0 ) JUMP_SPACES(p);
                                if ( !p || *p == '\0' ) break;

                                temp.fY = atof ( p );
                                temp.fZ = 0.0f;

                                vecTextures.push_back ( temp );

                                break;
                            }

                            case 'n':
                            {
                                /* Normales */
                                CVector temp;
                                ++p;
                                JUMP_SPACES(p);

                                if ( *p == '\0' )
                                {
                                    vecNormals.push_back ( temp );
                                    break;
                                }

                                temp.fX = atof ( p );
                                p = strchr ( p, ' ' );
                                if ( p != 0 ) JUMP_SPACES(p);
                                if ( !p || *p == '\0' ) break;

                                temp.fY = atof ( p );
                                p = strchr ( p, ' ' );
                                if ( p != 0 ) JUMP_SPACES(p);
                                if ( !p || *p == '\0' ) break;

                                temp.fZ = atof ( p );

                                vecNormals.push_back ( temp );

                                break;
                            }
                        }

                        break;
                    }

                    /**
                     ** Caras
                     **/
                    case 'f':
                    {
                        if ( pCurGeometry == 0 )
                        {
                            /* Nunca nos han dado una geometría, así que creamos una por defecto */
                            __GeometryData newGeometry;
                            newGeometry.szName = "";
                            newGeometry.pMaterial = 0;
                            vecGeometries.push_back ( newGeometry );
                            pCurGeometry = & ( vecGeometries.back ( ) );
                        }

                        /* Marcamos en qué posición de los vectores de índices para vértices, texturas y normales
                         * empiezan los índices que definen esta cara.
                         */
                        __FaceData data;
                        data.uiVertexIdx = static_cast < unsigned int > ( vecVertexIndices.size ( ) );
                        data.uiTextureIdx = static_cast < unsigned int > ( vecTextureIndices.size ( ) );
                        data.uiNormalIdx = static_cast < unsigned int > ( vecNormalIndices.size ( ) );
                        data.uiRealVertexIdx = 0;
                        data.uiCount = 0;

                        /* Saltamos espacios en blanco */
                        ++p;
                        JUMP_SPACES(p);

                        /* Leemos todas las caras */
                        while ( *p != '\0' )
                        {
                            unsigned int uiVertexIdx;
                            long lVertexIdx = strtol ( p, &p2, 10 );

                            if ( lVertexIdx > 0 )
                            {
                                uiVertexIdx = static_cast < unsigned int > ( lVertexIdx );
                            }
                            else
                            {
                                uiVertexIdx = static_cast < unsigned int > ( vecVertices.size ( ) + lVertexIdx + 1 );
                            }
                            vecVertexIndices.push_back ( static_cast < unsigned int > ( uiVertexIdx ) );

                            p = p2;
                            JUMP_SPACES(p);

                            if ( *p == '/' )
                            {
                                ++p;

                                if ( *p == '\0' || *p == '/' )
                                {
                                    /* No hay datos de la textura */
                                    vecTextureIndices.push_back ( 0 );
                                }
                                else
                                {
                                    /* Textura */
                                    unsigned int uiTextureIdx;
                                    long lTextureIdx = strtol ( p, &p2, 10 );

                                    if ( lTextureIdx > 0 )
                                    {
                                        uiTextureIdx = static_cast < unsigned int > ( lTextureIdx );
                                    }
                                    else
                                    {
                                        uiTextureIdx = static_cast < unsigned int > ( vecTextures.size ( ) + lTextureIdx + 1 );
                                    }
                                    vecTextureIndices.push_back ( uiTextureIdx );
   
                                    p = p2;
                                    JUMP_SPACES(p);
                                }

                                if ( *p == '/' )
                                {
                                    ++p;

                                    if ( *p == ' ' || *p == '\0' )
                                    {
                                        /* No hay datos de la normal */
                                        vecNormalIndices.push_back ( 0 );
                                    }
                                    else
                                    {
                                        /* Normal */
                                        unsigned int uiNormalIdx;
                                        long lNormalIdx = strtol ( p, &p2, 10 );

                                        if ( lNormalIdx > 0 )
                                        {
                                            uiNormalIdx = static_cast < unsigned int > ( lNormalIdx );
                                        }
                                        else
                                        {
                                            uiNormalIdx = static_cast < unsigned int > ( vecNormals.size ( ) + lNormalIdx + 1 );
                                        }
                                        vecNormalIndices.push_back ( uiNormalIdx );

                                        p = p2;
                                        JUMP_SPACES(p);
                                    }
                                }
                                else
                                {
                                    vecNormalIndices.push_back ( 0 );
                                }
                            }
                            else
                            {
                                vecTextureIndices.push_back ( 0 );
                                vecNormalIndices.push_back ( 0 );
                            }

                            data.uiCount++;
                        }


                        if ( data.uiCount > 3 )
                        {
                            /* Triangulamos la cara */
                            unsigned int uiNumberOfTriangles = data.uiCount - 2;

                            /* Truncamos el polígono actual a tres vértices */
                            data.uiCount = 3;
                            pCurGeometry->vecFaces.push_back ( data );

                            for ( unsigned int i = 1; i < uiNumberOfTriangles; ++i )
                            {
                                __FaceData newData;
                                newData.uiCount = 3;
                                newData.uiVertexIdx = static_cast < unsigned int > ( vecVertexIndices.size ( ) );
                                newData.uiTextureIdx = static_cast < unsigned int > ( vecTextureIndices.size ( ) );
                                newData.uiNormalIdx = static_cast < unsigned int > ( vecNormalIndices.size ( ) );
                                newData.uiRealVertexIdx = 0;

                                /* Primer vértice del nuevo triángulo */
                                vecVertexIndices.push_back ( vecVertexIndices [ data.uiVertexIdx ] );
                                vecTextureIndices.push_back ( vecTextureIndices [ data.uiTextureIdx ] );
                                vecNormalIndices.push_back ( vecNormalIndices [ data.uiNormalIdx ] );

                                /* Segundo vértice */
                                vecVertexIndices.push_back ( vecVertexIndices [ data.uiVertexIdx + i + 1 ] );
                                vecTextureIndices.push_back ( vecTextureIndices [ data.uiTextureIdx + i + 1 ] );
                                vecNormalIndices.push_back ( vecNormalIndices [ data.uiNormalIdx + i + 1 ] );

                                /* Tercer vértice */
                                vecVertexIndices.push_back ( vecVertexIndices [ data.uiVertexIdx + i + 2 ] );
                                vecTextureIndices.push_back ( vecTextureIndices [ data.uiTextureIdx + i + 2] );
                                vecNormalIndices.push_back ( vecNormalIndices [ data.uiNormalIdx + i + 2 ] );

                                /* Añadimos el nuevo triángulo a la lista */
                                pCurGeometry->vecFaces.push_back ( newData );
                            }
                        }
                        else if ( data.uiCount == 3 )
                        {
                            /* Comprobamos que el número de vértices de esta cara sea al menos 3, ya que
                             * el mínimo número de vértices de un polígono es 3. En caso contrario, ignoramos
                             * la cara.
                             */
                            pCurGeometry->vecFaces.push_back ( data );
                        }

                        break;
                    }

                    /**
                     ** Geometrías (grupos)
                     **/
                    case 'g':
                    {
                        /* Saltamos espacios en blanco */
                        ++p;
                        JUMP_SPACES(p);

                        std::string szGeometryName ( p );
                        pCurGeometry = 0;

                        /* Buscamos una geometría con ese nombre */
                        for ( std::vector < __GeometryData >::iterator i = vecGeometries.begin ( );
                              i != vecGeometries.end ( );
                              ++i )
                        {
                            if ( ( *i ).szName == szGeometryName )
                            {
                                pCurGeometry = & ( ( *i ) );
                                break;
                            }
                        }

                        if ( ! pCurGeometry )
                        {
                            /* No se encontró una geometría con ese nombre, creamos una nueva */
                            __GeometryData newGeometry;
                            newGeometry.szName = szGeometryName;
                            newGeometry.pMaterial = 0;
                            vecGeometries.push_back ( newGeometry );
                            pCurGeometry = & ( vecGeometries.back ( ) );
                        }

                        break;
                    }


                    default:
                    {
                        /**
                         ** Materiales                     
                         **/
                        if ( !strncmp ( p, "mtllib ", 7 ) )
                        {
                            /* Nos piden cargar un fichero con información de materiales */
                            p += 7;
                            JUMP_SPACES(p);

                            if ( *p != '\0' )
                            {
                                std::string _szMaterialsPath ( p );
                                wstring szMaterialsPath ( _szMaterialsPath.begin ( ), _szMaterialsPath.end ( ) );
                                szMaterialsPath.assign ( _szMaterialsPath.begin ( ), _szMaterialsPath.end ( ) );
                                LoadMaterials ( szMaterialsPath, mapMaterials );
                            }
                        }
                        
                        else if ( !strncmp ( p, "usemtl ", 7 ) )
                        {
                            /* Nos dicen qué material tendrá la geometría que estamos interpretando en este momento */
                            p += 7;
                            JUMP_SPACES(p);

                            if ( *p != '\0' )
                            {
                                std::string szMaterialName ( p );
                                std::map < std::string, CMaterial >::iterator it = mapMaterials.find ( szMaterialName );

                                if ( it != mapMaterials.end ( ) )
                                {
                                    if ( pCurGeometry == 0 )
                                    {
                                        /* Nunca nos han dado una geometría, así que creamos una por defecto */
                                        __GeometryData newGeometry;
                                        newGeometry.szName = "";
                                        newGeometry.pMaterial = 0;
                                        vecGeometries.push_back ( newGeometry );
                                        pCurGeometry = & ( vecGeometries.back ( ) );
                                    }

                                    pCurGeometry->pMaterial = &( ( *it ).second );
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Eliminamos las geometrías con cero caras */
        for ( std::vector < __GeometryData >::iterator i = vecGeometries.begin ( );
              i != vecGeometries.end ( );
              ++i )
        {
            if ( ( *i ).vecFaces.size ( ) == 0 )
            {
                vecGeometries.erase ( i );
                i = vecGeometries.begin ( );
            }
        }


        /**
         ** Hacemos una segunda pasada para volcar los datos de los vértices
         ** a estructuras temporales de nuevo, pero teniendo una instancia de
         ** CVertex por cada conjunto vértice - textura - normal único. Además,
         ** calculamos la normal superficial para los polígonos en los que no
         ** haya sido especificada.
         **/
        bool bNormalCalculated;
        CVector vecCalculatedNormal;
        std::vector < CVertex > vecRealVertices;
        std::vector < unsigned int > vecRealIndices;

        for ( std::vector < __GeometryData >::iterator i = vecGeometries.begin ( );
              i != vecGeometries.end ( );
              ++i )
        {
            std::vector < __FaceData >& vecFaces = ( *i ).vecFaces;

            for ( std::vector < __FaceData >::iterator j = vecFaces.begin ( );
                  j != vecFaces.end ( );
                  ++j )
            {
                __FaceData& data = ( *j );
                bNormalCalculated = false;
                CVector vecTexture;
                CVector vecNormal;

                data.uiRealVertexIdx = static_cast < unsigned int > ( vecRealIndices.size ( ) );

                for ( unsigned int k = 0; k < data.uiCount; ++k )
                {
                    CVector& vecPosition = vecVertices [ vecVertexIndices [ data.uiVertexIdx + k ] - 1 ];
                    unsigned int uiTextureIdx = vecTextureIndices [ data.uiTextureIdx + k ];
                    unsigned int uiNormalIdx = vecNormalIndices [ data.uiNormalIdx + k ];

                    if ( uiTextureIdx == 0 )
                    {
                        /* No especificaron ninguna textura */
                        vecTexture = CVector ( 0.0f, 0.0f, 0.0f );
                    }
                    else
                    {
                        vecTexture = vecTextures [ uiTextureIdx - 1 ];
                    }

                    if ( uiNormalIdx == 0 )
                    {
                        /* No especificaron ninguna normal */
                        if ( bNormalCalculated )
                        {
                            vecNormal = vecCalculatedNormal;
                        }
                        else
                        {
                            /*
                             * Calculamos la normal a mano
                             * En la primera pasada nos aseguramos de que todas las caras tengan
                             * tres vértices y sólo tres vértices, así que podemos asumirlo así.
                             */
                            bNormalCalculated = true;

                            /* La normal del plano es ( v1 - v2 ) x ( v2 - v3 ) */
                            CVector& vecPosition1 = vecVertices [ vecVertexIndices [ data.uiVertexIdx + 0 ] - 1 ];
                            CVector& vecPosition2 = vecVertices [ vecVertexIndices [ data.uiVertexIdx + 1 ] - 1 ];
                            CVector& vecPosition3 = vecVertices [ vecVertexIndices [ data.uiVertexIdx + 2 ] - 1 ];
                            vecCalculatedNormal = ( vecPosition1 - vecPosition2 ) ^ ( vecPosition2 - vecPosition3 );
                            vecNormal = vecCalculatedNormal;
                        }
                    }
                    else
                    {
                        vecNormal = vecNormals [ uiNormalIdx - 1 ];
                    }

                    /* Construímos el vértice "real" */
                    vecRealIndices.push_back ( static_cast < unsigned int > ( vecRealVertices.size ( ) ) );
                    CVertex newVertex;
                    newVertex.m_vecPosition = vecPosition;
                    newVertex.m_vecTexture = vecTexture;
                    newVertex.m_vecNormal = vecNormal;
                    newVertex.m_vecNormal.Normalize ( );
                    vecRealVertices.push_back ( newVertex );
                }
            }
        }

        /* Limpiamos las estructuras temporales que contenían la información leída del fichero */
        vecVertices.clear ( );
        vecNormals.clear ( );
        vecTextures.clear ( );
        vecVertexIndices.clear ( );
        vecTextureIndices.clear ( );
        vecNormalIndices.clear ( );
        vecVertices.resize ( 0 );
        vecNormals.resize ( 0 );
        vecTextures.resize ( 0 );
        vecVertexIndices.resize ( 0 );
        vecTextureIndices.resize ( 0 );
        vecNormalIndices.resize ( 0 );




        /**
         ** Volcamos los datos leídos a la estructura.
         **/
        /* Reservamos toda la memoria necesaria de golpe */
        void* pData = malloc ( sizeof ( CVertex ) * vecRealVertices.size ( ) +      /* Vértices */
                               sizeof ( unsigned int ) * vecRealIndices.size ( ) );

        /* Calculamos el offset de cada puntero */
        CVertex* pVertices = ( CVertex * ) pData;
        unsigned int* pIndices = ( unsigned int * ) ( &pVertices [ vecRealVertices.size ( ) ] );



        /**
         ** Volcamos los vértices y calculamos la suma de
         ** las coordenadas para hallar el centroide.
         **/
        m_uiNumVertices = static_cast < unsigned int > ( vecRealVertices.size ( ) );
        m_pVertices = pVertices;
        unsigned int uiCurVertex = 0;
        CVector vecCentroid;

        for ( std::vector < CVertex >::iterator i = vecRealVertices.begin ( );
              i != vecRealVertices.end ( );
              ++i )
        {
            m_pVertices [ uiCurVertex ] = ( *i );
            vecCentroid += ( *i ).GetPosition ( );
            ++uiCurVertex;
        }

        /**
         ** Calculamos el centroide con la media y actualizamos los vértices.
         **/
        vecCentroid /= ( float )m_uiNumVertices;

        for ( unsigned int i = 0; i < m_uiNumVertices; ++i )
        {
            CVector& vecPosition = m_pVertices [ i ].m_vecPosition;

            /* Actualizamos el vértice */
            vecPosition -= vecCentroid;
        }

        /* Ahora que está alineado, el centroide del modelo está en ( 0, 0, 0 ). */
        m_vecCentroid = CVector ( 0.0f, 0.0f, 0.0f );


        /**
         ** Volcamos los índices.
         **/
        m_uiNumFaces = static_cast < unsigned int > ( vecRealIndices.size ( ) ) / 3;
        m_puiFaces = pIndices;
        unsigned int uiCurIndex = 0;
        
        for ( std::vector < unsigned int >::iterator i = vecRealIndices.begin ( );
              i != vecRealIndices.end ( );
              ++i )
        {
            pIndices [ uiCurIndex ] = ( *i );
            ++uiCurIndex;
        }


        /**
         ** En el modelo raíz el mínimo índice posible será 0, y el máximo será
         ** el número total de vértices - 1.
         **/
        m_uiMinIndex = 0;
        m_uiMaxIndex = m_uiNumVertices - 1;



        /**
         ** Generamos los submodelos
         **/
        m_uiNumChildren = static_cast < unsigned int > ( vecGeometries.size ( ) );
        if ( m_uiNumChildren == 0 )
        {
            m_pChildModels = 0;
        }
        else
        {
            m_pChildModels = new CModel [ m_uiNumChildren ] ( );
            unsigned int uiCurChild = 0;
            unsigned int uiCurFaceCount = 0;

            for ( std::vector < __GeometryData >::iterator i = vecGeometries.begin ( );
                  i != vecGeometries.end ( );
                  ++i )
            {
                const __GeometryData& curGeometry = ( *i );
                CModel& curChild = m_pChildModels [ uiCurChild ];

                /* Identidad del submodelo */
                wstring szTemp ( curGeometry.szName.begin ( ), curGeometry.szName.end ( ) );
                szTemp.assign ( curGeometry.szName.begin ( ), curGeometry.szName.end ( ) );
                curChild.m_szModelName = szTemp;
                curChild.m_uiModelID = CModelManager::GetFreeModelID ( );

                /* Al menos en los .obj, los submodelos no pueden tener a su vez más submodelos */
                curChild.m_uiNumChildren = 0;
                curChild.m_pChildModels = 0;

                /* Copiamos la información de los vértices */
                curChild.m_uiNumVertices = m_uiNumVertices;
                curChild.m_pVertices = m_pVertices;


                std::vector < __FaceData >& vecFaces = ( *i ).vecFaces;
                /* En la segunda pasada se colocaron los índices a los vértices de cada geometría de forma
                 * secuencial, así que es seguro asignar a los submodelos una dirección de los índices igual
                 * a la dirección base de los índices de este modelo + el tamaño de unsigned int * número de
                 * triángulos de las geometrías anteriores */
                curChild.m_uiNumFaces = static_cast < unsigned int > ( vecFaces.size ( ) );
                curChild.m_puiFaces = m_puiFaces + ( uiCurFaceCount * 3 ); /* Por 3 por ser triángulos */
                uiCurFaceCount += curChild.m_uiNumFaces;
            
                /* Copiamos el material del submodelo, si se le definió uno. */
                if ( curGeometry.pMaterial != 0 )
                {
                    curChild.m_material = *( curGeometry.pMaterial );
                }

                ++uiCurChild;
            }
        }
    }

    CalculateStuff ( );



    /*
     * Cargamos el modelo de colisiones, si existe.
     */
    wstring szColPath = m_szModelName + L".col";
#ifdef WIN32
    std::fstream colFile ( szColPath.c_str( ), std::ios::in );
#else
    /* Linux no utiliza Unicode para acceder al sistema de ficheros, así que lo transformamos a ISO */
    std::string szColPath_iso ( szColPath.begin( ), szColPath.end( ) );
    szColPath_iso.assign ( szColPath.begin( ), szColPath.end( ) );
    std::fstream colFile ( szColPath_iso.c_str( ), std::ios::in );
#endif

    if ( ! colFile.fail ( ) )
    {
        colFile.close ( );

        m_pColModel = new CModel ( szColPath );
    }
}

#undef JUMP_SPACES


void CModel::CalculateStuff ( )
{
    /* Calculamos el centroide */
    m_vecCentroid = CVector ( 0.0f, 0.0f, 0.0f );
    for ( unsigned int i = 0; i < m_uiNumFaces; ++i )
        m_vecCentroid += m_pVertices [ m_puiFaces [ i ] ].GetPosition ( );
    m_vecCentroid /= m_uiNumFaces;

    /* Calculamos la bounding box, el radio y el máximo y mínimo índice */
    m_bboxMin = m_bboxMax = m_pVertices [ m_puiFaces [ 0 ] ].GetPosition ( );
    m_uiMaxIndex = m_uiMinIndex = m_puiFaces [ 0 ];
    m_fRadius = 0;
    for ( unsigned int i = 0; i < m_uiNumFaces; ++i )
    {
        /* Obtenemos el índice actual */
        unsigned int uiCurIndex = m_puiFaces [ i ];

        /* Actualizamos el máximo y el mínimo índice */
        if ( m_uiMinIndex > uiCurIndex )
            m_uiMinIndex = uiCurIndex;
        else if ( m_uiMaxIndex < uiCurIndex )
            m_uiMaxIndex = uiCurIndex;

        /* Movemos el vértice al orígen */
        const CVector& vecPosition = m_pVertices [ m_puiFaces [ i ] ].GetPosition ( );
        CVector vecRealPosition = vecPosition - m_vecCentroid;

        /* Calculamos el radio */
        float fModule = vecRealPosition.Module ( );
        if ( fModule > m_fRadius )
            m_fRadius = fModule;

        /* Actualizamos la "bounding box" */
        if ( vecRealPosition.fX < m_bboxMin.fX )
            m_bboxMin.fX = vecRealPosition.fX;
        else if ( vecRealPosition.fX > m_bboxMax.fX )
            m_bboxMax.fX = vecRealPosition.fX;
        if ( vecRealPosition.fY < m_bboxMin.fY )
            m_bboxMin.fY = vecRealPosition.fY;
        else if ( vecRealPosition.fY > m_bboxMax.fY )
            m_bboxMax.fY = vecRealPosition.fY;
        if ( vecRealPosition.fZ < m_bboxMin.fZ )
            m_bboxMin.fZ = vecRealPosition.fZ;
        else if ( vecRealPosition.fZ > m_bboxMax.fZ )
            m_bboxMax.fZ = vecRealPosition.fZ;

        /* Actualizamos el máximo y el mínimo índice */
    }

    /* Hacemos los cálculos para todos los submodelos */
    for ( unsigned int i = 0; i < m_uiNumChildren; ++i )
    {
        m_pChildModels [ i ].CalculateStuff ( );
    }
}

CModel::CModel ( const wstring& szModelName,
                 const CVertex* pVertices, unsigned int uiNumVertices,
                 const SModelGroup* pGroups, unsigned int uiNumGroups,
                 CModel* pColModel )
{
    Set ( szModelName, pVertices, uiNumVertices, pGroups, uiNumGroups, pColModel );
}

void CModel::Set ( const wstring& szModelName,
                   const CVertex* pVertices, unsigned int uiNumVertices,
                   const SModelGroup* pGroups, unsigned int uiNumGroups,
                   CModel* pColModel )
{
    unsigned int uiTotalFaces = 0;
    CModelManager& manager = CModelManager::GetSingleton ( );

    /* Obtenemos un ID único para este modelo */
    m_uiModelID = manager.GetFreeModelID ( );

    /* Copiamos el nombre */
    m_szModelName = szModelName;

    /* Contamos el número total de caras */
    for ( unsigned int i = 0; i < uiNumGroups; ++i )
    {
        uiTotalFaces += pGroups [ i ].uiNumFaces;
    }

    /* Reservamos memoria para vértices e índices */
    void* pData = malloc ( sizeof ( CVertex ) * uiNumVertices +
                           sizeof ( unsigned int ) * uiTotalFaces * 3 );

    /* Reservamos memoria para los submodelos */
    m_uiNumChildren = uiNumGroups;
    m_pChildModels = new CModel [ m_uiNumChildren ] ( );

    /* Volcamos los vértices */
    m_pVertices = ( CVertex * ) pData;
    m_uiNumVertices = uiNumVertices;
    memcpy ( m_pVertices, pVertices, sizeof ( CVertex ) * uiNumVertices );

    /* Volcamos los índices y los datos de los submodelos */
    m_puiFaces = ( unsigned int * ) &m_pVertices [ uiNumVertices ];
    m_uiNumFaces = uiTotalFaces;

    unsigned int* puiFaces = m_puiFaces;
    for ( unsigned int i = 0; i < uiNumGroups; ++i )
    {
        const SModelGroup& cur = pGroups [ i ];
        CModel& curChild = m_pChildModels [ i ];

        /* Submodelo */
        curChild.m_szModelName = cur.szName;
        curChild.m_uiModelID = manager.GetFreeModelID ( );

        curChild.m_pChildModels = 0;
        curChild.m_uiNumChildren = 0;

        curChild.m_pVertices = m_pVertices;
        curChild.m_uiNumVertices = m_uiNumVertices;

        curChild.m_puiFaces = puiFaces;
        curChild.m_uiNumFaces = cur.uiNumFaces;

        curChild.m_material = cur.material;

        /* Copiamos los índices al modelo raíz */
        memcpy ( puiFaces, cur.pIndices, cur.uiNumFaces * sizeof ( unsigned int ) * 3 );
        puiFaces += cur.uiNumFaces * 3;
    }

    CalculateStuff ( );
}
