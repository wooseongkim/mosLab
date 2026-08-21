#ifndef MOSVM_VM_ERROR_H
#define MOSVM_VM_ERROR_H

/**
 * @file vm_error.h
 * @brief 교수 제공 가상 하드웨어 라이브러리의 상태 코드.
 */

/**
 * @brief VM 라이브러리 함수가 반환하는 공통 상태 코드.
 *
 * 모든 VM API는 실패를 조용히 무시하지 않고 이 열거형 값으로 호출자에게
 * 원인을 알려야 한다. 학생 miniOS는 이 값을 자신의 오류 처리 정책으로
 * 변환하거나 그대로 전파할 수 있다.
 */
typedef enum vm_status {
    VM_OK = 0,          /**< 작업이 성공했다. */
    VM_ERR_NULL = 1,    /**< 필수 포인터 인자가 NULL이다. */
    VM_ERR_RANGE = 2,   /**< 주소, 크기, 인덱스가 VM 자원 범위를 벗어났다. */
    VM_ERR_STATE = 3,   /**< 현재 VM 또는 CPU 상태에서 허용되지 않는 작업이다. */
    VM_ERR_FULL = 4,    /**< 고정 크기 버퍼에 더 이상 기록할 공간이 없다. */
    VM_ERR_BAD_ARG = 5  /**< 인자 값이 API 계약을 만족하지 않는다. */
} vm_status_t;

#endif
