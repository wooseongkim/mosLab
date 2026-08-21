# Agentic miniOS Lab Design

작성일: 2026-08-20

## 목표

이 저장소는 AI 에이전트 기반 miniOS 구현 실습의 교수자 배포본이다. 교수자는 VM 라이브러리, 공개 API, 빌드/검증 체계, 데모와 문서를 고정 제공한다. 학생은 `src/` 내부 구현과 `README.md`의 개인 설계 설명만 수정한다.

## 역할 분리

교수 제공 영역은 `include/mosvm`, `include/minios`, `instructor/vm_runtime`, `lib/libmosvm.a`, `apps`, `tests`, `scripts`, `docs/instructor`, `Makefile`이다. 공개 테스트는 `include/minios` API와 콘솔 출력만 사용하며 `src/private` 이름이나 내부 구조를 보지 않는다.

학생 구현 영역은 `src/` 전체와 개인 README 설명이다. `src/` 초기 파일은 정답을 담지 않고 구현 목표, 필수 동작, 범위 검사, 오류 처리 지침을 한국어 주석으로 제공한다.

## VM 라이브러리

VM은 실제 x86 에뮬레이터가 아니라 OS 실습용 가상 하드웨어다. 기본 사양은 page size 256 bytes, physical memory 32 frames, block size 128 bytes, block device 64 blocks다. VM은 machine lifecycle, simulated tick, 단순 CPU instruction, program image, raw memory, raw block device, deterministic console만 제공한다. PID, PCB, scheduler, 동기화 정책, frame allocator, page table, file system, shell parser, syscall policy는 제공하지 않는다.

## LAB 흐름

LAB1은 kernel lifecycle, LAB2는 process, LAB3은 round-robin scheduler, LAB4는 sync, LAB5는 frame allocator, LAB6은 virtual memory, LAB7은 file system, LAB8은 syscall, LAB9는 shell, LAB10은 integration이다. 각 단계는 이전 단계 구현을 누적 사용한다.

## 검증

`make verify-instructor`는 교수 제공 VM 런타임을 빌드하고 VM 공개 테스트를 실행한다. `make test-labNN`은 학생 구현의 해당 LAB 공개 테스트를 실행한다. LAB 테스트는 학생 구현 전 실패할 수 있으며, 학생은 실패 메시지를 기준으로 구현을 진행한다.
