/*
 * Práctica de Gráficos por computador.
 *
 * CEntity.cpp
 * Objetivo:  Entidades que poseen atributos comunes: posición, rotación, renderizables...
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 */

#include "StdInc.h"

static CVector s_vecNull ( 0.0f, 0.0f, 0.0f );
static CMatrix s_matNull;

CEntity::CEntity ( )
    : m_pEntityModel ( 0 ),
      m_bIsVisible ( true ),
      m_bIsDestroyed ( false )
{
    CApplication::GetSingleton ( ).GetGame ( ).GetWorld ( ).RegisterEntity ( this );
}

CEntity::CEntity ( const CModel* model )
    : m_pEntityModel ( 0 ),
      m_bIsVisible ( true ),
      m_bIsDestroyed ( false )
{
    CApplication::GetSingleton ( ).GetGame ( ).GetWorld ( ).RegisterEntity ( this );

    SetModel ( model );
}

CEntity::~CEntity ( )
{
    if ( m_pEntityModel )
        delete m_pEntityModel;
}

void CEntity::Destroy ( )
{
    if ( ! m_bIsDestroyed )
    {
        CApplication::GetSingleton ( ).GetGame ( ).GetWorld ( ).UnregisterEntity ( this );
        m_bIsDestroyed = true;
        m_bIsVisible = false;
    }
}

void CEntity::SetModel ( const CModel* model )
{
    if ( m_pEntityModel )
        delete m_pEntityModel;

    m_pEntityModel = new CEntityModel ( model );
}

const CModel* CEntity::GetModel ( ) const
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetModel ( );
    else
        return 0;
}

CEntityModel* CEntity::GetEntityModel ( ) const
{
    return m_pEntityModel;
}



/* Transformaciones */
void CEntity::SetPosition ( const CVector& vecPosition )
{
    if ( m_pEntityModel )
        m_pEntityModel->SetPosition ( vecPosition );
}

void CEntity::Move ( const CVector& vec )
{
    if ( m_pEntityModel )
        m_pEntityModel->Move ( vec );
}

const CVector& CEntity::GetPosition ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetPosition ( );
    return s_vecNull;
}


void CEntity::SetRotation ( const CVector& vecRotation )
{
    if ( m_pEntityModel )
        m_pEntityModel->SetRotation ( vecRotation );
}

void CEntity::Rotate ( const CVector& vec )
{
    if ( m_pEntityModel )
        m_pEntityModel->Rotate ( vec );
}

const CVector& CEntity::GetRotation ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetRotation ( );
    return s_vecNull;
}


void CEntity::SetScale ( const CVector& vecScale )
{
    if ( m_pEntityModel )
        m_pEntityModel->SetScale ( vecScale );
}

void CEntity::Scale ( const CVector& vec )
{
    if ( m_pEntityModel )
        m_pEntityModel->Scale ( vec );
}

const CVector& CEntity::GetScale ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetScale ( );
    return s_vecNull;
}



/* Matrices */
const CMatrix& CEntity::GetRotationMatrix ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetRotationMatrix ( );
    return s_matNull;
}

const CMatrix& CEntity::GetTranslationMatrix ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetTranslationMatrix ( );
    return s_matNull;
}

const CMatrix& CEntity::GetScalingMatrix ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetScalingMatrix ( );
    return s_matNull;
}

const CMatrix& CEntity::GetMatrix ( )
{
    if ( m_pEntityModel )
        return m_pEntityModel->GetMatrix ( );
    return s_matNull;
}

void CEntity::SetRotationMatrix ( const CMatrix& matrix )
{
    if ( m_pEntityModel )
        m_pEntityModel->SetRotationMatrix ( matrix );
}

void CEntity::SetTranslationMatrix ( const CMatrix& matrix )
{
    if ( m_pEntityModel )
        m_pEntityModel->SetTranslationMatrix ( matrix );
}

void CEntity::SetScalingMatrix ( const CMatrix& matrix )
{
    if ( m_pEntityModel )
        m_pEntityModel->SetScalingMatrix ( matrix );
}



void CEntity::SetVisible ( bool bIsVisible )
{
    if ( m_bIsDestroyed )
        m_bIsVisible = false;
    else
        m_bIsVisible = bIsVisible;
}

