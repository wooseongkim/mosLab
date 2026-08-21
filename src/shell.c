#include "minios/shell.h"

/*
 * LAB9 구현 안내
 * - tokenizer/parser/command dispatch를 private helper로 분리하면 Copilot이 이해하기 쉽다.
 * - 빈 줄, 너무 긴 줄, 알 수 없는 명령, exit 명령을 deterministic하게 처리한다.
 * - tests는 콘솔 문자열만 확인하므로 private parser 이름에 의존하지 않는다.
 */

mos_status_t mos_shell_init(mos_kernel_t *kernel) {
    (void)kernel;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_shell_execute_line(mos_kernel_t *kernel, const char *line, mos_shell_result_t *result_out) {
    (void)kernel;
    (void)line;
    (void)result_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_shell_run_script(mos_kernel_t *kernel, const char *script, char *output, size_t output_size) {
    (void)kernel;
    (void)script;
    (void)output;
    (void)output_size;
    return MOS_ERR_UNIMPLEMENTED;
}
