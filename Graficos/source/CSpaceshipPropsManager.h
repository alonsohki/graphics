/*
 * Práctica de Gráficos por computador.
 *
 * CSpaceshipPropsManager.h
 * Objetivo:  Gestor de las propiedades de cada nave.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

struct SSpaceshipProps;
class CSpaceshipPropsManager;

#include "CColor.h"

struct SWeaponInfo
{
    enum eWeaponType
    {
        PLASMA,
        LASER,
        DRONS
    };

    CVector         vecPosition;
    eWeaponType     eType;
    CColor          color;
    unsigned int    uiTTL;
    bool            bSpecial;
};

struct SSpaceshipProps
{
    wstring         szName;
    float           fMaxSpeed;
    float           fTurnVelocity;
    float           fAccel;
    float           fHealth;
    float           fShield;
    CColor          colorShield;
    unsigned int    uiNumWeapons;
    unsigned int    uiWeaponsReloadTime;
    SWeaponInfo*    weaponsInfo;
};

class CSpaceshipPropsManager
{
public:
    static CSpaceshipPropsManager&          GetSingleton                ( );
    static CSpaceshipPropsManager*          GetSingletonPtr             ( );

public:
                    CSpaceshipPropsManager      ( );
                    ~CSpaceshipPropsManager     ( );

    void                        Initialize                  ( const wstring& szDataPath );
    const SSpaceshipProps*      GetProperties               ( const wstring& szShipName ) const;

private:
    std::vector < SSpaceshipProps * >   m_vecProps;
};
