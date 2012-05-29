#pragma once

#ifdef WIN32
#pragma message("Compiling precompiled header.\n")
#endif

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <tinyxml.h>
#include <VCollide.H>

#include "main.h"
#include "hex.h"

#include "IRenderer.h"
#include "IPulsable.h"

#include "CCamera.h"
#include "CCameraFixed.h"
#include "CCameraAttached.h"
#include "CCameraCinematic.h"
#include "CCallback.h"
#include "CVector.h"
#include "CVertex.h"
#include "CMatrix.h"
#include "CMatrixFactory.h"
#include "CModel.h"
#include "CModelManager.h"
#include "CGame.h"
#include "CGameScript.h"
#include "CPool.h"
#include "CDefaultRenderer.h"
#include "CEntity.h"
#include "CEntityModel.h"
#include "CPhysical.h"
#include "CSpaceshipPropsManager.h"
#include "CSpaceship.h"
#include "CApplication.h"
#include "CDirectory.h"
#include "CCameraTestDriver.h"
#include "CProjectile.h"
#include "CPlasma.h"
#include "CCrosshair.h"
#include "CSkyModel.h"
#include "CSky.h"
#include "CPixmap.h"
#include "CSphereModel.h"
#include "CPlanet.h"

