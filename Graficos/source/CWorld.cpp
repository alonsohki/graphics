/*
 * Práctica de Gráficos por computador.
 *
 * CWorld.cpp
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CWorld::CWorld ( )
    : m_pSky ( 0 )
{
    /* Creamos las cámaras y ponemos como cámara inicial la fija */
    m_pCameras [ CAMERA_FIXED ] = new CCameraFixed ( );
    m_pCameras [ CAMERA_ATTACHED ] = new CCameraAttached ( );
    m_pCameras [ CAMERA_CINEMATIC ] = new CCameraCinematic ( );
    m_pActiveCamera = m_pCameras [ CAMERA_FIXED ];

    /* Creamos la luz por defecto */
    CLight* pLight = CreateLight ( );
    pLight->SetEnabled ( );
    pLight->SetPosition ( CVector ( 0.0f, 1.0f, 1.0f ) );
    pLight->SetAmbient ( CColor ( 0.7f, 0.7f, 0.7f, 1.0f ) );
    pLight->SetDiffuse  ( CColor ( 0.6f, 0.6f, 0.6f, 1.0f ) );
    pLight->SetSpecular ( CColor ( 0.5f, 0.5f, 0.5f, 1.0f ) );
}

CWorld::~CWorld ( )
{
    if ( m_pSky )
        delete m_pSky;
}

void CWorld::DoPulse ( )
{
    if ( m_pSky == 0 )
        m_pSky = new CSky ( );

    /* Aplicamos las físicas a todas las entidades físicas del mundo y actualizamos las matrices en el detector de colisiones */
    double dMatrix [ 4 ] [ 4 ];
    const std::list < CPhysical * >::iterator& end = m_listPhysical.end ( );
    for ( std::list < CPhysical * >::iterator i = m_listPhysical.begin ( );
          i != end;
          ++i )
    {
        CPhysical* pPhysical = ( *i );
        pPhysical->ApplyPhysics ( );

        /* Volcamos la matriz de transformación al detector de colisiones */
        int ID = pPhysical->GetCollisionID ( );
        if ( ID != -1 )
        {
            const float* fMatrix = pPhysical->GetMatrix ( ).GetMatrix ( );
            for ( unsigned int row = 0; row < 4; ++row )
            {
                for ( unsigned int col = 0, n = 0; col < 16; col += 4, ++n )
                {
                    dMatrix [ row ] [ n ] = fMatrix [ row + col ];
                }
            }
            m_vc.UpdateTrans ( ID, dMatrix );
        }
    }

    /* Detectamos colisiones */
    VCReport report;
    m_vc.Collide ( &report );
    int iNumCollisions = report.numObjPairs ( );
    for ( int i = 0; i < iNumCollisions; ++i )
    {
        int ID1 = report.obj1ID ( i );
        int ID2 = report.obj2ID ( i );
//        int iTriangle1 = report.tri1ID ( i, 0 );
//        int iTriangle2 = report.tri2ID ( i, 0 );

        CPhysical* pPhysical1 = m_vecEntityColIDs [ ID1 ];
        CPhysical* pPhysical2 = m_vecEntityColIDs [ ID2 ];

        if ( pPhysical1->HasCollisions ( ) && pPhysical2->HasCollisions ( ) )
        {
            pPhysical1->m_bHasCollided = true;
            pPhysical1->m_pColEntity = pPhysical2;

            pPhysical2->m_bHasCollided = true;
            pPhysical2->m_pColEntity = pPhysical1;
        }
    }

    /* Hacemos un pulso a todas las entidades del mundo */
    const std::list < CEntity * >::iterator& entEnd = m_listEntities.end ( );
    for ( std::list < CEntity * >::iterator i = m_listEntities.begin ( );
          i != entEnd;
          ++i )
    {
        ( *i )->DoPulse ( );
    }

    /* Actualizamos la lista de entidades */
    const std::list < CEntity * >::iterator& addEnd = m_listEntitiesToAdd.end ( );
    for ( std::list < CEntity * >::iterator i = m_listEntitiesToAdd.begin ( );
          i != addEnd;
          ++i )
    {
        m_listEntities.push_front ( ( *i ) );
        CPhysical* pPhysical = dynamic_cast < CPhysical * > ( ( *i ) );
        if ( pPhysical )
        {
            m_listPhysical.push_front ( pPhysical );

            /* Registramos la entidad física en el detector de colisiones */
            const CModel* pModel = pPhysical->GetModel ( );
            const CModel* pColModel = 0;
            if ( pModel && ( pColModel = pModel->GetCollisionModel ( ) ) != 0 )
            {
                const CVertex* pVertices = pColModel->GetVertices ( );
                unsigned int uiNumVertices = pColModel->GetNumVertices ( );
                unsigned int uiNumTriangles = uiNumVertices / 3;

                int ID;

                m_vc.NewObject ( &ID );
                for ( unsigned int i = 0; i < uiNumTriangles; ++i )
                {
                    double v [ 3 ] [ 3 ];

                    for ( unsigned int j = 0; j < 3; ++j )
                    {
                        const CVector& vecPosition = pVertices->GetPosition ( );
                        v [ j ] [ 0 ] = vecPosition.fX;
                        v [ j ] [ 1 ] = vecPosition.fY;
                        v [ j ] [ 2 ] = vecPosition.fZ;
                        ++pVertices;
                    }

                    m_vc.AddTri ( v [ 0 ], v [ 1 ], v [ 2 ], i );
                }
                m_vc.EndObject ( );

                size_t maxID = m_vecEntityColIDs.size ( );
                if ( maxID >= (size_t)ID )
                {
                    m_vecEntityColIDs.resize ( ID + 1 );
                }
                m_vecEntityColIDs [ ID ] = pPhysical;
                pPhysical->SetCollisionID ( ID );
            }
        }
    }
    m_listEntitiesToAdd.clear ( );

    const std::list < CEntity * >::iterator& removeEnd = m_listEntitiesToRemove.end ( );
    for ( std::list < CEntity * >::iterator i = m_listEntitiesToRemove.begin ( );
          i != removeEnd;
          ++i )
    {
        m_listEntities.remove ( ( *i ) );
        CPhysical* pPhysical = dynamic_cast < CPhysical * > ( ( *i ) );
        if ( pPhysical )
            m_listPhysical.remove ( pPhysical );
        delete ( *i );
    }
    m_listEntitiesToRemove.clear ( );


    /* Hacemos un pulso a la cámara */
    m_pActiveCamera->DoPulse ( );
}

void CWorld::Reset ( )
{
    for ( std::list < CEntity * >::iterator i = m_listEntities.begin ( );
          i != m_listEntities.end ( );
          ++i )
    {
        delete ( *i );
    }
    m_listEntities.clear ( );
    m_pActiveCamera = m_pCameras [ CAMERA_FIXED ];
    m_pActiveCamera->SetTarget ( CVector ( 0.0f, 0.0f, -1.0f ) );
    m_pActiveCamera->SetPosition ( CVector ( 0.0f, 0.0f, 0.0f ) );
}

void CWorld::RegisterEntity ( CEntity* pEntity )
{
    /* Registramos la entidad en el mundo */
    m_listEntitiesToAdd.push_front ( pEntity );
}

void CWorld::UnregisterEntity ( CEntity* pEntity )
{
    /* Desregistramos la entidad del mundo */
    m_listEntitiesToRemove.push_front ( pEntity );

    /* Desregistramos la entidad del detector de colisiones si es una entidad física */
    CPhysical* pPhysical = dynamic_cast < CPhysical * > ( pEntity );
    int ID;
    if ( pPhysical && ( ID = pPhysical->GetCollisionID ( ) ) != -1 )
    {
        m_vecEntityColIDs [ ID ] = 0;
        m_vc.DeleteObject ( ID );
    }
}

const std::list < CEntity * >& CWorld::GetEntities ( ) const
{
	return m_listEntities;
}

/* Iluminación */
const std::vector < CLight >& CWorld::GetLights ( ) const
{
    return m_lights;
}

CLight* CWorld::CreateLight ( )
{
    m_lights.push_back ( CLight ( ) );
    return &( m_lights.back ( ) );
}

CLight* CWorld::GetLight ( unsigned int uiIndex )
{
    if ( uiIndex >= m_lights.size ( ) )
        return 0;

    return &( m_lights [ uiIndex ] );
}
