
ACLOCAL_AMFLAGS = -I m4

moddir = @APACHE_MODULEDIR@
mod_LTLIBRARIES = mod_sass.la

mod_sass_la_SOURCES = mod_sass.c \
	libsass/context.cpp libsass/functions.cpp \
	libsass/document.cpp libsass/constants.cpp libsass/document_parser.cpp \
	libsass/eval_apply.cpp libsass//node.cpp libsass/node_factory.cpp \
	libsass/node_emitters.cpp libsass//prelexer.cpp libsass/selector.cpp \
	libsass/sass_interface.cpp
mod_sass_la_CFLAGS = @APACHE_CFLAGS@ @APACHE_INCLUDES@
mod_sass_la_CPPFLAGS = @APACHE_CPPFLAGS@ @APACHE_INCLUDES@
mod_sass_la_LDFLAGS = -avoid-version -module @APACHE_LDFLAGS@
mod_sass_la_LIBS = @APACHE_LIBS@
