# 확장
## 확장 개발 방법
1. Lua 스크립트를 작성합니다. 확장 개발에서 가장 중요하고 난이도 있는 단계입니다.
    - 아래의 Lua API 절을 참조하세요.
2. 확장에 대한 정보를 담는 Extension.json 파일을 작성합니다.
    - 아래의 Extension.json 프로퍼티 절을 참조하세요.
3. 위에서 작성한 두 파일을 하나의 파일로 압축합니다.

## Lua API
### 타입
- [Int128](https://github.com/kmc7468/talkheui/blob/master/documents/확장/Lua%20API/Int128.md)

### 함수
|이름|매개 변수|반환|설명|
|:-:|:-:|:-:|:-|
|get_talkheui_version||string|확장이 실행되고 있는 톡희의 버전을 반환합니다. [유의적 버전](https://semver.org/lang/ko)의 규격을 준수합니다.|
|is_int128_available||boolean|확장이 실행되고 있는 톡희가 128비트 정수 연산을 지원하는지 반환합니다.|

### 이벤트 함수
이벤트 함수란 아희 코드에서 특정한 상황때 호출하는 함수로, 여러분들이 구현해야 하는 함수입니다.
|이름|매개 변수|반환|설명|필수 구현|
|:-:|:-:|:-:|:-|:-:|
|send||number(정수) 또는 Int128|아희 코드에서 확장에 값을 요청하면 호출됩니다.|Y|
|receive|number(정수) 또는 Int128||아희 코드에서 확장에 값을 전송하면 호출됩니다.|Y|
|send_count||number(정수) 또는 Int128|아희 코드에게 값을 몇 개 전송할 수 있는지 반환합니다. 0을 반환하면 스택이나 큐에 데이터가 없는 것처럼 취급됩니다.|Y|
|send_bytes||number(정수) 또는 Int128|아희 코드에게 값을 몇 바이트 전송할 수 있는지 반환합니다.|N|

## Extension.json 프로퍼티
|이름|타입|가능한 값|설명|필수 작성|
|:-:|:-:|:-:|:-|:-:|
|Name|string||확장의 이름을 저장합니다.|Y|
|Target|string|Aheui|어느 언어를 위한 확장인지 저장합니다.|Y|
|Type|string|Lua|어느 언어로 개발된 확장인지 저장합니다.|Y|
|Source|string||어느 파일이 스크립트 파일인지 저장합니다. 이름과 확장자만 저장해야 합니다.|Y|
|Developer|string||확장의 개발자를 저장합니다.|N|
|Description|string||확장의 설명을 저장합니다.|N|