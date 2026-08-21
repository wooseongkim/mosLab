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

학생은 `src/` 파일의 한국어 주석을 Copilot에게 구현 지시로 사용한다. 좋은 요청 예시는 다음과 같다.

```text
LAB3 scheduler.c의 공개 API 계약과 주석을 기준으로 C99 round-robin ready queue를 구현해 줘.
private helper는 src/private 아래에 추가해도 되지만 include/minios는 변경하지 마.
모든 범위 오류와 NULL 포인터를 mos_status_t로 반환해.
```

## LAB0 검증

저장소 루트에서 다음 명령을 실행한다.

```sh
./scripts/verify_lab0.sh
make verify-instructor
```

`verify_lab0.sh`가 GCC, Make, ar, WSL 여부를 확인하고, `make verify-instructor`가 VM 라이브러리 테스트를 통과하면 LAB0 준비가 끝난다.
