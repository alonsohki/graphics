/*
 * Práctica de Gráficos por computador.
 *
 * main.cpp
 * Objetivo:  Fichero principal que lanza la aplicación.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

int main ( int argc, const char* argv [], const char* envp [] )
{
    CApplication& app = CApplication::GetSingleton ();

    app.Initialize ( &argc, argv );
    return app.Loop ();
}
