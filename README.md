# 톡희
새로운 아희 인터프리터

## 주요 기능
- 강력한 아희 인터프리팅
	- 톡희는 아희 표준을 준수합니다. 모든 [아희 테스트 코드](https://github.com/aheui/snippets)를 정상적으로 통과했습니다.
	- 128비트 정수 입출력 및 계산을 지원합니다.
- 강력한 유니코드 지원
	- 모든 플랫폼에서 정상적인 유니코드 입출력을 지원합니다(Windows와 Ubuntu에서 테스트 되었습니다.).
	- 여러 코드 포인트로 구성된 Grapheme Cluster를 한 글자로 제대로 인식합니다.
- 강력한 확장 지원
	- Lua를 이용한 확장 개발을 지원하며, 차후에 지원 언어를 더 확대할 예정입니다.
		- LuaSocket이 내장되어 있습니다.

## 사용 방법
```
$ ./talkheui <아희 코드 경로> [확장 경로]
```

## 다운로드
Windows와 Ubuntu 사용자는 소스 코드를 직접 컴파일 하실 필요 없이 실행 파일을 바로 다운로드 하실 수 있습니다. [이곳](https://github.com/kmc7468/talkheui/releases)에서 최신 버전의 실행 파일을 확인해 보세요.

## 컴파일
```
$ git clone -b stable https://github.com/kmc7468/talkheui.git
$ cd talkheui
$ cmake CMakeLists.txt
$ make
```
위 명령어를 차례대로 입력하면 가장 최신의 안정화 버전이 컴파일 됩니다. 만약 불안정화 버전을 포함하여 가장 최신 버전을 컴파일 하고 싶다면 `-b stable`을 `-b unstable`로 수정, 개발 버전을 포함하여 가장 최신 버전을 컴파일 하고 싶다면 `-b stable`을 삭제하면 됩니다.

### 컴파일 옵션
- 최대 최적화: cmake 명령어를 실행할 때 `-DCMAKE_CXX_FLAGS="-O4"`를 덧붙여 주세요. 일부 컴파일러에서는 지원되지 않을 수도 있습니다.
- 128비트 정수 사용: cmake 명령어를 실행할 때 `-DUSE_MULTIPRECISION=TRUE`를 덧붙여 주세요. Boost C++ Libraries 설치가 필요합니다.

## 예제
[이곳](https://github.com/kmc7468/talkheui/tree/master/examples)에서 최신 버전의 예제를 확인하실 수 있습니다. 또는 examples 디렉터리에서 현재 버전의 예제를 확인하실 수도 있습니다.

## 확장 개발 방법
[이곳](https://github.com/kmc7468/talkheui/tree/master/documents/확장)에서 최신 버전의 확장 개발 방법을 확인하실 수 있습니다. 또는 documents/확장 디렉터리에서 현재 버전의 예제를 확인하실 수도 있습니다.

## 외부 라이브러리
다음의 외부 라이브러리들은 **별도 설치가 필요**합니다.
- [Boost C++ Libraries](https://www.boost.org)
	- 128비트 정수 지원을 사용하지 않을 경우 필요하지 않습니다.

다음의 외부 라이브러리들은 별도 설치가 필요하지 않습니다.
- [JSON for Modern C++](https://github.com/nlohmann/json) version 3.6.1
- [Lua](https://www.lua.org) 5.3.5
- [LuaSocket](https://github.com/diegonehab/luasocket) 3.0
- [miniz](https://github.com/richgel999/miniz) 2.1.0
- [UTF8-CPP](https://github.com/nemtrif/utfcpp) 3.1
- [u5e](https://github.com/ruoso/u5e) commit 3b970d5