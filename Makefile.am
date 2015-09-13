
ACLOCAL_AMFLAGS = -I m4

moddir = @APACHE_MODULEDIR@
mod_LTLIBRARIES = mod_sass.la

mod_sass_la_SOURCES = mod_sass.c \
	libsass/src/ast.cpp \
	libsass/src/base64vlq.cpp \
	libsass/src/bind.cpp \
	libsass/src/color_maps.cpp \
	libsass/src/constants.cpp \
	libsass/src/context.cpp \
	libsass/src/cssize.cpp \
	libsass/src/emitter.cpp \
	libsass/src/environment.cpp \
	libsass/src/error_handling.cpp \
	libsass/src/eval.cpp \
	libsass/src/expand.cpp \
	libsass/src/extend.cpp \
	libsass/src/file.cpp \
	libsass/src/functions.cpp \
	libsass/src/inspect.cpp \
	libsass/src/json.cpp \
	libsass/src/lexer.cpp \
	libsass/src/listize.cpp \
	libsass/src/memory_manager.cpp \
	libsass/src/node.cpp \
	libsass/src/output.cpp \
	libsass/src/parser.cpp \
	libsass/src/plugins.cpp \
	libsass/src/position.cpp \
	libsass/src/prelexer.cpp \
	libsass/src/remove_placeholders.cpp \
	libsass/src/sass.cpp \
	libsass/src/sass2scss.cpp \
	libsass/src/sass_context.cpp \
	libsass/src/sass_functions.cpp \
	libsass/src/sass_interface.cpp \
	libsass/src/sass_util.cpp \
	libsass/src/sass_values.cpp \
	libsass/src/source_map.cpp \
	libsass/src/to_c.cpp \
	libsass/src/to_string.cpp \
	libsass/src/to_value.cpp \
	libsass/src/units.cpp \
	libsass/src/utf8_string.cpp \
	libsass/src/util.cpp \
	libsass/src/values.cpp \
	libsass/src/c99func.c \
	libsass/src/cencode.c
mod_sass_la_CFLAGS = @APACHE_CFLAGS@ @APACHE_INCLUDES@
mod_sass_la_CPPFLAGS = @APACHE_CPPFLAGS@ @APACHE_INCLUDES@
mod_sass_la_LDFLAGS = -avoid-version -module @APACHE_LDFLAGS@
mod_sass_la_LIBS = @APACHE_LIBS@
