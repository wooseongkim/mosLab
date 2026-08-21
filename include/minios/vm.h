#ifndef MINIOS_VM_H
#define MINIOS_VM_H

/**
 * @file vm.h
 * @brief LAB6 virtual memory translation API.
 */

#include <stddef.h>

#include "minios/error.h"
#include "minios/kernel.h"
#include "minios/memory.h"
#include "minios/process.h"

/** @brief 가상 주소 타입. */
typedef size_t mos_vaddr_t;

/** @brief 물리 주소 타입. */
typedef size_t mos_paddr_t;

#define MOS_VM_PAGE_SIZE 256U
#define MOS_VM_MAX_MAPPINGS_PER_PROCESS 64U

/** @brief 단일 page mapping 정보. */
typedef struct mos_vm_mapping {
    mos_vaddr_t virtual_page; /**< 가상 페이지 번호. */
    mos_frame_t frame;        /**< 매핑된 물리 프레임 번호. */
    int writable;             /**< 쓰기 가능 여부. */
} mos_vm_mapping_t;

/** @brief 프로세스별 가상 주소 공간 관찰 정보. */
typedef struct mos_address_space {
    mos_pid_t pid;                                           /**< 주소 공간 소유 PID. */
    mos_vm_mapping_t mappings[MOS_VM_MAX_MAPPINGS_PER_PROCESS]; /**< page table entries. */
    size_t mapping_count;                                    /**< 사용 중인 mapping 수. */
} mos_address_space_t;

/** @brief page table을 초기화한다. */
mos_status_t mos_vm_init(mos_kernel_t *kernel);

/** @brief 가상 페이지를 물리 프레임에 매핑한다. */
mos_status_t mos_vm_map(mos_kernel_t *kernel, mos_pid_t pid, mos_vaddr_t virtual_page, mos_frame_t frame, int writable);

/** @brief 가상 페이지 매핑을 해제한다. */
mos_status_t mos_vm_unmap(mos_kernel_t *kernel, mos_pid_t pid, mos_vaddr_t virtual_page);

/** @brief 가상 주소를 물리 주소로 변환한다. */
mos_status_t mos_vm_translate(const mos_kernel_t *kernel, mos_pid_t pid, mos_vaddr_t virtual_address, mos_paddr_t *physical_out);

#endif
