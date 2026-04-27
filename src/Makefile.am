bin_PROGRAMS = blc

blc_SOURCES = \
    main.cpp \
    lexer.cpp \
    parser.cpp \
	symbol_table.cpp \
	symbol_table_generator.cpp \
	type_checker.cpp

AM_CXXFLAGS = \
    -std=c++17 \
    -I$(top_srcdir)/include \
	-Wall \
	-Wextra \
	-Wpedantic

if DEBUG
AM_CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer
else
AM_CXXFLAGS += -O2 -DNDEBUG
endif
