TEMPLATE  	= app
LANGUAGE  	= C++
CONFIG		+= qt opengl
QT		+= opengl
INCLUDEPATH =	include 
LIBS		+= -lGLU -ldl

HEADERS 	=	include/DeckLinkAPIDispatch.cpp \
                        cam2vr.h \
                        OpenGLCapture.h \
                        GLExtensions.h \
                        DeviceInfo.h

SOURCES 	= 	main.cpp \
                        include/DeckLinkAPIDispatch.cpp \
                        cam2vr.cpp \
                        OpenGLCapture.cpp \
                        GLExtensions.cpp \
                        DeviceInfo.cpp

FORMS 		= 
