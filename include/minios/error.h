#ifndef MINIOS_ERROR_H
#define MINIOS_ERROR_H

/**
 * @file error.h
 * @brief miniOS 공개 API가 사용하는 상태 코드.
 */

/**
 * @brief miniOS 함수의 공통 반환 코드.
 */
typedef enum mos_status {
    MOS_OK = 0,                /**< 성공. */
    MOS_ERR_NULL = 1,          /**< 필수 포인터가 NULL이다. */
    MOS_ERR_INVALID = 2,       /**< 인자가 유효하지 않다. */
    MOS_ERR_STATE = 3,         /**< 현재 상태에서 수행할 수 없다. */
    MOS_ERR_NOT_FOUND = 4,     /**< 요청한 객체를 찾지 못했다. */
    MOS_ERR_NO_SPACE = 5,      /**< 고정 자원이 부족하다. */
    MOS_ERR_RANGE = 6,         /**< 인덱스, 주소, 크기가 범위를 벗어났다. */
    MOS_ERR_BLOCKED = 7,       /**< 호출한 프로세스가 대기 상태로 전환되었다. */
    MOS_ERR_UNIMPLEMENTED = 99 /**< 학생 구현이 아직 제공되지 않았다. */
} mos_status_t;

#endif
