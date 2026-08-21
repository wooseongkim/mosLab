#ifndef MINIOS_FS_H
#define MINIOS_FS_H

/**
 * @file fs.h
 * @brief LAB7 inode-style file system API.
 */

#include <stddef.h>

#include "minios/blockdev.h"
#include "minios/error.h"

/** @brief inode 번호. */
typedef int mos_inode_t;

/** @brief 공개 파일 메타데이터. */
typedef struct mos_file_stat {
    mos_inode_t inode; /**< inode 번호. */
    size_t size;       /**< 파일 크기. */
} mos_file_stat_t;

/** @brief 파일 시스템을 초기화한다. */
mos_status_t mos_fs_init(mos_blockdev_t *device);

/** @brief 새 파일을 만든다. */
mos_status_t mos_fs_create(const char *path, mos_inode_t *inode_out);

/** @brief 파일에 데이터를 쓴다. */
mos_status_t mos_fs_write(const char *path, const void *buffer, size_t size);

/** @brief 파일에서 데이터를 읽는다. */
mos_status_t mos_fs_read(const char *path, void *buffer, size_t buffer_size, size_t *read_out);

/** @brief 파일 메타데이터를 조회한다. */
mos_status_t mos_fs_stat(const char *path, mos_file_stat_t *stat_out);

#endif
