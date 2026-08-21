# mosLab: AI 에이전트 기반 miniOS 구현 실습

이 저장소는 교수자 배포본이다. `include/`, `apps/`, `tests/`, `docs/instructor/`, `scripts/`, `Makefile`, `lib/libmosvm.a`는 고정 API와 검증 기준을 제공한다.

학생은 `src/` 내부 구현과 이 README의 개인 설계 설명만 수정한다. `src/`에는 초기 정답 구현이 없으며, 각 파일의 한국어 주석이 LAB별 구현 목표와 주의사항을 설명한다.

로컬 파일 수정 금지는 보안 경계가 아니다. 채점은 학생 제출물에서 `src/**`와 `README.md`만 추출하고, 신뢰된 교수자 헤더, VM 라이브러리, 테스트와 결합해 수행한다.

## 빠른 시작

```sh
./scripts/verify_lab0.sh
make verify-instructor
make test-lab01
```

## 주요 명령

- `make help`: 학생 배포본에서 지원하는 명령 확인
- `make verify-instructor`: 공개 헤더, 앱, 테스트 syntax와 배포된 VM 라이브러리 검증
- `make minios`: 학생 `src/` 구현과 VM 라이브러리 링크
- `make demo-lab01` ... `make demo-lab10`: LAB별 콘솔 데모
- `make test-lab01` ... `make test-lab10`: LAB별 공개 자동검증
- `make test`: LAB1-LAB10 공개 테스트 전체 실행
- `make clean`: 빌드 산출물 정리. `lib/libmosvm.a`는 삭제하지 않는다.

## 교수자/CI 전용

학생 배포 Makefile은 VM 라이브러리를 재생성하지 않는다. 교수자 또는 CI가 VM 런타임을 갱신해야 할 때만 다음 명령을 사용한다.

```sh
make -C instructor vm-lib
```

채점 서버는 학생 저장소의 `instructor/`, `include/`, `tests/`, `lib/` 변경본을 사용하지 않는다.

## 개인 설계 설명

학생은 각 LAB을 구현하면서 선택한 자료구조, 오류 처리 정책, 범위 검사 전략을 여기에 기록한다. 공개 API는 `include/minios/`를 변경하지 않는다.
