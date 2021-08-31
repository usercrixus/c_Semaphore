TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Semaphore.c \
        main.c

LIBS += -pthread

HEADERS += \
    Semaphore.h
