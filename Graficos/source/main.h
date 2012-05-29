/*
 * Práctica de Gráficos por computador.
 *
 * main.h
 * Objetivo:  Cabecera principal del programa.
 *
 * Autores:   Alberto Alonso Pinto      <rydencillo@gmail.com>
 *            Iñigo Illán Aranburu      <iillan002@ikasle.ehu.es>
 *
 */

#pragma once

#define WINDOW_TITLE "Gráficos por computador"
#define SHOWFPS
#define DRAW_COORDINATE_SYSTEM

#ifdef WIN32
    #include <windows.h>
    #include <time.h>
    #include <tchar.h>
    #define Sleep_Miliseconds   Sleep
#else
	#include <unistd.h>
    #include <sys/time.h>
    #define Sleep_Miliseconds(x) usleep ( x * 1000 )
    #define BYTE char
#endif

#define PI 3.141592653589f
#define PI2 6.283185307178f
#define deg2rad(x) ((x) * PI / 180 )
#define rad2deg(x) ((x) * 180 / PI )

#include <iostream>
#include <string>
using std::cout;
using std::wcout;
using std::endl;
using std::wstring;

/* Soporte para gettimeofday() en windows,
 * obtenido de http://www.openasthra.com/wp-content/uploads/gettimeofday.c
 */
#ifdef WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

inline int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    tmpres /= 10;  /*convert into microseconds*/
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  return 0;
}
#endif
