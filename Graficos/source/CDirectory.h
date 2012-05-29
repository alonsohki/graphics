/*
 * Pr�ctica de Gr�ficos por computador.
 *
 * CDirectory.h
 * Objetivo:  Clase que permite abrir e iterar directorios, multiplataforma (Win32 y POSIX).
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            I�igo Ill�n Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

class CDirectory;

#include "main.h"

#ifdef WIN32
	#include "CDirectory_win32.h"
#else
	#include "CDirectory_posix.h"
#endif
