# LAB0 개발환경 구축

## 대상 환경

- Windows 11
- WSL Ubuntu
- GCC
- GNU Make
- Visual Studio Code Remote-WSL
- GitHub Copilot

## WSL 설치

PowerShell을 관리자 권한으로 열고 Ubuntu를 설치한다.

```powershell
wsl --install -d Ubuntu
```

설치 후 Ubuntu 터미널에서 패키지를 갱신한다.

```sh
sudo apt update
sudo apt install -y build-essential git make gcc
```

## VS Code Remote-WSL

VS Code에서 Remote Development 확장을 설치한다. Ubuntu 터미널에서 저장소 루트로 이동한 뒤 다음 명령을 실행한다.

```sh
code .
```

좌측 하단에 `WSL: Ubuntu`가 표시되는지 확인한다.

## Copilot 사용 방식

VS Code 좌측 하단이 `WSL: Ubuntu`인 상태에서 GitHub 계정으로 로그인한다. GitHub Copilot 확장을 설치한 뒤 Command Palette에서 `GitHub Copilot: Sign in`을 실행한다.

학생 라이선스가 있는 경우 GitHub 웹 설정에서 Student Developer Pack 또는 학교 계정 혜택이 활성화되어 있는지 확인한다. VS Code 상태 표시줄의 Copilot 아이콘이 활성 상태인지 확인하고, 비활성 상태이면 `GitHub Copilot: Check Status`를 실행한다.

Chat 또는 Agent 모드는 저장소 루트에서 열어야 한다. Copilot Chat에는 `src/` 파일의 한국어 주석, 해당 LAB의 `include/minios/*.h`, 실패한 테스트 로그를 함께 제공한다. Agent에게는 `include/minios/` 공개 API를 바꾸지 말고 `src/` 안에서 구현하라고 명시한다.

좋은 요청 예시는 다음과 같다.

```text
LAB3 scheduler.c의 공개 API 계약과 주석을 기준으로 C99 round-robin ready queue를 구현해 줘.
private helper는 src/private 아래에 추가해도 되지만 include/minios는 변경하지 마.
모든 범위 오류와 NULL 포인터를 mos_status_t로 반환해.
```

테스트가 실패하면 실패 로그 전체를 다시 Copilot Chat에 붙여 넣고 다음 순서로 요청한다.

```text
아래 make test-lab03 실패 로그를 기준으로 원인을 분석해 줘.
include/minios/scheduler.h 계약을 어기지 말고 src/scheduler.c만 고쳐 줘.
고친 뒤 다시 실행할 명령도 알려 줘.
```

변경 후에는 Source Control 뷰에서 diff를 열어 Copilot이 `include/minios/`, `tests/`, `Makefile`을 바꾸지 않았는지 확인한다. 실습 중 학생이 수정하는 기본 위치는 `src/`와 개인 설계 설명을 적는 `README.md`이다.

## LAB0 검증

저장소 루트에서 다음 명령을 실행한다.

```sh
./scripts/verify_lab0.sh
make verify-instructor
```

`verify_lab0.sh`가 GCC, Make, ar, WSL 여부를 확인하고, `make verify-instructor`가 VM 라이브러리 테스트를 통과하면 LAB0 준비가 끝난다.

각 LAB 구현 후에는 해당 공개 테스트를 먼저 실행한다.

```sh
make test-lab01
make test-lab02
```

여러 LAB을 연달아 수정한 뒤에는 전체 공개 테스트를 실행한다.

```sh
make test
```
