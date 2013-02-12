
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = libsass

moddir = @APACHE_MODULEDIR@
mod_LTLIBRARIES = mod_sass.la

mod_sass_la_SOURCES = mod_sass.c
mod_sass_la_CFLAGS = @APACHE_CFLAGS@ @APACHE_INCLUDES@
mod_sass_la_CPPFLAGS = @APACHE_CPPFLAGS@ @APACHE_INCLUDES@
mod_sass_la_LDFLAGS = -avoid-version -module @APACHE_LDFLAGS@
mod_sass_la_LIBS = @APACHE_LIBS@

mod_sass_la_LIBADD = $(SUBDIRS)/libsass.la
