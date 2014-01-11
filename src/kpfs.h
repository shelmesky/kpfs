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

#ifndef	__KPFS_H__
#define	__KPFS_H__

#define KPFS_APP_NAME			"kpfs"
#define KPFS_VERSION			"0.1"

#define KPFS_CONSUMER_KEY		"xc8D2NfL9c53vkrP"
#define KPFS_CONSUMER_SECRET		"p7Lo7Q6XBMipbHBw"

#define KPFS_API_REQUEST_TOKEN		"https://openapi.kuaipan.cn/open/requestToken"
#define KPFS_API_USER_AUTH		"https://www.kuaipan.cn/api.php?ac=open&op=authorise"
#define KPFS_API_ACCESS_TOKEN 		"https://openapi.kuaipan.cn/open/accessToken"

#define KPFS_OAUTH_VERSION		"1.0"
#define KPFS_SIGNATURE_METHOD		"HMAC-SHA1"

#define KPFS_MAX_PATH			(512)
#define KPFS_MAX_BUF			(4096)
#define KPFS_CURL_LOW_SPEED_LIMIT	(1)
#define KPFS_CURL_LOW_SPEED_TIMEOUT	(60)

#define KPFS_HTTP_GET			"GET"
#define KPFS_HTTP_POST			"POST"

#define KPFS_API_BASE_URL		"http://openapi.kuaipan.cn/"
#define KPFS_API_VERSION		"1"
#define KPFS_API_ROOT_KUAIPAN		"kuaipan"
#define KPFS_API_ROOT_APP_FOLDER	"app_folder"
#define KPFS_API_ACCOUNT_INFO		KPFS_API_BASE_URL KPFS_API_VERSION "/account_info"
#define KPFS_API_METADATA		KPFS_API_BASE_URL KPFS_API_VERSION "/metadata"
#define KPFS_API_CREATE_FOLDER		KPFS_API_BASE_URL KPFS_API_VERSION "/fileops/create_folder"
#define KPFS_API_MOVE			KPFS_API_BASE_URL KPFS_API_VERSION "/fileops/move"
#define KPFS_API_COPY			KPFS_API_BASE_URL KPFS_API_VERSION "/fileops/copy"
#define KPFS_API_DELETE			KPFS_API_BASE_URL KPFS_API_VERSION "/fileops/delete"

#define KPFS_API_THUMBNAIL		"http://conv.kuaipan.cn/1/fileops/thumbnail"
#define KPFS_API_UPLOAD_LOCATE		"http://api-content.dfs.kuaipan.cn/1/fileops/upload_locate"
#define KPFS_API_UPLOAD_FILE		"1/fileops/upload_file"
#define KPFS_API_DOWNLOAD_FILE		"http://api-content.dfs.kuaipan.cn/1/fileops/download_file"

#define KPFS_ID_MAX_FILE_SIZE		"max_file_size"
#define KPFS_ID_QUOTA_TOTAL		"quota_total"
#define KPFS_ID_QUOTA_USED		"quota_used"
#define KPFS_ID_USER_NAME		"user_name"
#define KPFS_ID_USER_ID			"user_id"
#define KPFS_ID_FILES			"files"
#define KPFS_ID_IS_DELETED		"is_deleted"
#define KPFS_ID_NAME			"name"
#define KPFS_ID_REV			"rev"
#define KPFS_ID_CREATE_TIME		"create_time"
#define KPFS_ID_MODIFY_TIME		"modify_time"
#define KPFS_ID_SIZE			"size"
#define KPFS_ID_TYPE			"type"
#define KPFS_ID_FILE_ID			"file_id"
#define KPFS_ID_PATH			"path"
#define KPFS_ID_ROOT			"root"
#define KPFS_ID_HASH			"hash"
#define KPFS_ID_MSG			"msg"
#define KPFS_ID_URL			"url"

#define KPFS_MSG_STR_REUSED_NONCE	"reused nonce"

#define KPFS_NODE_TYPE_STR_FILE		"file"
#define KPFS_NODE_TYPE_STR_FOLDER	"folder"

#define KPFS_CONF_ID_CONSUMER_KEY	"consumer_key"
#define KPFS_CONF_ID_CONSUMER_SECRET	"consumer_secret"
#define KPFS_CONF_ID_ROOT		"root"
#define KPFS_CONF_ID_MOUNT_POINT	"mount_point"
#define KPFS_CONF_ID_OAUTH_JSON_FILE	"oauth_json_file"
#define KPFS_CONF_ID_WRITABLE_TMP_PATH	"writable_tmp_path"

#define KPFS_DEFAULT_WRITABLE_TMP_PATH	"/tmp"
#define KPFS_DEFAULT_OAUTH_JSON_FILE	"kpfs_oauth.json"

int kpfs_file_log(const char *fmt, ...);

#define KPFS_DEBUG

#ifdef KPFS_DEBUG
#define KPFS_LOG(format,args...)	fprintf(stdout, format, ## args)
#define KPFS_FILE_LOG			kpfs_file_log
#else
#define KPFS_LOG(format,args...)
#define KPFS_FILE_LOG
#endif

#define KPFS_SAFE_FREE(a) { if (a) {free(a); a = NULL;} }

#define KPFS_LOG_FILE_NAME		"kpfs.log"

#define KPFS_COOKIE_FILE_NAME		"cookie.txt"

typedef enum {
	KPFS_RET_FAIL = -1,
	KPFS_RET_OK = 0,
} kpfs_ret;

#endif
