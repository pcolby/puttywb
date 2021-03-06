# Make an application called puttywb.
TARGET = puttywb
TEMPLATE = app

# Disable automatic ASCII conversions (best practice, especially for i18n support).
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

# Define the DEBUG macro for qmake's debug target.
CONFIG(debug,debug|release):DEFINES+=DEBUG

# Set the dependancies.
RESOURCES = qrc/puttywb.qrc
win32:RC_FILE=qrc/puttywb.rc
include(src/src.pri)

# Neaten the output directories.
CONFIG(debug,debug|release) {
  MOC_DIR = debug/tmp
  OBJECTS_DIR = debug/tmp
  RCC_DIR = debug/tmp
}
CONFIG(release,debug|release) {
  MOC_DIR = release/tmp
  OBJECTS_DIR = release/tmp
  RCC_DIR = release/tmp
}

# Create our custom revbuild target.
win32:revbuild.commands = utils\\gitversion.cmd qrc\\puttywb.rc_ qrc\\puttywb.rc
QMAKE_EXTRA_TARGETS += revbuild

# Hook our revbuild target in between qmake's Makefile update and the actual project target.
revbuildhook.depends = revbuild
CONFIG(debug,debug|release):revbuildhook.target = Makefile.Debug
CONFIG(release,debug|release):revbuildhook.target = Makefile.Release
QMAKE_EXTRA_TARGETS += revbuildhook

# Enable application internationalisatio / localisation.
#TRANSLATIONS = i18n/en.ts i18n/en_AU.ts i18n/en_GB.ts i18n/en_US.ts
#QMAKE_POST_LINK += lupdate -no-obsolete -silent client.pro

# Enable GNU profiling (if desired).
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg
