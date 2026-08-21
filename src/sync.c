#include "minios/sync.h"

/*
 * LAB4 구현 안내
 * - semaphore 카운터와 대기 큐를 어떻게 결합할지 설계한다.
 * - mutex는 소유자 추적, 중복 lock, unlock 오류 처리를 분명히 해야 한다.
 * - 단일 스레드 시뮬레이션이어도 race-free 추상 동작을 문서화하며 구현한다.
 */

mos_status_t mos_sem_init(mos_semaphore_t *sem, int initial_value) {
    (void)sem;
    (void)initial_value;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_sem_wait(mos_semaphore_t *sem) {
    (void)sem;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_sem_signal(mos_semaphore_t *sem) {
    (void)sem;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_mutex_init(mos_mutex_t *mutex) {
    (void)mutex;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_mutex_lock(mos_mutex_t *mutex) {
    (void)mutex;
    return MOS_ERR_UNIMPLEMENTED;
}

mos_status_t mos_mutex_unlock(mos_mutex_t *mutex) {
    (void)mutex;
    return MOS_ERR_UNIMPLEMENTED;
}
