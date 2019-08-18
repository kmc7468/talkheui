# 톡희
새로운 아희 인터프리터<br>
**아직 정식으로 공개되지 않은 버전이라 불안정 할 수 있습니다.**

## 주요 기능
- 강력한 아희 인터프리팅
	- 톡희는 아희 표준을 준수합니다. 모든 [아희 테스트 코드](https://github.com/aheui/snippets)를 정상적으로 통과했습니다.
	- 아래의 기능을 제공하고도 [caheui](https://github.com/aheui/caheui)보다 2배 밖에 느리지 않습니다.
- 강력한 유니코드 지원
	- 모든 플랫폼에서 정상적인 유니코드 입출력을 지원합니다(Windows와 Ubuntu에서 테스트 되었습니다.).
	- 여러 코드 포인트로 구성된 Grapheme Cluster를 한 글자로 제대로 인식합니다.
- 강력한 확장 지원
	- Lua를 이용한 확장 개발을 지원하며, 차후에 지원 언어를 더 확대할 예정입니다.
		- LuaSocket이 내장되어 있습니다.

## 사용 방법
```
talkheui <아희 코드 경로> [확장 경로]
```

## 컴파일
```
$ git clone https://github.com/kmc7468/talkheui.git
$ cd talkheui
$ cmake CMakeLists.txt
$ make
```

## 확장 개발
### 1. Lua로 스크립트 작성
반드시 필요한 함수는 다음과 같습니다.

|이름|매개 변수|반환|설명|
|:-:|:-:|:-:|:-|
|send||number(정수)|아희 코드에서 확장에 값을 요청하면 호출됩니다.||
|receive|number||아희 코드에서 확장에 값을 전송하면 호출됩니다.||
|send_size||number(정수)|아희 코드에게 값을 몇 개 전송할 수 있는지 반환합니다. 0을 반환하면 스택이나 큐에 데이터가 없는 것처럼 취급됩니다.|
|send_bytes||number(정수)|아희 코드에게 값을 몇 바이트 전송할 수 있는지 반환합니다. 디버깅을 위한 것으로, 아무 값이나 반환해도 무관합니다.|

### 2. extension.json 파일 작성
확장에 대한 정보를 저장합니다. 반드시 필요한 프로퍼티는 다음과 같습니다.

이름|타입|가능한 값|설명
:-:|:-:|:-:|:-
name|string||확장의 이름을 저장합니다.|
target|string|Aheui|어느 언어를 위한 확장인지 저장합니다.|
type|string|Lua|어느 언어로 개발된 확장인지 저장합니다.|
source|string||어느 파일이 스크립트 파일인지 저장합니다. 이름과 확장자만 저장해야 합니다.|

반드시 필요하지 않은 프로퍼티는 다음과 같습니다.

이름|타입|설명|
:-:|:-:|:-:|:-|
developer|string|확장의 개발자를 저장합니다.|
|description|string|확장의 설명을 저장합니다.|

### 3. 파일 압축
*1*.에서 작성한 스크립트와 *2*.에서 작성한 extension.json 파일을 하나의 파일로 압축합니다.

## 외부 라이브러리
- [JSON for Modern C++](https://github.com/nlohmann/json) version 3.6.1
- [Lua](https://www.lua.org) 5.3.5
- [LuaSocket](https://github.com/diegonehab/luasocket) 3.0
- [miniz](https://github.com/richgel999/miniz) 2.1.0
- [UTF8-CPP](https://github.com/nemtrif/utfcpp) 3.1
- [u5e](https://github.com/ruoso/u5e) commit 3b970d5