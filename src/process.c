#include "minios/process.h"

/*
 * LAB2 구현 안내
 * - PID 발급, PCB 저장소, 상태 전이를 직접 설계한다. tests는 public API만 호출한다.
 * - 고정 배열을 써도 되고 private 자료구조를 추가해도 된다.
 * - vm_cpu_context_init() 실패를 반드시 처리하고, PID 범위 검사를 빠뜨리지 않는다.
 * - 정답 구조체 필드명을 맞추려 하지 말고 동작 계약을 만족시키는 설계를 우선한다.
 */

mos_status_t mos_process_table_init(void) {
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_process_create(const vm_program_t *program, mos_pid_t *pid_out) {
    (void)program;
    (void)pid_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_process_get(mos_pid_t pid, mos_process_t **process_out) {
    (void)pid;
    (void)process_out;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_process_exit(mos_pid_t pid) {
    (void)pid;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_process_count(size_t *count_out) {
    (void)count_out;
    return MOS_ERR_UNIMPLEMENTED;
}
