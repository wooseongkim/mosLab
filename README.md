# mosLab: AI 에이전트 기반 miniOS 구현 실습

이 저장소는 교수자 배포본이다. `include/`, `apps/`, `tests/`, `docs/instructor/`, `scripts/`, `Makefile`, `lib/libmosvm.a`는 고정 API와 검증 기준을 제공한다.

학생은 `src/` 내부 구현과 이 README의 개인 설계 설명만 수정한다. `src/`에는 초기 정답 구현이 없으며, 각 파일의 한국어 주석이 LAB별 구현 목표와 주의사항을 설명한다.

## 빠른 시작

```sh
./scripts/verify_lab0.sh
make verify-instructor
make test-lab01
```

## 주요 명령

- `make vm-lib`: 교수 제공 VM 정적 라이브러리 생성
- `make verify-instructor`: VM 라이브러리와 교수 제공 테스트 검증
- `make minios`: 학생 `src/` 구현과 VM 라이브러리 링크
- `make demo-lab01` ... `make demo-lab10`: LAB별 콘솔 데모
- `make test-lab01` ... `make test-lab10`: LAB별 공개 자동검증
- `make test`: LAB1-LAB10 공개 테스트 전체 실행
- `make clean`: 빌드 산출물 정리

## 개인 설계 설명

학생은 각 LAB을 구현하면서 선택한 자료구조, 오류 처리 정책, 범위 검사 전략을 여기에 기록한다. 공개 API는 `include/minios/`를 변경하지 않는다.
