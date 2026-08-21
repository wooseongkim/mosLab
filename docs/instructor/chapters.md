# LAB1-LAB10 지도 요약

## LAB1 kernel lifecycle

VM 생성, 커널 상태 전이, timer tick 조회를 구현한다. 중복 boot, shutdown 전 tick, NULL 인자 정책을 정한다.

## LAB2 process

PID 발급, PCB 저장소, READY/RUNNING/EXITED 상태 전이를 구현한다. VM CPU context는 PCB에 저장할 수 있지만 스케줄링 정책은 아직 만들지 않는다.

## LAB3 scheduler

ready queue와 round-robin 순서를 구현한다. 테스트 예시 출력은 다음과 같다.

```text
$ make test-lab03
[PASS] scheduler initializes
[PASS] enqueue process 1
[PASS] enqueue process 2
LAB03 RESULT: 3 passed, 0 failed
```

## LAB4 sync

semaphore와 mutex를 구현한다. 단일 스레드 시뮬레이션이어도 BLOCKED 상태와 wake-up 규칙을 설계하게 한다.

## LAB5 memory

VM spec에서 frame 수와 page size를 읽고 frame allocator를 만든다. VM은 raw memory만 제공한다.

데모는 kernel boot, memory init, frame allocation, stats 조회를 순서대로 호출한다.

```text
$ make demo-lab05
LAB05 kernel boot status: 0
LAB05 memory init status: 0
LAB05 frame alloc status: 0
LAB05 memory stats status: 0
LAB05 observed frame=<allocated> total=<frames> free=<frames-1>
```

## LAB6 virtual memory

가상 페이지를 물리 프레임에 매핑하고 VA를 PA로 변환한다. page fault와 writable 정책은 miniOS가 결정한다.

데모는 LAB2에서 생성한 실제 PID와 LAB5 allocator에서 얻은 frame을 사용해 VA를 PA로 변환한다.

```text
$ make demo-lab06
LAB06 kernel boot status: 0
LAB06 process table init status: 0
LAB06 process create status: 0
LAB06 memory init status: 0
LAB06 frame alloc status: 0
LAB06 virtual memory init status: 0
LAB06 map status: 0
LAB06 translate status: 0
LAB06 observed pid=<created> frame=<allocated> physical=<frame * 256 + 7>
```

## LAB7 file system

raw block device 위에 inode-style file system을 만든다. 디렉터리, inode, data block 배치를 학생이 설계한다.

## LAB8 syscall

FD table과 open/read/write/close API를 만든다. TRAP 명령은 syscall 확장 지점으로만 제공된다.

데모는 block device와 file system을 초기화하고 `/log` 파일을 만든 뒤 syscall 계층에서 open/write/close를 수행한다.

```text
$ make demo-lab08
LAB08 kernel boot status: 0
LAB08 block device init status: 0
LAB08 file system init status: 0
LAB08 create /log status: 0
LAB08 syscall init status: 0
LAB08 open /log status: 0
LAB08 write /log status: 0
LAB08 close /log status: 0
LAB08 observed inode=<created> fd=<opened> written=3
```

## LAB9 shell

한 줄 명령 parser와 deterministic REPL script 실행을 만든다. 콘솔 출력 문자열이 공개 검증의 기준이다.

데모는 block device, file system, syscall, shell을 순서대로 초기화한 뒤 `help`와 `exit` 스크립트를 실행한다.

```text
$ make demo-lab09
LAB09 kernel boot status: 0
LAB09 block device init status: 0
LAB09 file system init status: 0
LAB09 create /readme status: 0
LAB09 syscall init status: 0
LAB09 shell init status: 0
LAB09 shell script status: 0
LAB09 observed output=<deterministic shell output>
```

## LAB10 integration

LAB1-LAB9를 순서대로 초기화하고 통합 workload를 실행한다. 실패 시 어느 단계까지 성공했는지 report에 남긴다.

```text
$ make demo-lab10
LAB10 system boot status: 0
LAB10 labs ready after boot: 10
LAB10 integration demo status: 0
LAB10 labs ready after demo: 10
```
