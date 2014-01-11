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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <oauth.h>

#define __STRICT_ANSI__
#include <json/json.h>

#include "kpfs.h"
#include "kpfs_oauth.h"
#include "kpfs_conf.h"

#define OAUTH_TOKEN_ID		"oauth_token"
#define OAUTH_TOKEN_SECRET_ID	"oauth_token_secret"

#define KPFS_USER_ID		"user_id"
#define KPFS_USER_CHARGED_DIR	"charged_dir"

kpfs_oauth g_kpfs_oauth;

typedef struct _kpfs_user_info {
	char user_id[64];
	char charged_dir[KPFS_MAX_PATH];
} kpfs_user_info;

kpfs_user_info g_kpfs_user_info;

void kpfs_oauth_init()
{
	memset(&g_kpfs_oauth, 0, sizeof(g_kpfs_oauth));
}

static kpfs_ret kpfs_oauth_parse_json(const char *buf)
{
	json_object *jobj = NULL;

	if (!buf)
		return KPFS_RET_FAIL;

	jobj = json_tokener_parse(buf);

	if (!jobj || is_error(jobj))
		return KPFS_RET_FAIL;

	json_object_object_foreach(jobj, key, val) {
		if (!strcmp(key, OAUTH_TOKEN_ID)) {
			if (json_type_string == json_object_get_type(val)) {
				snprintf(g_kpfs_oauth.oauth_token, sizeof(g_kpfs_oauth.oauth_token), "%s", json_object_get_string(val));
			}
		} else if (!strcmp(key, OAUTH_TOKEN_SECRET_ID)) {
			if (json_type_string == json_object_get_type(val)) {
				snprintf(g_kpfs_oauth.oauth_token_secret, sizeof(g_kpfs_oauth.oauth_token_secret), "%s", json_object_get_string(val));
			}
		} else if (!strcmp(key, KPFS_USER_ID)) {
			if (json_type_int == json_object_get_type(val)) {
				snprintf(g_kpfs_user_info.user_id, sizeof(g_kpfs_user_info.user_id), "%d", json_object_get_int(val));
			}
		} else if (!strcmp(key, KPFS_USER_CHARGED_DIR)) {
			if (json_type_string == json_object_get_type(val)) {
				snprintf(g_kpfs_user_info.charged_dir, sizeof(g_kpfs_user_info.charged_dir), "%s", json_object_get_string(val));
			}
		}
	}
	json_object_put(jobj);

	return KPFS_RET_OK;
}

static kpfs_ret kpfs_oauth_json_get_filename(char *file, int len)
{
	if (NULL == file)
		return KPFS_RET_FAIL;
	snprintf(file, len, "%s", kpfs_conf_get_oauth_json_file());
	return KPFS_RET_OK;
}

static kpfs_ret kpfs_oauth_save_to_json(char *buf, int len)
{
	int fd = 0;
	int ret = 0;
	char file[KPFS_MAX_PATH] = { 0 };

	if (NULL == buf)
		return KPFS_RET_FAIL;
	kpfs_oauth_json_get_filename(file, sizeof(file));
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (-1 == fd) {
		KPFS_LOG("fail to open file (%s)\n", file);
		return KPFS_RET_FAIL;
	}
	ret = write(fd, buf, len);
	if (-1 == ret) {
		KPFS_LOG("fail to write file (%s)\n", file);
	}
	close(fd);
	return KPFS_RET_OK;
}

kpfs_ret kpfs_oauth_load()
{
	struct stat st;
	int fd = 0;
	int ret = 0;
	char file[KPFS_MAX_PATH] = { 0 };
	char *buf = NULL;

	kpfs_oauth_json_get_filename(file, sizeof(file));
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
	if (KPFS_RET_OK == kpfs_oauth_parse_json(buf)) {
		if (g_kpfs_oauth.oauth_token[0] != '\0') {
			KPFS_LOG("success to load kpfs json from file (%s)\n", file);
			KPFS_SAFE_FREE(buf);
			return KPFS_RET_OK;
		}
	}
	KPFS_LOG("could not read oauth token from (%s)\n", file);
	KPFS_SAFE_FREE(buf);
	return KPFS_RET_FAIL;
}

kpfs_ret kpfs_oauth_request_token()
{
	const char *request_token_uri = KPFS_API_REQUEST_TOKEN;
	char *req_url = NULL;
	char *reply = NULL;
	kpfs_ret ret = KPFS_RET_OK;

	KPFS_LOG("request token ...\n");

	req_url =
	    oauth_sign_url2(request_token_uri, NULL, OA_HMAC, NULL, (const char *)kpfs_conf_get_consumer_key(),
			    (const char *)kpfs_conf_get_consumer_secret(), NULL, NULL);
	if (!req_url) {
		ret = KPFS_RET_FAIL;
		goto error_out;
	}

	reply = oauth_http_get(req_url, NULL);

	if (!reply) {
		ret = KPFS_RET_FAIL;
		goto error_out;
	}

	KPFS_LOG("response: %s\n", reply);

	if (KPFS_RET_OK != kpfs_oauth_parse_json(reply)) {
		ret = KPFS_RET_FAIL;
		goto error_out;
	}

	if (g_kpfs_oauth.oauth_token[0] == '\0') {
		KPFS_LOG("oauth_token is incorrect.\n");
		ret = KPFS_RET_FAIL;
		goto error_out;
	}
	KPFS_LOG("oauth_token: %s\n", g_kpfs_oauth.oauth_token);

error_out:
	KPFS_SAFE_FREE(req_url);
	KPFS_SAFE_FREE(reply);

	return ret;
}

kpfs_ret kpfs_oauth_authorize()
{
	char auth_url[512] = { 0 };
	char *str = NULL;

	snprintf(auth_url, sizeof(auth_url), "%s&oauth_token=%s", KPFS_API_USER_AUTH, g_kpfs_oauth.oauth_token);

	printf("Please open this link in your browser: \n\n%s\n\n", auth_url);
	printf("And enter your ID and Password on the page to authorize\n");
	printf("this application to access your kuaipan\n");
	printf("Please enter the authorization number here: \n");

	str = gets(g_kpfs_oauth.oauth_verifier);
	KPFS_LOG("\ngets: ret %s, oauth_verifier: %s\n", str, g_kpfs_oauth.oauth_verifier);
	if (g_kpfs_oauth.oauth_verifier[0] != '\0') {
		return KPFS_RET_OK;
	}
	return KPFS_RET_FAIL;
}

kpfs_ret kpfs_oauth_access_token()
{
	const char *access_token_uri = KPFS_API_ACCESS_TOKEN;
	char *t_key = g_kpfs_oauth.oauth_token;
	char *t_secret = g_kpfs_oauth.oauth_token_secret;
	char *req_url = NULL;
	char *reply = NULL;
	kpfs_ret ret = KPFS_RET_OK;

	KPFS_LOG("access token ...\n");

	req_url =
	    oauth_sign_url2(access_token_uri, NULL, OA_HMAC, NULL, (const char *)kpfs_conf_get_consumer_key(),
			    (const char *)kpfs_conf_get_consumer_secret(), t_key, t_secret);
	if (!req_url) {
		ret = KPFS_RET_FAIL;
		goto error_out;
	}

	reply = oauth_http_get(req_url, NULL);

	if (!reply) {
		ret = KPFS_RET_FAIL;
		goto error_out;
	}

	KPFS_LOG("response: %s\n", reply);

	if (0 != kpfs_oauth_parse_json(reply)) {
		ret = KPFS_RET_FAIL;
		goto error_out;
	}
	if (g_kpfs_oauth.oauth_token[0] == '\0') {
		KPFS_LOG("oauth_token is incorrect.\n");
		ret = KPFS_RET_FAIL;
		goto error_out;
	}
	kpfs_oauth_save_to_json(reply, strlen(reply));

	KPFS_LOG("oauth_token: %s\n", g_kpfs_oauth.oauth_token);
	KPFS_LOG("user id: %s\n", g_kpfs_user_info.user_id);
	KPFS_LOG("charged dir: %s\n", g_kpfs_user_info.charged_dir);

error_out:
	KPFS_SAFE_FREE(req_url);
	KPFS_SAFE_FREE(reply);

	return ret;
}

char *kpfs_oauth_token_secret_get()
{
	return g_kpfs_oauth.oauth_token_secret;
}

char *kpfs_oauth_token_get()
{
	return g_kpfs_oauth.oauth_token;
}
