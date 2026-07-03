# MiniOS 전체 구현용 ChatGPT 프롬프트

아래 내용을 새 ChatGPT 대화에 입력하고 `MiniOS_Agentic_Lab_Starter.zip`을 함께 첨부한다. 이 프롬프트는 동일한 파일 바이트를 보장하는 것이 아니라, 공개 API와 검증 테스트가 요구하는 동일한 동작을 구현하도록 설계되었다.

---

당신은 운영체제 수업의 학생이며 Python으로 MiniOS reference implementation을 완성해야 한다. 첨부한 `MiniOS_Agentic_Lab_Starter.zip`을 압축 해제하고 파일을 직접 수정하며 실제 테스트를 실행하라.

## 최종 목표

다음 명령이 성공해야 한다.

```bash
python -m pip install -e ".[test]"
python tools/verify.py --chapter CH13
```

예상 최종 결과는 `Forbidden API check passed`와 `11 passed`이다.

## 절대 규칙

1. `tests/`, `tools/verify.py`, `tools/check_forbidden.py`를 수정하지 마라.
2. README에 정의된 public module, class, method 이름과 signature를 변경하지 마라.
3. test를 삭제, skip, xfail 처리하거나 assertion을 약화하지 마라.
4. `multiprocessing`, `threading`, `asyncio`, `queue`, `mmap`, `ctypes`, `subprocess`, `pickle`, native process API를 사용하지 마라.
5. Python `socket` import는 `minios/net/socket_transport.py`에서만 사용하라.
6. host OS의 scheduler, mutex, signal, process, virtual memory 기능으로 MiniOS 기능을 대신하지 마라.
7. 일반 파일 기반 `disk.img`, `swap.img`와 localhost TCP만 사용하라.
8. 한 챕터 구현 후 해당 챕터까지 누적 test를 실행하고, 실패하면 구현 코드만 수정하라.

## 구현 순서

### CH01 Virtual Hardware

- `Machine`, `CPU`, `Memory`, file-backed disk와 timer interrupt를 구현하라.
- CPU register는 `R0`~`R3`, PC와 halted 상태를 가진다.
- 필요한 instruction은 `MOVI`, `STORE8`, `LOAD8`, `ADD`, `HALT`다.
- RAM과 block 범위를 검사하고 잘못된 접근에는 예외를 발생시켜라.

검증:

```bash
python tools/verify.py --chapter CH01
```

### CH02 Kernel and Syscall

- `Kernel.boot()`, `Syscall.CONSOLE_WRITE`, console byte buffer를 구현하라.
- boot 이전 syscall과 알 수 없는 syscall에는 `SyscallError`를 발생시켜라.

### CH03 Process Lifecycle

- `PCB`와 `NEW`, `READY`, `RUNNING`, `BLOCKED`, `TERMINATED` 상태를 사용하라.
- PID는 1부터 중복 없이 발급하라.
- process instruction은 `COMPUTE n`, `EXIT code`다.
- 한 tick에 RUNNING process는 최대 하나여야 하며 `wait(pid)`가 exit code를 반환해야 한다.

### CH04 Synchronization and IPC

- host lock을 사용하지 말고 MiniOS `Mutex`를 직접 구현하라.
- owner는 하나이며 waiter는 FIFO deque로 관리하라.
- `SignalQueue`는 PID별 queue, `Mailbox`는 bounded message queue로 구현하라.
- owner가 아닌 process의 release, full mailbox와 empty receive를 검사하라.

### CH06 Round Robin

- `collections.deque`로 ready queue를 직접 구성하라.
- quantum이 끝나면 current PID를 queue 뒤로 이동시켜라.
- finished와 blocked process는 재삽입하지 마라.
- quantum 2의 첫 실행 순서는 `1,1,2,2,3,3`이어야 한다.

### CH07 Contiguous Memory

- first-fit allocator를 구현하라.
- allocation 결과는 `start`, `size`, 고유 token을 가져야 한다.
- free 후 인접 segment를 병합하고 double free를 거부하라.
- `check_invariants()`가 gap, overlap과 전체 크기를 검사하게 하라.

### CH08 Paging and Swap

- page key는 `(pid, virtual_page)`로 하여 process 주소 공간을 격리하라.
- frame 수가 부족하면 가장 오래 접근하지 않은 LRU page를 선택하라.
- swap slot은 `swap.img`의 `slot * page_size` 위치에 저장하라.
- cross-page read/write와 page fault, swap-in, swap-out 통계를 구현하라.

### CH09 Block I/O

- 일반 파일을 고정 크기 block device로 사용하라. 기존 파일은 mount 시 truncate하지 마라.
- `SCANQueue`는 현재 head 방향의 request를 먼저 처리하고 이후 반대 방향을 처리하라.
- 각 완료 request마다 `IO_COMPLETE` interrupt event를 한 개 생성하라.

### CH10 File System

- `MFS1` magic과 길이 header가 있는 영속 metadata를 block device에 저장하라.
- inode 0은 예약하고 inode bitmap과 block bitmap을 계산하라.
- 최소 API는 `format`, `mount`, `create`, `open`, `read`, `write`, `close`, `unlink`, `unmount`다.
- 파일 내용은 안전한 binary encoding으로 저장하고 remount 후 복원하라.
- open file descriptor는 3부터 발급하고 offset을 관리하라.

### CH11 Network IPC

- packet header는 network byte order `!4sIIIII`를 사용하라.
- 필드는 magic `MOS1`, source, destination, sequence, payload length, CRC32 checksum이다.
- TCP 전송에는 4-byte frame length prefix를 추가하라.
- socket 처리 코드는 오직 `socket_transport.py`에 두고 `MiniNode`는 transport를 사용하게 하라.
- malformed packet은 `PacketError`로 거부하라.

### CH13 Integration

- `MiniOS.boot()`가 Machine, block device, FileSystem, Kernel, Shell을 초기화하게 하라.
- 최초 disk는 format하고 기존 disk는 mount하라.
- shell에서 `echo text > /file`, `cat /file`, `run count n`, `ls`, `rm`, `ps`를 처리하라.
- shutdown 후 새 MiniOS 객체로 reboot하면 RAM과 process는 비어 있고 파일은 유지되어야 한다.

## 작업 방식

각 단계에서 다음 순서를 지켜라.

1. 대상 test와 현재 stub을 읽는다.
2. 구현 계획과 핵심 불변식을 5줄 이내로 설명한다.
3. test를 수정하지 않고 구현 파일만 편집한다.
4. `python tools/verify.py --chapter CHxx`를 실행한다.
5. 실패 stack trace의 최초 원인을 수정한다.
6. 이전 챕터 회귀 test까지 통과한 다음 다음 단계로 이동한다.

## 최종 출력

- 변경한 파일 목록
- 챕터별 핵심 설계 결정
- 금지 API 검사 결과
- 전체 pytest 결과 원문
- 남은 제한 사항

계획만 제안하고 멈추지 말고, 파일 수정과 테스트 실행을 끝까지 수행하라.

---

