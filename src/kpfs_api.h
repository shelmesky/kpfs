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

#ifndef	__KPFS_API_H__
#define	__KPFS_API_H__

char *kpfs_api_account_info();
char *kpfs_api_metadata(const char *path);
char *kpfs_api_download_link_create(const char *path);
kpfs_ret kpfs_api_create_folder(const char *path);
char *kpfs_api_upload_locate();
char *kpfs_api_upload_file(char *dest_fullpath, char *src_fullpath);
kpfs_ret kpfs_api_delete(const char *path);
kpfs_ret kpfs_api_move(const char *from_path, const char *to_path);
#endif
