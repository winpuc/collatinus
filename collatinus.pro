VERSION = "11.2"

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += MEDIEVAL

TEMPLATE = app
TARGET = collatinusd
INCLUDEPATH += . src
DEPENDPATH += .

DESTDIR = bin
OBJECTS_DIR= obj/
MOC_DIR = moc/
QMAKE_DISTCLEAN += $${DESTDIR}/collatinus

CONFIG += console
CONFIG -= app_bundle
CONFIG += release_binary

QT += core
QT -= gui
QT          += xmlpatterns
QT += network

# Input
HEADERS += src/ch.h \
           src/flexion.h \
           src/irregs.h \
           src/lemme.h \
           src/dicos.h \
		   src/modele.h \
#           src/flexfr.h \
    src/mot.h \
    src/lasla.h \
    src/tagueur.h \
    src/scandeur.h \
    src/lemCore.h \
    src/lemmatiseur.h \
    src/server.h

SOURCES += src/ch.cpp \
           src/flexion.cpp \
#		       src/frequences.cpp \
           src/irregs.cpp \
           src/lemme.cpp \
           src/dicos.cpp \
#           src/flexfr.cpp \
           src/main.cpp \
           src/modele.cpp \
           src/scandeur.cpp \
    src/mot.cpp \
    src/lasla.cpp \
    src/tagueur.cpp \
    src/lemCore.cpp \
    src/lemmatiseur.cpp \
    src/server.cpp

