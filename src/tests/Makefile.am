include $(abs_top_builddir)/src/rules.mk

noinst_PROGRAMS = \
	kpfs_test_request_token		\
	kpfs_test_access_token		\
	kpfs_test_metadata		\
	kpfs_test_download_by_range	\
	kpfs_test_upload_file
	
AM_CPPFLAGS = -DFUSE_USE_VERSION=26

kpfs_test_request_token_SOURCES = kpfs_test_request_token.c
kpfs_test_request_token_LDADD = $(MYLDFLAGS)

kpfs_test_access_token_SOURCES = kpfs_test_access_token.c
kpfs_test_access_token_LDADD = $(MYLDFLAGS)

kpfs_test_metadata_SOURCES = kpfs_test_metadata.c
kpfs_test_metadata_LDADD = $(MYLDFLAGS)

kpfs_test_download_by_range_SOURCES = kpfs_test_download_by_range.c
kpfs_test_download_by_range_LDADD = $(MYLDFLAGS)

kpfs_test_upload_file_SOURCES = kpfs_test_upload_file.c
kpfs_test_upload_file_LDADD = $(MYLDFLAGS)
