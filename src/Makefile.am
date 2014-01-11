## Process this file with automake to produce Makefile.in
include $(abs_top_builddir)/src/rules.mk

SUBDIRS = . tests

DIST_SUBDIRS = $(SUBDIRS)

lib_LTLIBRARIES = libkpfs.la

libkpfs_la_SOURCES = \
	kpfs_api.c	\
	kpfs_util.c	\
	kpfs_conf.c	\
	kpfs_curl.c	\
	kpfs_node.c	\
	kpfs_oauth.c
	
AM_CPPFLAGS = -DFUSE_USE_VERSION=26

bin_PROGRAMS = kpfs

kpfs_SOURCES = \
	kpfs.h		\
	kpfs.c

kpfs_LDADD = $(MYLDFLAGS)

EXTRA_DIST = \
	rules.mk	\
	kpfs.h		\
	kpfs_api.h	\
	kpfs_util.h	\
	kpfs_conf.h	\
	kpfs_curl.h	\
	kpfs_node.h	\
	kpfs_oauth.h

kpfsdir=$(includedir)/kpfs
kpfs_HEADERS = \
	kpfs.h		\
	kpfs_api.h	\
	kpfs_util.h	\
	kpfs_conf.h	\
	kpfs_curl.h	\
	kpfs_node.h	\
	kpfs_oauth.h
