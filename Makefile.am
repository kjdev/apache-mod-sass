
ACLOCAL_AMFLAGS = -I m4

moddir = @APACHE_MODULEDIR@
mod_LTLIBRARIES = mod_sass.la

mod_sass_la_SOURCES = mod_sass.c \
	libsass/ast.cpp libsass/functions.cpp libsass/base64vlq.cpp \
	libsass/inspect.cpp libsass/bind.cpp libsass/output_compressed.cpp \
	libsass/constants.cpp libsass/output_nested.cpp libsass/context.cpp \
	libsass/parser.cpp libsass/contextualize.cpp libsass/prelexer.cpp \
	libsass/copy_c_str.cpp libsass/sass.cpp libsass/emscripten_wrapper.cpp \
	libsass/sass_interface.cpp libsass/error_handling.cpp \
	libsass/source_map.cpp libsass/eval.cpp libsass/to_c.cpp libsass/expand.cpp \
	libsass/to_string.cpp libsass/extend.cpp libsass/trim.cpp \
	libsass/file.cpp libsass/units.cpp
mod_sass_la_CFLAGS = @APACHE_CFLAGS@ @APACHE_INCLUDES@
mod_sass_la_CPPFLAGS = @APACHE_CPPFLAGS@ @APACHE_INCLUDES@
mod_sass_la_LDFLAGS = -avoid-version -module @APACHE_LDFLAGS@
mod_sass_la_LIBS = @APACHE_LIBS@
