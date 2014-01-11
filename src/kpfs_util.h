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

#ifndef	__KPFS_UTIL_H__
#define	__KPFS_UTIL_H__

#include "kpfs.h"

int kpfs_file_log(const char *fmt, ...);
kpfs_ret kpfs_util_account_info_store(char *user_name, char *user_id, off_t quota_total, off_t quota_used, off_t max_file_size);
off_t kpfs_util_account_quota_total_get();
off_t kpfs_util_account_quota_used_get();
void kpfs_util_account_info_dump();
char *kpfs_util_upload_locate_get();
char *kpfs_util_get_parent_path(char *path);
#endif
