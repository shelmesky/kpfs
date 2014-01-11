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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define __STRICT_ANSI__
#include <json/json.h>

#include "kpfs_conf.h"

kpfs_conf g_kpfs_conf;

char *kpfs_conf_get_consumer_key()
{
	return g_kpfs_conf.consumer_key;
}

char *kpfs_conf_get_consumer_secret()
{
	return g_kpfs_conf.consumer_secret;
}

char *kpfs_conf_get_root()
{
	return g_kpfs_conf.root;
}

char *kpfs_conf_get_mount_point()
{
	return g_kpfs_conf.mount_point;
}

char *kpfs_conf_get_oauth_json_file()
{
	return g_kpfs_conf.oauth_json_file;
}

char *kpfs_conf_get_writable_tmp_path()
{
	return g_kpfs_conf.writable_tmp_path;
}

static kpfs_ret kpfs_conf_parse_json(const char *buf)
{
	json_object *jobj = NULL;

	if (!buf)
		return KPFS_RET_FAIL;

	jobj = json_tokener_parse(buf);

	if (!jobj || is_error(jobj))
		return KPFS_RET_FAIL;

	memset(&g_kpfs_conf, 0, sizeof(g_kpfs_conf));

	json_object_object_foreach(jobj, key, val) {
		if (!strcmp(key, KPFS_CONF_ID_CONSUMER_KEY)) {
			if (json_type_string == json_object_get_type(val))
				snprintf(g_kpfs_conf.consumer_key, sizeof(g_kpfs_conf.consumer_key), "%s", json_object_get_string(val));
		} else if (!strcmp(key, KPFS_CONF_ID_CONSUMER_SECRET)) {
			if (json_type_string == json_object_get_type(val))
				snprintf(g_kpfs_conf.consumer_secret, sizeof(g_kpfs_conf.consumer_secret), "%s", json_object_get_string(val));
		} else if (!strcmp(key, KPFS_CONF_ID_ROOT)) {
			if (json_type_string == json_object_get_type(val)) {
				snprintf(g_kpfs_conf.root, sizeof(g_kpfs_conf.root), "%s", json_object_get_string(val));
				if (0 != strcmp(KPFS_API_ROOT_KUAIPAN, g_kpfs_conf.root) && 0 != strcmp(KPFS_API_ROOT_APP_FOLDER, g_kpfs_conf.root)) {
					snprintf(g_kpfs_conf.root, sizeof(g_kpfs_conf.root), "%s", KPFS_API_ROOT_KUAIPAN);
				}
			}
		} else if (!strcmp(key, KPFS_CONF_ID_MOUNT_POINT)) {
			if (json_type_string == json_object_get_type(val))
				snprintf(g_kpfs_conf.mount_point, sizeof(g_kpfs_conf.mount_point), "%s", json_object_get_string(val));
		} else if (!strcmp(key, KPFS_CONF_ID_OAUTH_JSON_FILE)) {
			if (json_type_string == json_object_get_type(val))
				snprintf(g_kpfs_conf.oauth_json_file, sizeof(g_kpfs_conf.oauth_json_file), "%s", json_object_get_string(val));
		} else if (!strcmp(key, KPFS_CONF_ID_WRITABLE_TMP_PATH)) {
			if (json_type_string == json_object_get_type(val))
				snprintf(g_kpfs_conf.writable_tmp_path, sizeof(g_kpfs_conf.writable_tmp_path), "%s", json_object_get_string(val));
		}
	}
	json_object_put(jobj);

	if (g_kpfs_conf.mount_point[0] == '\0') {
		return KPFS_RET_FAIL;
	}

	if (g_kpfs_conf.consumer_key[0] == '\0')
		snprintf(g_kpfs_conf.consumer_key, sizeof(g_kpfs_conf.consumer_key), "%s", KPFS_CONSUMER_KEY);
	if (g_kpfs_conf.consumer_secret[0] == '\0')
		snprintf(g_kpfs_conf.consumer_secret, sizeof(g_kpfs_conf.consumer_secret), "%s", KPFS_CONSUMER_SECRET);
	if (g_kpfs_conf.root[0] == '\0')
		snprintf(g_kpfs_conf.root, sizeof(g_kpfs_conf.root), "%s", KPFS_API_ROOT_KUAIPAN);
	if (g_kpfs_conf.oauth_json_file[0] == '\0')
		snprintf(g_kpfs_conf.oauth_json_file, sizeof(g_kpfs_conf.oauth_json_file), "%s", KPFS_DEFAULT_OAUTH_JSON_FILE);
	if (g_kpfs_conf.writable_tmp_path[0] == '\0')
		snprintf(g_kpfs_conf.writable_tmp_path, sizeof(g_kpfs_conf.writable_tmp_path), "%s", KPFS_DEFAULT_WRITABLE_TMP_PATH);

	return KPFS_RET_OK;
}

kpfs_ret kpfs_conf_load(char *file)
{
	struct stat st;
	int fd = 0;
	char *buf = NULL;
	int ret = 0;

	if (NULL == file)
		return KPFS_RET_FAIL;

	if (0 != stat(file, &st)) {
		return KPFS_RET_FAIL;
	}
	fd = open(file, O_RDONLY);
	if (-1 == fd) {
		KPFS_LOG("fail to open file (%s)\n", file);
		return KPFS_RET_FAIL;
	}

	buf = calloc(st.st_size, 1);
	ret = read(fd, buf, st.st_size);
	if (-1 == ret) {
		KPFS_LOG("fail to read file (%s)\n", file);
	}
	close(fd);
	if (KPFS_RET_FAIL == kpfs_conf_parse_json(buf)) {
		KPFS_LOG("fail to load correct conf params from (%s).\n", file);
		KPFS_SAFE_FREE(buf);
		return KPFS_RET_FAIL;
	}
	KPFS_SAFE_FREE(buf);
	return KPFS_RET_OK;
}

void kpfs_conf_dump()
{
	KPFS_LOG("kpfs conf:\n");
	KPFS_LOG("\tconsumer_key: %s\n", g_kpfs_conf.consumer_key);
	KPFS_LOG("\tconsumer_secret: %s\n", g_kpfs_conf.consumer_secret);
	KPFS_LOG("\troot: %s\n", g_kpfs_conf.root);
	KPFS_LOG("\tmount_point: %s\n", g_kpfs_conf.mount_point);
	KPFS_LOG("\toauth_json_file: %s\n", g_kpfs_conf.oauth_json_file);
	KPFS_LOG("\twritable_tmp_path: %s\n", g_kpfs_conf.writable_tmp_path);
}
