/*
 * Práctica de Gráficos por computador.
 *
 * CModelManager.h
 * Objetivo:  Gestor de modelos.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CModelManager;

#include "main.h"
#include "CModel.h"
#include <list>

class CModelManager
{
    friend class CModel;

private:
                            CModelManager       ( );
public:
                            ~CModelManager      ( );

    void                    Initialize          ( const wstring& szModelsPath );
    const CModel*           GetModel            ( const wstring& szModelName );

public:
    static CModelManager&   GetSingleton        ( );
    static CModelManager*   GetSingletonPtr     ( );

private:
    std::list<CModel *>     m_modelsList;

private:
    static unsigned int     GetFreeModelID      ( );
};
