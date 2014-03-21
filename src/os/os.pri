DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD
VPATH += $$PWD

HEADERS += abstractputtyinstance.h puttyinstance.h

win32 {
  HEADERS += puttyinstance-win.h   versioninfo.h
  SOURCES += puttyinstance-win.cpp versioninfo.cpp
  QMAKE_LIBDIR += "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0A\\lib"
  QMAKE_LIBDIR += "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\lib"
  LIBS += -lversion
}
