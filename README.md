# MiniOS Agentic Coding Lab

> 이 디렉터리는 전체 acceptance test를 통과하는 교수자용 reference implementation이다. 학생 배포용 skeleton은 `outputs/MiniOS_Agentic_Lab_Starter.zip`을 사용한다.

## 1. 프로젝트 정의

이 프로젝트의 MiniOS는 실제로 부팅되는 native OS가 아니라 Python으로 구현하는 결정적 사용자 공간 OS 시뮬레이터다. Linux 호스트는 Python과 pytest를 실행하는 용도로만 사용한다.

다음 기능은 사용하지 않는다.

- `fork`, `exec`, host process scheduler, kernel module, QEMU, root 권한
- `multiprocessing`, `threading`, `asyncio`, `queue.Queue`
- `mmap`, raw device, 실제 파티션 변경, raw socket
- host mutex나 host signal을 MiniOS 기능으로 대신하는 구현

일반 파일로 만든 `disk.img`와 CH11의 `127.0.0.1` TCP socket만 허용한다. TCP socket은 MiniOS 패킷을 전달하는 transport일 뿐이며 패킷, mailbox, process 상태는 학생 코드가 직접 구현한다.

## 2. 선택한 챕터

CH05 교착 상태와 CH12 보안은 MiniOS의 최소 동작에 필요하지 않아 정규 과제에서 제외한다. 교착 상태는 CH04의 선택 실험으로만 다룬다.

| 과제 | 연결 챕터 | 구현 기능 | 완료 조건 |
|---|---|---|---|
| A1 | CH01 | VirtualCPU, RAM, file-backed block device, timer IRQ | 프로그램 실행, RAM 저장, block read/write, timer interrupt |
| A2 | CH02 | kernel boot와 syscall dispatcher | 장치 초기화와 정상·잘못된 syscall 구분 |
| A3 | CH03 | PCB, 상태 전이, spawn, exit, wait | 복수 process가 생성되어 종료되고 자원이 회수됨 |
| A4 | CH04 | MiniOS Mutex, SignalQueue, Mailbox | lock 대기·인계, signal 전달, message 송수신 |
| A5 | CH06 | ready queue와 Round Robin scheduler | quantum 단위 선점과 공정한 순환 실행 |
| A6 | CH07 | contiguous allocator와 free block 병합 | first-fit 할당, 해제, 인접 free 영역 병합 |
| A7 | CH08 | page table, LRU, page fault, swap file | frame 부족 시 swap-out/in과 데이터 보존 |
| A8 | CH09 | block I/O controller와 SCAN request queue | request 정렬, read/write, I/O 완료 IRQ |
| A9 | CH10 | superblock, inode, bitmap, file API | format·mount·create·read·write·unlink와 재마운트 |
| A10 | CH11 | packet codec와 두 MiniOS node의 TCP transport | localhost에서 packet 송수신과 checksum 검사 |
| A11 | CH13 | shell와 전체 subsystem 통합 | 파일 생성, process 실행, 재부팅 후 데이터 유지 |

LRU는 CPU scheduler가 아니라 CH08의 page replacement policy로 구현한다. CH06에서는 MiniOS 동작에 필요한 RR만 필수로 한다.

## 3. 고정 API 계약

학생은 다음 module과 public API 이름을 유지한다. 내부 구현은 자유지만 공개 테스트가 사용하는 signature를 바꾸면 안 된다.

```text
minios.hardware.machine.Machine
minios.kernel.kernel.Kernel
minios.kernel.syscall.Syscall, SyscallError
minios.process.pcb.PCB, ProcessState
minios.sync.primitives.Mutex, SignalQueue, Mailbox
minios.scheduler.rr.RRScheduler
minios.memory.allocator.ContiguousAllocator
minios.vm.pager.Pager
minios.io.block.FileBlockDevice, IOController, IORequest, SCANQueue
minios.fs.filesystem.FileSystem
minios.net.packet.Packet, PacketError
minios.net.node.MiniNode
minios.system.MiniOS
```

## 4. 공통 agentic coding 절차

1. `python tools/verify.py --chapter CHxx`를 실행해 현재 실패를 확인한다.
2. AI agent에게 현재 과제의 공개 API와 실패한 test 하나만 전달한다.
3. agent는 먼저 변경 계획과 불변 조건을 작성한다.
4. 학생 승인 후 test 하나를 통과시키는 최소 변경만 수행한다.
5. 전체 누적 test와 forbidden API 검사를 다시 실행한다.
6. 학생은 diff를 검토하고 `prompts/CHxx.md`에 prompt, 실패 원인, 수정 근거를 기록한다.

권장 prompt:

```text
현재 CH08 과제를 수행한다.
CH01~CH07의 public API와 통과한 test를 변경하지 않는다.
host mmap, multiprocessing, threading은 사용할 수 없다.
먼저 실패한 test와 기존 코드를 읽고 구현 계획과 불변 조건을 제시하라.
한 번에 acceptance criterion 하나만 구현하고 pytest 결과를 보고하라.
```

## 5. 과제별 검증 방안

### A1 / CH01 Virtual Hardware

- 공개 검증: CPU instruction 종료, RAM 값, block round-trip, timer IRQ를 확인한다.
- 숨은 검증: RAM 범위 오류, 잘못된 block 번호, `max_ticks` 무한 실행 방지.
- 필수 불변식: CPU가 `HALT`하면 더 이상 PC가 증가하지 않는다.

### A2 / CH02 Kernel Boundary

- 공개 검증: `boot()`, console syscall, 잘못된 syscall 예외를 확인한다.
- 숨은 검증: boot 이전 syscall 거부, 중복 boot의 일관성, syscall argument 검사.
- 필수 불변식: user program은 device 객체를 직접 받지 않는다.

### A3 / CH03 Process Lifecycle

- 공개 검증: 두 PCB의 `NEW → READY → RUNNING → TERMINATED` 전이를 확인한다.
- 숨은 검증: PID 중복 방지, 종료 process 재실행 금지, wait 후 exit code 반환.
- 필수 불변식: 한 tick에 RUNNING process는 최대 하나다.

### A4 / CH04 Synchronization and IPC

- 공개 검증: mutex owner 인계, FIFO waiter, signal dequeue, mailbox 송수신을 확인한다.
- 숨은 검증: owner가 아닌 process의 release 거부, mailbox overflow·empty 처리.
- 필수 불변식: mutex owner는 동시에 하나뿐이다.

### A5 / CH06 Round Robin

- 공개 검증: quantum 2에서 실행 순서가 `1,1,2,2,3,3`인지 확인한다.
- 숨은 검증: 빈 queue, process 종료, blocked process 제외, quantum 1.
- 필수 불변식: READY process가 존재하면 유한 tick 안에 실행된다.

### A6 / CH07 Memory Allocation

- 공개 검증: first-fit 위치, free, 인접 free block 병합을 확인한다.
- 숨은 검증: 0·음수 크기, double free, out-of-memory, fragmentation 통계.
- 필수 불변식: allocated segment와 free segment는 겹치지 않는다.

### A7 / CH08 Virtual Memory

- 공개 검증: 2 frame에 3 page를 접근하여 LRU swap과 데이터 복구를 확인한다.
- 숨은 검증: process별 page table 격리, dirty page, cross-page read, swap slot 회수.
- 필수 불변식: 같은 `(pid, virtual page)`가 두 frame에 동시에 존재하지 않는다.

### A8 / CH09 Block I/O

- 공개 검증: SCAN 순서, block write/read, `IO_COMPLETE` IRQ를 확인한다.
- 숨은 검증: head 방향 반전, 동일 block request, 범위 오류, 완료 순서.
- 필수 불변식: 완료되지 않은 request는 완료 IRQ를 만들지 않는다.

### A9 / CH10 File System

- 공개 검증: format, mount, create, write, read, remount, unlink를 확인한다.
- 숨은 검증: duplicate path, block 경계 write, inode 고갈, bitmap 회수, 손상 superblock.
- 필수 불변식: 사용 중인 inode와 data block은 bitmap에서 할당 상태여야 한다.

### A10 / CH11 Network IPC

- 공개 검증: packet encode/decode와 localhost node 간 payload 전달을 확인한다.
- 숨은 검증: 잘못된 magic, 길이, checksum, timeout, 중복 sequence number.
- 필수 불변식: 검증에 실패한 packet은 mailbox에 전달하지 않는다.

### A11 / CH13 Integration

- 공개 검증: shell로 파일 생성·조회, process 실행, shutdown·reboot 후 파일 유지.
- 숨은 검증: subsystem 초기화 순서, 여러 명령 연속 실행, 이전 챕터 회귀 test.
- 필수 불변식: reboot 후 RAM과 process는 초기화되고 disk file만 유지된다.

## 6. 실행 방법

```bash
python -m pip install -e ".[test]"
python tools/check_forbidden.py
python tools/verify.py --chapter CH01
python tools/verify.py --chapter CH08
python tools/verify.py --chapter CH13
```

`verify.py`는 지정 챕터까지의 모든 과제를 누적 검증한다. 교수자용 hidden test도 동일한 marker 이름을 사용해 별도 디렉터리에서 추가할 수 있다.

## 7. 제출물

- `minios/` 구현 코드
- 공개 test와 학생이 추가한 unit test
- `prompts/CHxx.md` agent 작업 기록
- `reports/CHxx.md` 상태 흐름, 불변식, 실패 사례 설명
- `python tools/verify.py --chapter CHxx` 결과
- 챕터별 Git tag

권장 평가는 구현 35%, 불변식과 OS 개념 25%, test 20%, 회귀 안정성 10%, agent 사용 기록과 구두 설명 10%다.
