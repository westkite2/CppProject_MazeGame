# 미로 게임 (Maze Game)
## 프로젝트 개요
* 2021년 1학기에 서강대학교 컴퓨터공학설계및실험1 강의에서 openFrameworks를 이용한 게임을 개발하는 과제로 작성하였다.
* 미로에서 적을 피해 골인 지점까지 도달하는 게임이다.
* C++, openFrameworks를 이용하였다.
## 프로젝트 내용
### 게임 규칙
* 목표: 적을 피해 미로 통과하기
* 성공 조건: 미로에 놓인 모든 아이템을 획득한 뒤 골인 지점에 도달
* 실패 조건: 적과 충돌
### 조작 방법
* 방향 키: 한 칸씩 이동
* W, S, A, D: 폭탄 사용 (플레이어를 둘러싼 벽 중 해당하는 방향의 벽 한 칸을 제거하여 길 생성)
* Space bar: 문 입장 (플레이어가 있는 곳의 문과 같은 번호가 적힌 문의 위치로 이동)
* Q: 게임 종료
### 구성 요소
* 플레이어: 하얀 원
* 적: 빨간 원
* 아이템: 노란 삼각형
* 폭탄: 파란 별 (획득 시 하단에 표시됨)
* 문: 회색 사각형 (같은 숫자가 적힌 문이 한 쌍씩 존재)
### 알고리즘 요약
* 게임을 플레이할 때마다 미로의 형태 및 구성요소(적, 아이템, 폭탄, 문)가 랜덤 위치에 생성됨
* 미로 생성: 이진 트리 알고리즘을 사용하여 생성 시간이 짧고 공간 효율이 좋음
* 적 이동: 현재 위치에서 이동 가능한 방향이 1개이면 해당 방향, 2개이면 현재의 반대 방향, 3개 이상이면 되돌아가는 길이 아닌 것 중 임의로 선택
* 문 생성: 미로의 왼쪽과 오른쪽에 절반씩 생성
### 스크린샷
[시작 화면]

![스크린샷1](https://github.com/westkite2/CppProject_MazeGame/blob/main/screenshot_1.png)

[문 기능]

![스크린샷2](https://github.com/westkite2/CppProject_MazeGame/blob/main/screenshot_2.png)

[폭탄 기능]

![스크린샷3](https://github.com/westkite2/CppProject_MazeGame/blob/main/screenshot_3.png)

[엔딩 화면]

![스크린샷4](https://github.com/westkite2/CppProject_MazeGame/blob/main/screenshot_4.png)
