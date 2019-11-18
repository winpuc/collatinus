VERSION = "12.0"
DEFINES += VERSION=\\\"$$VERSION\\\"

TEMPLATE = app
TARGET = collatinus
INCLUDEPATH += . src
DEPENDPATH += .

qtHaveModule(printsupport): QT += printsupport
QT += widgets
QT += network
QT += svg

LIBS += -lquazip5

#QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -fstack-protector-strong
#QMAKE_CPPFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2

CONFIG += release_binary

TRANSLATIONS    = collatinus_fr.ts
TRANSLATIONS    += collatinus_en.ts

unix:!macx:DESTDIR = bin
OBJECTS_DIR= obj/
MOC_DIR = moc/
QMAKE_DISTCLEAN += $${DESTDIR}/collatinus

# Input
HEADERS += src/ch.h \
           src/dicos.h \
           src/flexion.h \
           src/irregs.h \
           src/lasla.h \
           src/lemcore.h \
           src/lemmatiseur.h \
           src/lemme.h \
           src/mainwindow.h \
		   src/maj.h \
		   src/modele.h \
           src/modules.h \
           src/mot.h \
           src/reglevg.h \
           src/scandeur.h \
           src/tagueur.h \
           src/vargraph.h

SOURCES += src/ch.cpp \
           src/dicos.cpp \
           src/flexion.cpp \
           src/irregs.cpp \
           src/lasla.cpp \
           src/lemcore.cpp \
           src/lemmatiseur.cpp \
           src/lemme.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
		   src/maj.cpp \
           src/modele.cpp \
           src/modules.cpp \
           src/mot.cpp \
           src/reglevg.cpp \
           src/scandeur.cpp \
           src/tagueur.cpp \
           src/vargraph.cpp

RESOURCES += collatinus.qrc
RC_ICONS = res/collatinus.ico

# install
unix:{
    target.path = /usr/bin
    ui.path = /usr/share/collatinus
    ui.files = bin/collatinus_en.qm
    data.files = bin/data/*.*
    data.path = /usr/share/collatinus/data 
    doc.path = /usr/share/doc/collatinus
    doc.files = bin/doc/*
    INSTALLS += target
    INSTALLS += data
    INSTALLS += ui
}

macx:{
    TARGET = Collatinus_$${VERSION}
    #note mac os x, fair un $ qmake -spec macx-g++
    #CONFIG += x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
    ICON = collatinus.icns
    #QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk

    #QMAKE_POST_LINK=strip Collatinus.app/Contents/MacOS/collatinus

    # install into app bundle
    # à changer en ressources
    data.path = $${TARGET}.app/Contents/MacOS/data
    data.files =  bin/data/*
#    deploy.depends = install_documentation
    deploy.depends += install
#    documentation.path = Collatinus_11.app/Contents/MacOS/doc/
#    documentation.files = doc/*.html doc/*.css
    # ajouter un cible qui fait macdeploy Collatinus.app
    deploy.commands = macdeployqt $${TARGET}.app
#    dmg.depends = deploy
#	dmg.commands = ./MacOS/Collatinus.sh
#    INSTALLS += documentation
    INSTALLS += data
    QMAKE_EXTRA_TARGETS += deploy
#    QMAKE_EXTRA_TARGETS += dmg
}
