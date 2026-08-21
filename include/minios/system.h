#ifndef MINIOS_SYSTEM_H
#define MINIOS_SYSTEM_H

/**
 * @file system.h
 * @brief LAB10 integration API.
 */

#include "minios/error.h"
#include "minios/kernel.h"

/** @brief 통합 부팅 결과. */
typedef struct mos_system_report {
    mos_status_t status; /**< 통합 검증 상태. */
    unsigned int labs_ready; /**< 성공적으로 준비된 LAB 단계 수. */
} mos_system_report_t;

/** @brief 전체 miniOS subsystem을 순서대로 초기화한다. */
mos_status_t mos_system_boot(mos_kernel_t *kernel, mos_system_report_t *report_out);

/** @brief 통합 smoke workload를 실행한다. */
mos_status_t mos_system_run_demo(mos_kernel_t *kernel, mos_system_report_t *report_out);

#endif
