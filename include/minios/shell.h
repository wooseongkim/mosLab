#ifndef MINIOS_SHELL_H
#define MINIOS_SHELL_H

/**
 * @file shell.h
 * @brief LAB9 shell REPL API.
 */

#include <stddef.h>

#include "minios/error.h"

/** @brief shell 명령 실행 결과. */
typedef struct mos_shell_result {
    mos_status_t status; /**< 명령 처리 상태. */
    int should_exit;     /**< REPL 종료 요청 여부. */
} mos_shell_result_t;

/** @brief shell 계층을 초기화한다. */
mos_status_t mos_shell_init(void);

/** @brief 한 줄 명령을 실행한다. */
mos_status_t mos_shell_execute_line(const char *line, mos_shell_result_t *result_out);

/** @brief 여러 줄 입력을 deterministic하게 실행한다. */
mos_status_t mos_shell_run_script(const char *script, char *output, size_t output_size);

#endif
