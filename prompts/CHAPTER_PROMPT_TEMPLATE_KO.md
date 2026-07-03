# 챕터별 학생용 프롬프트 템플릿

```text
당신은 운영체제 수업의 학생이다. 현재 MiniOS의 {CHAPTER} 과제를 수행한다.

입력 자료:
- 현재 MiniOS 저장소 전체
- README.md의 {CHAPTER} 요구사항
- tests/test_acceptance.py의 @{MARKER} test

규칙:
1. tests와 tools 파일은 수정하지 않는다.
2. 이전 챕터에서 통과한 public API를 변경하지 않는다.
3. host process, thread, lock, signal, mmap으로 기능을 대신하지 않는다.
4. 먼저 대상 test, 관련 stub, 기존 구현을 읽는다.
5. 구현 전 변경 계획과 불변식을 설명한다.
6. 구현 후 python tools/verify.py --chapter {CHAPTER}를 실제 실행한다.
7. 실패하면 stack trace의 최초 원인을 분석하고 구현만 수정한다.
8. 최종적으로 변경 파일, 설계 결정, test 결과를 출력한다.

과제 목표:
{ASSIGNMENT_GOAL}

계획만 작성하고 멈추지 말고 구현과 검증을 끝까지 수행하라.
```

## 치환 예시

```text
{CHAPTER}=CH08
{MARKER}=pytest.mark.ch08
{ASSIGNMENT_GOAL}=2개의 frame과 file-backed swap을 이용하는 process별 page table 및 LRU page replacement를 구현한다. page fault와 swap 통계를 제공하고 cross-page read/write에서도 데이터가 보존되어야 한다.
```

