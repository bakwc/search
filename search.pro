TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    n_gram_searcher.cpp \
    stupid_searcher.cpp \
    suffix_tree_searcher.cpp \
    suffix_array_searcher.cpp

HEADERS += \
    searcher.h \
    n_gram_searcher.h \
    stupid_searcher.h \
    suffix_tree_searcher.h \
    suffix_array_searcher.h

QMAKE_CXXFLAGS += -std=c++11
