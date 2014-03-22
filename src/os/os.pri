DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD
VPATH += $$PWD

HEADERS += abstractputtyinstance.h puttyinstance.h

win32 {
  HEADERS += puttyinstance-win.h   versioninfo.h
  SOURCES += puttyinstance-win.cpp versioninfo.cpp
  LIBS += -lversion
}
