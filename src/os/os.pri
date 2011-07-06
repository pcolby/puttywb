DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD
win32 {
  HEADERS += versioninfo.h
  SOURCES += versioninfo.cpp
  QMAKE_LIBDIR += "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0A\\lib"
  QMAKE_LIBDIR += "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\lib"
  LIBS += -lversion
}
