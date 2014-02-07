######################################################################
# FAUSTLIVE.PRO
######################################################################

# THANKS TO QMAKE, THIS FILE WILL PRODUCE THE MAKEFILE OF FAUSTLIVE APPLICATION
# IT DESCRIBES ALL THE LINKED LIBRAIRIES, COMPILATION OPTIONS, THE SOURCES TO BE COMPILED

# APPLICATION SETTINGS

FAUSTDIR = /usr/local/lib/faust

TEMPLATE = app
TARGET = FaustLive
DEPENDPATH += /usr/local/include/faust/gui
INCLUDEPATH += .
INCLUDEPATH += /usr/local/include	

ICON = Resources/FaustLiveIcon.icns
QMAKE_INFO_PLIST = FaustLiveInfo.plist

OBJECTS_DIR += src
MOC_DIR += src
RCC_DIR += src

all.commands += $(shell mkdir Resources/Libs)

MYFILES = $$system(ls $$FAUSTDIR/*.lib)

for(FILE, MYFILES) {
	all.commands += $(shell cp $$FILE Resources/Libs)
}

all.commands += $(shell cp $$FAUSTDIR/scheduler.ll Resources/Libs)

QMAKE_CXXFLAGS += -Wno-unused-variable -g
QMAKE_EXTRA_TARGETS += all

CONFIG -= x86_64

LLVMLIBS = $$system(/usr/local/Cellar/llvm/3.3/bin/llvm-config --libs)
LLVMDIR = $$system(/usr/local/Cellar/llvm/3.3/bin/llvm-config --ldflags)

QT+=network widgets

LIBS+=-L/usr/local/lib/faust

LIBS+=-lfaust
LIBS+= $$LLVMDIR $$LLVMLIBS

LIBS+=-lHTTPDFaust
LIBS+=-lOSCFaust -loscpack

LIBS+=-L/usr/local/Cellar/llvm/3.3/lib/
LIBS+=-L/usr/local/lib
LIBS+=-lmicrohttpd
LIBS+=-lqrencode
LIBS+=-lboost_system-mt -lboost_filesystem-mt -lboost_program_options-mt
LIBS+= $$LLVMDIR
LIBS+=-lcurl
LIBS+=-lc++

HEADERS += src/utilities.h 
SOURCES += src/utilities.cpp

equals(REMVAR, 1){
	DEFINES += REMOTE
}

equals(CAVAR, 1){
	message("COREAUDIO LINKED")
	LIBS+= -framework CoreAudio -framework AudioUnit -framework CoreServices
	DEFINES += COREAUDIO
	HEADERS += 	src/CA_audioFactory.h\
				src/CA_audioSettings.h\
				src/CA_audioManager.h\
				src/CA_audioFader.h 
				
	SOURCES += 	src/CA_audioFactory.cpp \
				src/CA_audioSettings.cpp \
				src/CA_audioManager.cpp 
}else{
	message("COREAUDIO NOT LINKED")
}

equals(JVAR, 1){
	message("JACK LINKED")
	LIBS+= -ljack
	DEFINES += JACK
	HEADERS += 	src/JA_audioFactory.h \
				src/JA_audioSettings.h \
				src/JA_audioManager.h \
				src/JA_audioFader.h \
	
	SOURCES += 	src/JA_audioSettings.cpp \
				src/JA_audioManager.cpp \
				src/JA_audioFactory.cpp \
				src/JA_audioFader.cpp 
}else{
	message("JACK NOT LINKED")
}	

equals(NJVAR, 1){
	message("NETJACK LINKED")
	LIBS += -ljacknet
	DEFINES += NETJACK
	HEADERS += 	src/NJ_audioFactory.h \
				src/NJ_audioSettings.h \
				src/NJ_audioManager.h \
				src/NJ_audioFader.h \
				src/JsonParser.h \
							
	
	SOURCES += 	src/NJ_audioFactory.cpp \
				src/NJ_audioSettings.cpp \
				src/NJ_audioManager.cpp \
				src/NJ_audioFader.cpp \
				src/JsonParser.cpp 
}else{
	message("NETJACK NOT LINKED")
}		

equals(ALVAR, 1){
	message("ALSA LINKED")
	LIBS += -lasound
	DEFINES += ALSA
	HEADERS += 	src/AL_audioFactory.h \
				src/AL_audioSettings.h \
				src/AL_audioManager.h \
				src/AL_audioFader.h \
	
	SOURCES += 	src/AL_audioFactory.cpp \
				src/AL_audioSettings.cpp \
				src/AL_audioManager.cpp 
}else{
	message("ALSA NOT LINKED")
}		

equals(PAVAR, 1){
	message("PORT AUDIO LINKED")
	LIBS += -lportaudio
	DEFINES += PORTAUDIO
	HEADERS += 	src/PA_audioFactory.h \
				src/PA_audioSettings.h \
				src/PA_audioManager.h \
				src/PA_audioFader.h \
	
	SOURCES += 	src/PA_audioFader.cpp \
				src/PA_audioFactory.cpp \
				src/PA_audioSettings.cpp \
				src/PA_audioManager.cpp 
}else{
	message("PORT AUDIO NOT LINKED")
}		

HEADERS +=	src/AudioSettings.h \
			src/AudioManager.h \
			src/AudioFactory.h \
			src/AudioCreator.h \
			src/AudioFader_Interface.h \
			src/AudioFader_Implementation.h \
			/usr/local/include/faust/gui/faustqt.h \
			src/FJUI.h \
			src/FLToolBar.h \
			src/HTTPWindow.h \
			src/FLrenameDialog.h \
			src/FLErrorWindow.h \
			src/FLExportManager.h \
			src/FLServerHttp.h \
			src/FLEffect.h \
			src/FLWindow.h \ 
			src/FLApp.h \
    		src/SimpleParser.h 

SOURCES += 	src/AudioCreator.cpp \
			src/AudioFader_Implementation.cpp \
			src/FLToolBar.cpp \
			src/HTTPWindow.cpp \
			src/FLrenameDialog.cpp \
			src/FLErrorWindow.cpp \
			src/FLExportManager.cpp \
			src/FLServerHttp.cpp \
			src/FLEffect.cpp \
			src/FLWindow.cpp \ 
			src/FLApp.cpp \
			src/main.cpp \
    		src/SimpleParser.cpp 

 RESOURCES     = Resources/application.qrc




