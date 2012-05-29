/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CModel.h
 * Objetivo:  Clase que procesa los ficheros de los modelos disponibles y crea
 *            una interfaz para ellos, adem�s de dibujarlos a demanda.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CModel;

#include <vector>
#include <map>
#include "main.h"
#include "CVertex.h"
#include "CVector.h"
#include "CColor.h"
#include "CMaterial.h"

struct SModelGroup
{
    wstring         szName;
    unsigned int*   pIndices;
    unsigned int    uiNumFaces;
    CMaterial       material;
};

class CModel
{
public:
    /* Constructores y destructor */
                                CModel              ( const wstring& szModelPath );
protected:
                                CModel              ( const wstring& szModelName,
                                                      const CVertex* pVertices, unsigned int uiNumVertices,
                                                      const SModelGroup* pGroups, unsigned int uiNumGroups,
                                                      CModel* pColModel
                                                    );

    void                        Set                 ( const wstring& szModelName,
                                                      const CVertex* pVertices, unsigned int uiNumVertices,
                                                      const SModelGroup* pGroups, unsigned int uiNumGroups,
                                                      CModel* pColModel
                                                    );

                                CModel              ( );
public:
    virtual                     ~CModel             ( );

    /* Datos de identificaci�n del modelo */
    unsigned int                GetID               ( ) const { return m_uiModelID; }
    inline const wstring&       GetName             ( ) const { return m_szModelName; }

    /* Bounding box: la caja que contiene al modelo */
    inline void                 GetBoundingBox      ( CVector& min, CVector& max ) const { min = m_bboxMin; max = m_bboxMax; }

    /* Radio: distancia del centro al v�rtice m�s alejado ( esfera que contiene al modelo ) */
    inline float                GetRadius           ( ) const { return m_fRadius; }

    /* Centroide */
    inline const CVector&       GetCentroid         ( ) const { return m_vecCentroid; }

    /* Submodelos */
    inline unsigned int         GetNumChildren      ( ) const { return m_uiNumChildren; }
    inline const CModel*        GetChildren         ( ) const { return m_pChildModels; }

    /* V�rtices */
    inline unsigned int         GetNumVertices      ( ) const { return m_uiNumVertices; }
    inline const CVertex*       GetVertices         ( ) const { return m_pVertices; }

    /*
     * Caras
     * El int�rprete triangula las caras de m�s de cuatro v�rtices, as� que es seguro acceder
     * dir�ctamente al array de �ndices e ir sumando de tres en tres para obtener los �ndices
     * de cada cara.
     */
    inline unsigned int         GetNumFaces         ( ) const { return m_uiNumFaces; }
    inline const unsigned int*  GetIndices          ( ) const { return m_puiFaces; }

    /* Estas funciones retornan el m�nimo y el m�ximo �ndice que se espera encontrar en este modelo */
    inline unsigned int         GetMinIndex         ( ) const { return m_uiMinIndex; }
    inline unsigned int         GetMaxIndex         ( ) const { return m_uiMaxIndex; }

    /* Material del modelo */
    inline const CMaterial&     GetMaterial         ( ) const { return m_material; }

    /* Modelo de colisiones */
    const CModel*               GetCollisionModel   ( ) const { return ( m_pColModel ? m_pColModel : this ); }

    /* Modelos din�micos ( generados por c�digo ) */
    virtual bool                IsDynamic           ( ) const { return false; }

private:
    /* Funciones privadas para el uso del int�rprete de modelos */
    void                        LoadModel           ( const wstring& szModelPath );
    void                        LoadMaterials       ( const wstring& szMaterialsPath, std::map < std::string, CMaterial >& mapMaterials );
    /* CalculateStuff: Calcula el radio, la bounding box, el centroide, etc�tera... del modelo */
    void                        CalculateStuff      ( );

private:
    /* Informaci�n del modelo */
    unsigned int                m_uiModelID;
    wstring                     m_szModelName;

    /* Submodelos ( grupos de caras ) */
    unsigned int                m_uiNumChildren;
    CModel*                     m_pChildModels;

    /*
     * V�rtices
     * Estos campos s�lo son v�lidos en el modelo ra�z y no en los submodelos.
     * Los submodelos s�lo definen �ndices que referencian a los v�rtices del modelo ra�z.
     */
    unsigned int                m_uiNumVertices;
    CVertex*                    m_pVertices;

    /*
     * Caras
     * El modelo ra�z define todas las caras del modelo, incluyendo las de los submodelos.
     * As� pues, se puede dibujar tanto el modelo completo usando estos campos como dibujarlo
     * por partes ( pudiendo transformarlas por separado respecto al ra�z ) accediendo a los
     * �ndices de los modelos hijos.
     */
    unsigned int                m_uiNumFaces;
    unsigned int*               m_puiFaces;

    /* Material */
    CMaterial                   m_material;

    /* Bounding box */
    CVector                     m_bboxMin;
    CVector                     m_bboxMax;

    /*
     * Radio
     * Distancia del centro al v�rtice m�s alejado.
     * Extremadamente �til a la hora de calcular colisiones, ya que la colisi�n con
     * la esfera que rodea el modelo es trivial y ayuda a descartar r�pidamente entidades
     * que no colisionen con lo que se est� comprobando. Tambi�n se puede utilizar a la hora
     * de establecer los "offset" en la c�mara "attached" para colocarla a una distancia
     * que permita visualizar el modelo completo.
     */
    float                       m_fRadius;


    /* Centroide
     * En el modelo ra�z ser� ( 0, 0, 0 ), en los submodelos ser� la distancia desde ( 0, 0, 0 )
     * al centroide del submodelo.
     */
    CVector                     m_vecCentroid;

    /* M�nimo y m�ximo �ndice que podremos encontrarnos en la lista de �ndices de las caras */
    unsigned int                m_uiMinIndex;
    unsigned int                m_uiMaxIndex;

    /* Modelo de colisiones: modelo muy simplificado para realizar tests de colisi�n */
    CModel*                     m_pColModel;


private:
    /* Estructuras usadas por el int�rprete */
    struct __FaceData
    {
        unsigned int    uiVertexIdx;
        unsigned int    uiTextureIdx;
        unsigned int    uiNormalIdx;
        unsigned int    uiRealVertexIdx;
        unsigned int    uiCount;
    };

    struct __GeometryData
    {
        std::string                 szName;
        CMaterial*                  pMaterial;
        std::vector < __FaceData >  vecFaces;
    };
};
