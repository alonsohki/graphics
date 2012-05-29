/*
 * Práctica de Gráficos por computador.
 *
 * CPhysical.h
 * Objetivo:  Físicas. No se deben instanciar objetos de esta clase diréctamente, sino
 *            objetos que hereden de ella y hagan uso de estas físicas.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
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
 * - Aceleración lineal: Puntos / segundo^2
 * - Velocidad angular: Radianes / segundo
 * - Aceleración angular: Radianes / segundo^2
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

    /* Las aceleraciones se especifican mediante un vector de dirección y un módulo (fAccel). 
     * Si el vector de dirección no se especifica o es ( 0, 0, 0 ), se aplica a la dirección actual.
     * El vector de dirección debe ser un vector unitario. En caso de no serlo se normaliza.
     */
    void                SetAcceleration         ( float fAccel, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    void                AddAcceleration         ( float fAccel, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    CVector             GetAcceleration         ( ) const;
private:
    void                ApplyAcceleration       ( float fTimeFactor );
public:

    /* Las velocidades se especifican de igual forma que las aceleraciones, sólo que estas se mantienen
     * constantes a no ser que intervenga otra física que pueda modificarlas, como es la aceleración.
     */
    void                SetVelocity             ( float fVelocity, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    void                AddVelocity             ( float fVelocity, const CVector& vecDirection = CVector ( 0.0f, 0.0f, 0.0f ) );
    CVector             GetVelocity             ( ) const;
private:
    void                ApplyVelocity           ( float fTimeFactor );
public:

    /* Aceleración y velocidad angular. Igual que con las lineales, pero afectan a la rotación de la nave
     * sobre su eje en vez de a la posición de la nave.
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
    bool                m_bHasCollided;     // Marca si esta entidad ha colisionado durante el último pulso
    CEntity*            m_pColEntity;       // Almacena la entidad con la que ha colisionado durante el último pulso
    CVector             m_vecCollision;     // Almacena el punto del espacio en el que se ha producido la colisión
    int                 m_iCollisionID;     // Almacena el identificador para esta entidad en el detector de colisiones

    CTimeCounter        m_timeCounter;
};
