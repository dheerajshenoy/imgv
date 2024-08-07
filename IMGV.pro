######################################################################
# Automatically generated by qmake (3.1) Tue Aug 6 00:05:00 2024
######################################################################

TEMPLATE = app
TARGET = ./bin/imgv
INCLUDEPATH += .
MOC_DIR = moc/
OBJECTS_DIR = obj/

CONFIG += c++20

QT += gui core widgets
LIBS += -llua -lwebp

# Input
HEADERS += src/ImageWidget.hpp \
           src/imgv.hpp \
           src/StatusBar.hpp \
           src/Thumbnail.hpp \
           src/ThumbnailWidget.hpp \
           src/sol/sol.hpp \
           src/AboutDialog.hpp \
           src/MovieItem.hpp \
           src/utils.hpp \
           src/exif.hpp

SOURCES += src/main.cpp \
            src/ImageWidget.cpp \
           src/imgv.cpp \
           src/StatusBar.cpp \
           src/Thumbnail.cpp \
           src/ThumbnailWidget.cpp \
           src/AboutDialog.cpp \
           src/utils.cpp \
           src/exif.cpp
