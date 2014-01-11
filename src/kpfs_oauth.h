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

#ifndef	__KPFS_OAUTH_H__
#define	__KPFS_OAUTH_H__

#include <string.h>
#include "kpfs.h"

typedef struct _kpfs_oauth {
	char oauth_token_secret[64];
	char oauth_token[64];
	char oauth_verifier[64];
} kpfs_oauth;

extern kpfs_oauth g_kpfs_oauth;
void kpfs_oauth_init();
kpfs_ret kpfs_oauth_load();
kpfs_ret kpfs_oauth_request_token();
kpfs_ret kpfs_oauth_authorize();
kpfs_ret kpfs_oauth_access_token();
char *kpfs_oauth_token_secret_get();
char *kpfs_oauth_token_get();
#endif
