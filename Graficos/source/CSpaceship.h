/*
 * Práctica de Gráficos por computador.
 *
 * CSpaceship.h
 * Objetivo:  Naves espaciales. Entidades físicas con atributos específicos ( disparar, munición, vida, etc... ).
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CSpaceship;

#include "CModel.h"
#include "CEntity.h"
#include "CPhysical.h"
#include "CPool.h"
#include "CSpaceshipPropsManager.h"
#include "CTimeCounter.h"

class CSpaceship : public CPhysical
{
public:
                                    CSpaceship              ( const CModel* pModel );
protected:
    virtual                         ~CSpaceship             ( );
public:

    inline void*                    operator new            ( size_t size )
    {
        return CSpaceship::m_poolShips.Alloc ( size );
    }

    inline void                     operator delete         ( void* p )
    {
        CSpaceship::m_poolShips.Free ( static_cast < CSpaceship * > ( p ) );
    }

    virtual void                    DoPulse                 ( );

    bool                            IsOk                    ( ) const;

    inline const SSpaceshipProps*   GetProperties           ( ) const { return m_pProps; }

    void                            Accelerate              ( bool bDirection );
    void                            TurnRoll                ( bool bDirection );
    void                            TurnYaw                 ( bool bDirection );
    void                            TurnPitch               ( bool bDirection );
    void                            Shoot                   ( bool bDirection );
    void                            SpecialShoot            ( bool bDirection );

    inline void                     SetShootTarget          ( const CVector& vecTarget ) { m_vecShooting = vecTarget; }
    inline void                     SetSpecialShootTarget   ( const CVector& vecTarget ) { m_vecSpecialShooting = vecTarget; }

    inline bool                     IsShooting              ( ) const { return m_bShooting; }
    inline bool                     IsSpecialShooting       ( ) const { return m_bSpecialShooting; }

    /* Salud y daños de la nave */
    inline float                    GetHealth               ( ) const { return m_fHealth; }
    inline float                    GetShield               ( ) const { return m_fShield; }
    inline void                     SetHealth               ( float fHealth ) { m_fHealth = fHealth; }
    inline void                     SetShield               ( float fShield ) { m_fShield = fShield; }
    void                            Damage                  ( float fLoss );
    inline bool                     IsWasted                ( ) const { return ( m_fHealth <= 0.0f && m_fShield <= 0.0f ); }
    inline bool                     IsVisible               ( ) const { return !IsWasted ( ); }

private:
    const SSpaceshipProps*          m_pProps;
    static CPool < CSpaceship >     m_poolShips;

    bool                            m_bShooting;
    CVector                         m_vecShooting;
    unsigned int                    m_uiCurWeapon;
    bool                            m_bSpecialShooting;
    CVector                         m_vecSpecialShooting;
    CTimeCounter                    m_lastShoot;

    CTimeCounter                    m_timeCounter;
    int                             m_iMovementFactor;

    float                           m_fHealth;
    float                           m_fShield;
};
