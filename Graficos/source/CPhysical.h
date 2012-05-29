/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CPhysical.h
 * Objetivo:  F�sicas. No se deben instanciar objetos de esta clase dir�ctamente, sino
 *            objetos que hereden de ella y hagan uso de estas f�sicas.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CPhysical;

#include "CEntity.h"
#include "CVector.h"
#include "CTimeCounter.h"

/*
 * Unidades:
 * - Velocidad lineal: Puntos / segundo
 * - Aceleraci�n lineal: Puntos / segundo^2
 * - Velocidad angular: Radianes / segundo
 * - Aceleraci�n angular: Radianes / segundo^2
 */

class CPhysical : public CEntity
{
    friend class CWorld;

public:
                        CPhysical               ( );
protected:
    virtual             ~CPhysical              ( );

private:
    void                ApplyPhysics            ( );

public:
    virtual inline void DoPulse                 ( ) { };

    /* Las aceleraciones se especifican mediante un vector de direcci�n y un m�dulo (fAccel). 
     * Si el vector de direcci�n no se especifica o es ( 0, 0, 0 ), se aplica a la direcci�n actual.
     * El vector de direcci�n debe ser un vector unitario. En caso de no serlo se normaliza.
     */
    void                SetAcceleration         ( float fAccel, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    void                AddAcceleration         ( float fAccel, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    CVector             GetAcceleration         ( ) const;
private:
    void                ApplyAcceleration       ( float fTimeFactor );
public:

    /* Las velocidades se especifican de igual forma que las aceleraciones, s�lo que estas se mantienen
     * constantes a no ser que intervenga otra f�sica que pueda modificarlas, como es la aceleraci�n.
     */
    void                SetVelocity             ( float fVelocity, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    void                AddVelocity             ( float fVelocity, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    CVector             GetVelocity             ( ) const;
private:
    void                ApplyVelocity           ( float fTimeFactor );
public:

    /* Aceleraci�n y velocidad angular. Igual que con las lineales, pero afectan a la rotaci�n de la nave
     * sobre su eje en vez de a la posici�n de la nave.
     */
    void                SetTurnAcceleration     ( float fAccel, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    void                AddTurnAcceleration     ( float fAccel, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    CVector             GetTurnAcceleration     ( ) const;
private:
    void                ApplyTurnAcceleration   ( float fTimeFactor );
public:

    void                SetTurnVelocity         ( float fVelocity, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    void                AddTurnVelocity         ( float fVelocity, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    CVector             GetTurnVelocity         ( ) const;
private:
    void                ApplyTurnVelocity       ( float fTimeFactor );
public:

    /* Colisiones */
    inline void         SetCollisions           ( bool bState ) { m_bHasCollisions = bState; }
    inline bool         HasCollisions           ( ) const { return m_bHasCollisions; }
    void                ClearCollisions         ( );

    bool                HasCollided             ( ) { return m_bHasCollided; }
    CEntity*            GetColEntity            ( ) { return m_pColEntity; }
    const CVector&      GetColPoint             ( ) { return m_vecCollision; }
private:
    inline void         SetCollisionID          ( int ID ) { m_iCollisionID = ID; }
    inline int          GetCollisionID          ( ) const { return m_iCollisionID; }

private:
    CVector             m_vecAcceleration;
    CVector             m_vecVelocity;
    CVector             m_vecTurnAcceleration;
    CVector             m_vecTurnVelocity;

    /* Colisiones */
    bool                m_bHasCollisions;   // Marca si se deben procesar las colisiones en esta entidad
    bool                m_bHasCollided;     // Marca si esta entidad ha colisionado durante el �ltimo pulso
    CEntity*            m_pColEntity;       // Almacena la entidad con la que ha colisionado durante el �ltimo pulso
    CVector             m_vecCollision;     // Almacena el punto del espacio en el que se ha producido la colisi�n
    int                 m_iCollisionID;     // Almacena el identificador para esta entidad en el detector de colisiones

    CTimeCounter        m_timeCounter;
};
