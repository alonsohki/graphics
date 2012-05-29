bin_PROGRAMS=graficos

graficos_SOURCES=main.cpp CApplication.cpp CDirectory_posix.cpp CModel.cpp CModelManager.cpp CWorld.cpp CEntity.cpp \
	CDefaultRenderer.cpp CEntityModel.cpp CController.cpp CCameraTestDriver.cpp CCamera.cpp CCameraFixed.cpp \
	CCameraAttached.cpp CCameraCinematic.cpp CPhysical.cpp CSpaceship.cpp CGame.cpp CGameScript.cpp \
	CSpaceshipPropsManager.cpp CProjectile.cpp CSky.cpp CCrosshair.cpp CPlasma.cpp CPixmap.cpp CSkyModel.cpp \
	CPlanet.cpp CSphereModel.cpp

graficos_LDADD=-lGL -lGLU -lglut -lm -lrt -L../../tinyxml -ltinyxml -L../../glew/lib -lglew \
	-L../../VCollide/lib -lVCollide -L../../VCollide/RAPID -lRAPID

AM_CPPFLAGS=-I. -I.. -I../../tinyxml -I../../glew/include -I../../VCollide/include -I../../VCollide/RAPID -Wall -pipe -O3 -g3
