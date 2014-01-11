/*
   (c) Copyright 2012  Tao Yu

   All rights reserved.

   Written by Tao Yu <yut616@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef	__KPFS_CONF_H__
#define	__KPFS_CONF_H__

#include "kpfs.h"

typedef struct kpfs_conf_t kpfs_conf;
struct kpfs_conf_t {
	char consumer_key[64];
	char consumer_secret[64];
	char root[64];
	char mount_point[KPFS_MAX_PATH];
	char oauth_json_file[KPFS_MAX_PATH];
	char writable_tmp_path[KPFS_MAX_PATH];
};

char *kpfs_conf_get_consumer_key();
char *kpfs_conf_get_consumer_secret();
char *kpfs_conf_get_root();
char *kpfs_conf_get_mount_point();
char *kpfs_conf_get_oauth_json_file();
char *kpfs_conf_get_writable_tmp_path();
kpfs_ret kpfs_conf_load(char *file);
void kpfs_conf_dump();
#endif
