/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * main.cpp
 * Objetivo:  Fichero principal que lanza la aplicaci�n.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#include "StdInc.h"

int main ( int argc, const char* argv [], const char* envp [] )
{
    CApplication& app = CApplication::GetSingleton ();

    app.Initialize ( &argc, argv );
    return app.Loop ();
}
