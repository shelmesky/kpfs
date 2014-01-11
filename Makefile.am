## Process this file with automake to produce Makefile.in
SUBDIRS = src #docs

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = kpfs.pc

ACLOCAL_AMFLAGS = -I m4

confdir = $(datadir)/kpfs/
conf_DATA = kpfs.conf

EXTRA_DIST = autogen.sh kpfs.pc.in gtk-doc.make

kpfsdir = $(includedir)/kpfs
kpfs_HEADERS = config.h
