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
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

#define __STRICT_ANSI__
#include <json/json.h>

#include "kpfs.h"
#include "kpfs_conf.h"
#include "kpfs_util.h"
#include "kpfs_api.h"

typedef struct kpfs_account_info_t kpfs_account_info;
struct kpfs_account_info_t {
	off_t max_file_size;
	char user_name[512];
	char user_id[64];
	off_t quota_total;
	off_t quota_used;
};

kpfs_account_info g_kpfs_account_info;

char g_kpfs_upload_locate_url[KPFS_MAX_PATH] = { 0 };

int kpfs_file_log(const char *fmt, ...)
{
	va_list ap;
	char tmp[4096] = { 0 };
	char buf[4096] = { 0 };
	char log_file[KPFS_MAX_PATH] = { 0 };
	int fd = 0;
	char ftime[64] = { 0 };
	struct timeval tv;
	time_t curtime;
	int ret = 0;

	gettimeofday(&tv, NULL);
	curtime = tv.tv_sec;
	strftime(ftime, sizeof(ftime), "%F %T", localtime(&curtime));

	va_start(ap, fmt);
	vsnprintf(tmp, sizeof(tmp), fmt, ap);
	va_end(ap);

	snprintf(buf, sizeof(buf), "[%s] %s", ftime, tmp);
	snprintf(log_file, sizeof(log_file), "%s/%s", kpfs_conf_get_writable_tmp_path(), KPFS_LOG_FILE_NAME);
	fd = open(log_file, O_APPEND | O_WRONLY | O_CREAT, 0666);
	if (-1 == fd)
		return -1;
	ret = write(fd, buf, strlen(buf));
	if (-1 == ret)
		printf("fail to write: %s.\n", log_file);
	close(fd);
	return 0;
}

kpfs_ret kpfs_util_account_info_store(char *user_name, char *user_id, off_t quota_total, off_t quota_used, off_t max_file_size)
{
	memset(&g_kpfs_account_info, 0, sizeof(g_kpfs_account_info));

	if (NULL == user_name || NULL == user_id)
		return KPFS_RET_FAIL;

	snprintf(g_kpfs_account_info.user_name, sizeof(g_kpfs_account_info.user_name), "%s", user_name);
	snprintf(g_kpfs_account_info.user_id, sizeof(g_kpfs_account_info.user_id), "%s", user_id);
	g_kpfs_account_info.quota_total = quota_total;
	g_kpfs_account_info.quota_used = quota_used;
	g_kpfs_account_info.max_file_size = max_file_size;

	return KPFS_RET_OK;
}

off_t kpfs_util_account_quota_total_get()
{
	return g_kpfs_account_info.quota_total;
}

off_t kpfs_util_account_quota_used_get()
{
	return g_kpfs_account_info.quota_used;
}

void kpfs_util_account_info_dump()
{
	KPFS_FILE_LOG("kpfs account info:\n");
	KPFS_FILE_LOG("\tuser_name: %s\n", g_kpfs_account_info.user_name);
	KPFS_FILE_LOG("\tuser_id: %s\n", g_kpfs_account_info.user_id);
	KPFS_FILE_LOG("\tquota_total: %lu\n", g_kpfs_account_info.quota_total);
	KPFS_FILE_LOG("\tquota_used: %lu\n", g_kpfs_account_info.quota_used);
	KPFS_FILE_LOG("\tmax_file_size: %lu\n", g_kpfs_account_info.max_file_size);
}

char *kpfs_util_upload_locate_get()
{
	char *response = NULL;
	json_object *jobj = NULL;

	if (g_kpfs_upload_locate_url[0] != '\0')
		return g_kpfs_upload_locate_url;

	response = kpfs_api_upload_locate();

	jobj = json_tokener_parse(response);
	if (NULL == jobj || is_error(jobj)) {
		KPFS_FILE_LOG("%s:%d, json_tokener_parse return error.\n", __FUNCTION__, __LINE__);
		KPFS_SAFE_FREE(response);
		return NULL;
	}
	json_object_object_foreach(jobj, key, val) {
		if (!strcmp(key, KPFS_ID_URL)) {
			if (json_type_string == json_object_get_type(val)) {
				snprintf(g_kpfs_upload_locate_url, sizeof(g_kpfs_upload_locate_url), "%s", json_object_get_string(val));
				KPFS_FILE_LOG("%s:%d, url: %s.\n", __FUNCTION__, __LINE__, g_kpfs_upload_locate_url);
			}
		}
	}
	json_object_put(jobj);

	KPFS_SAFE_FREE(response);

	return g_kpfs_upload_locate_url;
}

char *kpfs_util_get_parent_path(char *path)
{
	char *p = NULL;
	char *parent_path = NULL;

	if (NULL == path)
		return NULL;

	p = strrchr(path, '/');
	if (NULL == p)
		return NULL;
	parent_path = calloc(KPFS_MAX_PATH, 1);
	if (NULL == parent_path)
		return NULL;
	strncpy(parent_path, path, p - path);

	return parent_path;
}
