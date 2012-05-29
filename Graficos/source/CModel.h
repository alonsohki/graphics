/*
 * Práctica de Gráficos por computador.
 *
 * CModel.h
 * Objetivo:  Clase que procesa los ficheros de los modelos disponibles y crea
 *            una interfaz para ellos, además de dibujarlos a demanda.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
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

    /* Datos de identificación del modelo */
    unsigned int                GetID               ( ) const { return m_uiModelID; }
    inline const wstring&       GetName             ( ) const { return m_szModelName; }

    /* Bounding box: la caja que contiene al modelo */
    inline void                 GetBoundingBox      ( CVector& min, CVector& max ) const { min = m_bboxMin; max = m_bboxMax; }

    /* Radio: distancia del centro al vértice más alejado ( esfera que contiene al modelo ) */
    inline float                GetRadius           ( ) const { return m_fRadius; }

    /* Centroide */
    inline const CVector&       GetCentroid         ( ) const { return m_vecCentroid; }

    /* Submodelos */
    inline unsigned int         GetNumChildren      ( ) const { return m_uiNumChildren; }
    inline const CModel*        GetChildren         ( ) const { return m_pChildModels; }

    /* Vértices */
    inline unsigned int         GetNumVertices      ( ) const { return m_uiNumVertices; }
    inline const CVertex*       GetVertices         ( ) const { return m_pVertices; }

    /*
     * Caras
     * El intérprete triangula las caras de más de cuatro vértices, así que es seguro acceder
     * diréctamente al array de índices e ir sumando de tres en tres para obtener los índices
     * de cada cara.
     */
    inline unsigned int         GetNumFaces         ( ) const { return m_uiNumFaces; }
    inline const unsigned int*  GetIndices          ( ) const { return m_puiFaces; }

    /* Estas funciones retornan el mínimo y el máximo índice que se espera encontrar en este modelo */
    inline unsigned int         GetMinIndex         ( ) const { return m_uiMinIndex; }
    inline unsigned int         GetMaxIndex         ( ) const { return m_uiMaxIndex; }

    /* Material del modelo */
    inline const CMaterial&     GetMaterial         ( ) const { return m_material; }

    /* Modelo de colisiones */
    const CModel*               GetCollisionModel   ( ) const { return ( m_pColModel ? m_pColModel : this ); }

    /* Modelos dinámicos ( generados por código ) */
    virtual bool                IsDynamic           ( ) const { return false; }

private:
    /* Funciones privadas para el uso del intérprete de modelos */
    void                        LoadModel           ( const wstring& szModelPath );
    void                        LoadMaterials       ( const wstring& szMaterialsPath, std::map < std::string, CMaterial >& mapMaterials );
    /* CalculateStuff: Calcula el radio, la bounding box, el centroide, etcétera... del modelo */
    void                        CalculateStuff      ( );

private:
    /* Información del modelo */
    unsigned int                m_uiModelID;
    wstring                     m_szModelName;

    /* Submodelos ( grupos de caras ) */
    unsigned int                m_uiNumChildren;
    CModel*                     m_pChildModels;

    /*
     * Vértices
     * Estos campos sólo son válidos en el modelo raíz y no en los submodelos.
     * Los submodelos sólo definen índices que referencian a los vértices del modelo raíz.
     */
    unsigned int                m_uiNumVertices;
    CVertex*                    m_pVertices;

    /*
     * Caras
     * El modelo raíz define todas las caras del modelo, incluyendo las de los submodelos.
     * Así pues, se puede dibujar tanto el modelo completo usando estos campos como dibujarlo
     * por partes ( pudiendo transformarlas por separado respecto al raíz ) accediendo a los
     * índices de los modelos hijos.
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
     * Distancia del centro al vértice más alejado.
     * Extremadamente útil a la hora de calcular colisiones, ya que la colisión con
     * la esfera que rodea el modelo es trivial y ayuda a descartar rápidamente entidades
     * que no colisionen con lo que se esté comprobando. También se puede utilizar a la hora
     * de establecer los "offset" en la cámara "attached" para colocarla a una distancia
     * que permita visualizar el modelo completo.
     */
    float                       m_fRadius;


    /* Centroide
     * En el modelo raíz será ( 0, 0, 0 ), en los submodelos será la distancia desde ( 0, 0, 0 )
     * al centroide del submodelo.
     */
    CVector                     m_vecCentroid;

    /* Mínimo y máximo índice que podremos encontrarnos en la lista de índices de las caras */
    unsigned int                m_uiMinIndex;
    unsigned int                m_uiMaxIndex;

    /* Modelo de colisiones: modelo muy simplificado para realizar tests de colisión */
    CModel*                     m_pColModel;


private:
    /* Estructuras usadas por el intérprete */
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
