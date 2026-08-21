#include "minios/fs.h"

/*
 * LAB7 구현 안내
 * - inode table, directory entry, file data block 배치를 직접 설계한다.
 * - path 길이와 문자 범위를 검사하고, 같은 이름 생성/없는 파일 읽기를 구분한다.
 * - 공개 테스트는 파일 생성/읽기/쓰기/stat 동작만 확인한다.
 */

mos_status_t mos_fs_init(mos_blockdev_t *device) {
    (void)device;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_fs_create(const char *path, mos_inode_t *inode_out) {
    (void)path;
    (void)inode_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_fs_write(const char *path, const void *buffer, size_t size) {
    (void)path;
    (void)buffer;
    (void)size;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_fs_read(const char *path, void *buffer, size_t buffer_size, size_t *read_out) {
    (void)path;
    (void)buffer;
    (void)buffer_size;
    (void)read_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_fs_stat(const char *path, mos_file_stat_t *stat_out) {
    (void)path;
    (void)stat_out;
    return MOS_ERR_UNIMPLEMENTED;
}
