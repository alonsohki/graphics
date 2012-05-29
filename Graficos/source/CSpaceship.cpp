/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CSpaceship.cpp
 * Objetivo:  Naves espaciales. Entidades f�sicas con atributos espec�ficos ( disparar, munici�n, vida, etc... ).
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

CPool < CSpaceship > CSpaceship::m_poolShips ( 4096 );

CSpaceship::CSpaceship ( const CModel* pModel )
    : m_pProps ( 0 ),
      m_uiCurWeapon ( 0 )
{
    CEntity::SetModel ( pModel );
    if ( pModel )
    {
        m_pProps = CSpaceshipPropsManager::GetSingleton ( ).GetProperties ( pModel->GetName ( ) );
        m_fHealth = m_pProps->fHealth;
        m_fShield = m_pProps->fShield;
    }
}

CSpaceship::~CSpaceship ( )
{
}

void CSpaceship::DoPulse ( )
{
    if ( ! IsWasted ( ) )
    {
        const CVector& vecVelocity = CPhysical::GetVelocity ( );
        float fVelocityModule = vecVelocity.Module ( );

        /* Obtenemos la direcci�n de la nave */
        CVector vecDirection = CVector ( 0.0f, 0.0f, 1.0f ) * CEntity::GetEntityModel ( )->GetRotationMatrix ( );
        vecDirection.Normalize ( );

        fVelocityModule += m_pProps->fAccel * m_timeCounter.Query ( ) / 1000000.0f * m_iMovementFactor;

        if ( fVelocityModule <= 0.0f )
        {
            CPhysical::SetVelocity ( 0.0f );
        }
        else
        {
            /* Ajustamos el m�dulo de la velocidad a la velocidad m�xima */
            if ( fVelocityModule > m_pProps->fMaxSpeed )
                fVelocityModule = m_pProps->fMaxSpeed;

            /* Aplicamos la direcci�n obtenida con la rotaci�n de la nave */
            CPhysical::SetVelocity ( fVelocityModule, vecDirection );
        }

        CPhysical::DoPulse ( );

        /* Disparos */
        if ( m_bShooting )
        {
            unsigned long ulMilisecondsSpent = m_lastShoot.Query ( false ) / 1000;

            if ( ulMilisecondsSpent > m_pProps->uiWeaponsReloadTime )
            {
                unsigned int uiLoops = 0;

                while ( m_pProps->weaponsInfo [ m_uiCurWeapon ].bSpecial == true )
                {
                    ++m_uiCurWeapon;
                    if ( m_uiCurWeapon >= m_pProps->uiNumWeapons )
                    {
                        m_uiCurWeapon = 0;
                        ++uiLoops;

                        /* Si no hay armas disponibles, evitamos iteraciones infinitas */
                        if ( uiLoops == 2 )
                            break;
                    }
                }
                m_lastShoot.Update ( );

                if ( uiLoops < 2 )
                {
                    /* Obtenemos la informaci�n del arma */
                    SWeaponInfo& weapon = m_pProps->weaponsInfo [ m_uiCurWeapon ];

                    /* Obtenemos la posici�n transformada del arma */
                    CVector vecGunPosition = weapon.vecPosition * CEntity::GetMatrix ( );

                    CVector vecShoot = m_vecShooting - vecGunPosition;
                    vecShoot.Normalize ( );

                    CProjectile* pProjectile = 0;
                    switch ( weapon.eType )
                    {
                        case SWeaponInfo::PLASMA:
                        {
                            pProjectile = new CPlasma ( vecGunPosition,
                                                        vecShoot * 8000.0f,
                                                        weapon.color,
                                                        weapon.uiTTL,
                                                        this );
                            break;
                        }
                        case SWeaponInfo::LASER:
                        case SWeaponInfo::DRONS:
                        { break; }
                    }

                    if ( pProjectile )
                    {
                        CMatrix matOrientation;
                        CMatrixFactory::GetOrientationMatrix ( matOrientation, vecShoot );
                        pProjectile->SetRotationMatrix ( matOrientation );
                    }

                    ++m_uiCurWeapon;
                    if ( m_uiCurWeapon >= m_pProps->uiNumWeapons )
                        m_uiCurWeapon = 0;
                }
            }
        }

        if ( CPhysical::HasCollided ( ) )
        {
            /* Nos aseguramos de que la colisi�n no haya sido con un proyectil nuestro */
            CProjectile* pProjectile = dynamic_cast < CProjectile * > ( CPhysical::GetColEntity ( ) );
            if ( !pProjectile || pProjectile->GetCreator ( ) != this )
            {
                Damage ( 2000.0f );
            }
        }
    }
    else
    {
        m_iMovementFactor = 0;
        CPhysical::SetVelocity ( 0.0f );
        CPhysical::SetAcceleration ( 0.0f );
        CPhysical::SetTurnVelocity ( 0.0f );
        CPhysical::SetTurnAcceleration ( 0.0f );
        CPhysical::DoPulse ( );
        SetCollisions ( false );
    }
}

bool CSpaceship::IsOk ( ) const
{
    return ( m_pProps != 0 );
}


/* Control */
void CSpaceship::Accelerate ( bool bDirection )
{
    if ( ! IsWasted ( ) )
    {
        if ( bDirection )
            m_iMovementFactor++;
        else
            m_iMovementFactor--;
    }
}

void CSpaceship::TurnRoll ( bool bDirection )
{
    if ( ! IsWasted ( ) )
    {
        if ( bDirection )
            CPhysical::AddTurnVelocity ( m_pProps->fTurnVelocity, CVector ( 0.0f, 0.0f, 1.0f ) );
        else
            CPhysical::AddTurnVelocity ( m_pProps->fTurnVelocity, CVector ( 0.0f, 0.0f, -1.0f ) );
    }
}

void CSpaceship::TurnYaw ( bool bDirection )
{
    if ( ! IsWasted ( ) )
    {
        if ( bDirection )
            CPhysical::AddTurnVelocity ( m_pProps->fTurnVelocity, CVector ( 0.0f, 1.0f, 0.0f ) );
        else
            CPhysical::AddTurnVelocity ( m_pProps->fTurnVelocity, CVector ( 0.0f, -1.0f, 0.0f ) );
    }
}

void CSpaceship::TurnPitch ( bool bDirection )
{
    if ( ! IsWasted ( ) )
    {
        if ( bDirection )
            CPhysical::AddTurnVelocity ( m_pProps->fTurnVelocity, CVector ( 1.0f, 0.0f, 0.0f ) );
        else
            CPhysical::AddTurnVelocity ( m_pProps->fTurnVelocity, CVector ( -1.0f, 0.0f, 0.0f ) );
    }
}

void CSpaceship::Shoot ( bool bDirection )
{
    m_bShooting = bDirection;
}

void CSpaceship::SpecialShoot ( bool bDirection )
{
    m_bSpecialShooting = bDirection;
}

void CSpaceship::Damage ( float fLoss )
{
    if ( fLoss > m_fShield )
    {
        fLoss -= m_fShield;
        m_fShield = 0.0f;

        if ( fLoss > m_fHealth )
        {
            m_fHealth = 0.0f;

            /* Aqu� deber�amos hacer explotar la nave, pero de momento simplemente
             * le quitamos las colisiones y evitamos su renderizado.
             */
            CPhysical::SetCollisions ( false );
        }
        else
        {
            m_fHealth -= fLoss;
        }
    }
    else
        m_fShield -= fLoss;
}
