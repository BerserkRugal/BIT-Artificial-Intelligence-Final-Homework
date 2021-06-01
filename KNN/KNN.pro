QT       += core gui
CONFIG += console

SOURCES += \
    gobang_moves.cpp \
    knn_fuc.cpp \
    main.cpp

HEADERS += \
    gobang_moves.h \
    knn_fuc.h

INCLUDEPATH += \
D:\TOOLS\OpenCVBuild\install\include \
D:\TOOLS\opencv\opencv\build\include \
D:\TOOLS\opencv\opencv\build\bin



LIBS += \
D:\TOOLS\OpenCVBuild\install\x64\mingw\bin\libopencv_*.dll
