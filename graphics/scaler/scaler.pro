TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt


LIBS += -lpng -lpthread -lgtest
QMAKE_CXXFLAGS +=\
    -std=c++11 \
    -lpng


SOURCES += main.cpp \
    image.cpp \
    grid.cpp \
    gtest.cpp

HEADERS += \
    image.hpp \
    grid.hpp

