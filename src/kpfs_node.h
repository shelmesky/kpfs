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

#ifndef	__KPFS_NODE_H__
#define	__KPFS_NODE_H__

#include <glib.h>

typedef enum {
	KPFS_NODE_TYPE_FILE,
	KPFS_NODE_TYPE_FOLDER
} kpfs_node_type;

typedef struct kpfs_node_t kpfs_node;

struct kpfs_node_t {
	char *fullpath;
	char *id;
	char *name;
	char *revision;
	kpfs_node_type type;
	int is_deleted;
	struct stat st;
	pthread_mutex_t mutex;
	GHashTable *sub_nodes;
};

#define KPFS_NODE_LOCK(node)	pthread_mutex_lock(&(node->mutex));
#define KPFS_NODE_UNLOCK(node)	pthread_mutex_unlock(&(node->mutex));

kpfs_node *kpfs_node_root_get();
kpfs_node *kpfs_node_root_create(char *id, char *name, off_t size);
void kpfs_node_free(gpointer p);
kpfs_node *kpfs_node_get_by_path(kpfs_node * node, const char *path);
void kpfs_node_dump(kpfs_node * node);
int kpfs_node_get_root_path();
kpfs_ret kpfs_node_parse_dir(kpfs_node * parent_node, const char *path);
kpfs_ret kpfs_node_rebuild(kpfs_node * node);
#endif
