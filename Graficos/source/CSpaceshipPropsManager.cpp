/*
 * Práctica de Gráficos por computador.
 *
 * CSpaceshipPropsManager.cpp
 * Objetivo:  Gestor de las propiedades de cada nave.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

/* Singleton */
CSpaceshipPropsManager* CSpaceshipPropsManager::GetSingletonPtr ( )
{
    static CSpaceshipPropsManager* instance = 0;

    if ( instance == 0 )
    {
        instance = new CSpaceshipPropsManager ( );
    }

    return instance;
}

CSpaceshipPropsManager& CSpaceshipPropsManager::GetSingleton ( )
{
    return *GetSingletonPtr ( );
}



/* Clase */
CSpaceshipPropsManager::CSpaceshipPropsManager ( )
{
}

CSpaceshipPropsManager::~CSpaceshipPropsManager ( )
{
    for ( std::vector < SSpaceshipProps * >::iterator i = m_vecProps.begin ( );
          i != m_vecProps.end ( );
          ++i )
    {
        SSpaceshipProps* cur = (*i);
        if ( cur != 0 )
        {
            delete [] cur->weaponsInfo;
            delete cur;
        }
    }
}

void CSpaceshipPropsManager::Initialize ( const wstring& szDataPath_ )
{
    std::string szDataPath ( szDataPath_.begin ( ), szDataPath_.end ( ) );
    szDataPath.assign ( szDataPath_.begin ( ), szDataPath_.end ( ) );
    szDataPath += "/ships.xml";
    TiXmlDocument doc ( szDataPath.c_str ( ) );

    /* Cargamos las propiedades de las naves desde un fichero XML */
    bool bLoaded = doc.LoadFile ( );
    TiXmlElement* pElement = 0;

    if ( bLoaded )
    {
        TiXmlElement* pRoot = doc.RootElement ( );
        if ( pRoot )
        {
            TiXmlNode* pShip = 0;
            while ( ( pShip = pRoot->IterateChildren ( pShip ) ) )
            {
                TiXmlNode* pVelocity = pShip->FirstChild ( "velocity" );
                TiXmlNode* pHealth = pShip->FirstChild ( "health" );
                TiXmlNode* pShield = pShip->FirstChild ( "shield" );
                TiXmlNode* pWeapons = pShip->FirstChild ( "weapons" );
                if ( pVelocity && pHealth && pShield && pWeapons )
                {
                    /* Contamos el número de armas */
                    unsigned int uiNumWeapons = 0;
                    TiXmlNode* pWeapon = 0;
                    while ( ( pWeapon = pWeapons->IterateChildren ( pWeapon ) ) )
                        ++uiNumWeapons;

                    /* Reservamos memoria */
                    SSpaceshipProps* pProps = new SSpaceshipProps;
                    pProps->uiNumWeapons = uiNumWeapons;
                    pProps->weaponsInfo = new SWeaponInfo [ uiNumWeapons ];

                    /* Volcamos el nombre de la nave */
                    pElement = pShip->ToElement ( );
                    const char* szName = pElement->Attribute ( "name" );
                    std::string temp ( szName );
                    pProps->szName.assign ( temp.begin ( ), temp.end ( ) );

                    /* Volcamos las propiedades de las armas */
                    unsigned int uiCurWeapon = 0;
                    pWeapon = 0;
                    while ( ( pWeapon = pWeapons->IterateChildren ( pWeapon ) ) )
                    {
                        pElement = pWeapon->ToElement ( );
                        SWeaponInfo& cur = pProps->weaponsInfo [ uiCurWeapon ];
                        ++uiCurWeapon;

                        pElement->QueryFloatAttribute ( "x", &(cur.vecPosition.fX) );
                        pElement->QueryFloatAttribute ( "y", &(cur.vecPosition.fY) );
                        pElement->QueryFloatAttribute ( "z", &(cur.vecPosition.fZ) );

                        const char* szType = pElement->Attribute ( "type" );
                        if ( !strcmp ( szType, "laser" ) )
                            cur.eType = SWeaponInfo::LASER;
                        else if ( !strcmp ( szType, "drons" ) )
                            cur.eType = SWeaponInfo::DRONS;
                        else
                            cur.eType = SWeaponInfo::PLASMA;

                        const char* szColor = pElement->Attribute ( "color" );
                        cur.color.Set ( szColor );

                        pElement->QueryIntAttribute ( "ttl", (int *)&(cur.uiTTL) );

                        const char* szSpecial = pElement->Attribute ( "special" );
                        if ( !strcmp ( szSpecial, "no" ) )
                            cur.bSpecial = false;
                        else
                            cur.bSpecial = true;
                    }

                    /* Volcamos el resto de los datos */
                    pElement = pWeapons->ToElement ( );
                    pElement->QueryIntAttribute ( "reloadtime", (int *)&(pProps->uiWeaponsReloadTime) );

                    pElement = pVelocity->ToElement ( );
                    pElement->QueryFloatAttribute ( "max", &(pProps->fMaxSpeed) );
                    pElement->QueryFloatAttribute ( "accel", &(pProps->fAccel) );
                    pElement->QueryFloatAttribute ( "turn", &(pProps->fTurnVelocity) );

                    pElement = pHealth->ToElement ( );
                    pElement->QueryFloatAttribute ( "value", &(pProps->fHealth) );

                    pElement = pShield->ToElement ( );
                    pElement->QueryFloatAttribute ( "value", &(pProps->fShield ) );
                    const char* szColor = pElement->Attribute ( "color" );
                    pProps->colorShield.Set ( szColor );


                    /* Añadimos las propiedades de esta nave a la lista */
                    m_vecProps.push_back ( pProps );
                }
            }
        }
    }
}


const SSpaceshipProps* CSpaceshipPropsManager::GetProperties ( const wstring& szShipName ) const
{
    const std::vector < SSpaceshipProps * >::const_iterator end = m_vecProps.end ( );

    for ( std::vector < SSpaceshipProps * >::const_iterator i = m_vecProps.begin ( );
          i != end;
          ++i )
    {
        if ( szShipName == (*i)->szName )
            return (*i);
    }

    return 0;
}
