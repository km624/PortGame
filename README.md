# Sub Warrior

서브컬쳐 캐릭터를 활용한 **무쌍 액션 게임**입니다. 첫 언리얼 포트폴리오로 제작했습니다.
플레이어가 다수의 적을 상대하고, 호위 캐릭터(BodyGuard)를 배치해 함께 전투합니다.

- **기간** 2024.11 ~ 2025.02
- **엔진 / 언어** Unreal Engine · C++
- **역할** 개인 프로젝트 (기획 · 프로그래밍 전담)

---

## 직접 작성한 코드

`Source/PortGame/` 아래가 전부 직접 작성한 C++ 코드입니다. (헤더·소스 296개)

```
Source/PortGame/
├── Interface/     인터페이스 20종 — 시스템 간 결합도 분리
├── AI/            Behavior Tree Task · Service · Decorator 직접 구현
├── UI / MainUI/   HUD, 위젯
├── Data/          DataAsset 기반 캐릭터·무기·전투 데이터
├── BodyGuard/     호위 캐릭터 배치와 행동 로직
├── Skill/         스킬 베이스와 파생 스킬
├── Component/     공격 · 스탯 · 타게팅 · 위젯 컴포넌트
├── Character /  Player /  Weapon /  Animation
└── Save/          세이브 게임
```

### 봐주셨으면 하는 부분

**① 인터페이스 기반 설계 — [`Interface/`](Source/PortGame/Interface)**
20개의 인터페이스로 시스템 간 직접 참조를 끊었습니다. 캐릭터·AI·UI가 서로의 구체 클래스를 모른 채 필요한 기능만 주고받습니다.
- [`PlayerAttackInterface.h`](Source/PortGame/Interface/PlayerAttackInterface.h) · [`PGAICharacterInterface.h`](Source/PortGame/Interface/PGAICharacterInterface.h) · [`ObjectPoolingInterface.h`](Source/PortGame/Interface/ObjectPoolingInterface.h)

**② Behavior Tree 노드 직접 구현 — [`AI/`](Source/PortGame/AI)**
언리얼 기본 노드로 부족한 판단을 Task · Service · Decorator로 직접 만들었습니다.
- [`BTTask_FindNextAttackField.h`](Source/PortGame/AI/BTTask_FindNextAttackField.h) — 다음 공격 위치 탐색
- [`BTDecorator_WhoTarget.h`](Source/PortGame/AI/BTDecorator_WhoTarget.h) — 타깃 판정 분기
- [`BTService_AttackRange.h`](Source/PortGame/AI/BTService_AttackRange.h) — 사거리 지속 갱신

**③ 데이터와 로직 분리 — [`Data/`](Source/PortGame/Data)**
캐릭터 스펙, 무기, 콤보, 음성을 DataAsset으로 빼내 코드 수정 없이 조정할 수 있게 했습니다.
- [`PlayerCharacterDataAsset.h`](Source/PortGame/Data/PlayerCharacterDataAsset.h) · [`ComboData.h`](Source/PortGame/Data/ComboData.h) · [`WeaponData.h`](Source/PortGame/Data/WeaponData.h)

**④ 호위 캐릭터 시스템 — [`BodyGuard/`](Source/PortGame/BodyGuard)**
아군을 배치하고 옵션에 따라 행동을 바꾸는 구조입니다. 배치 전 미리보기까지 포함합니다.
- [`BodyGuardBase.h`](Source/PortGame/BodyGuard/BodyGuardBase.h) → [`BodyGuardLogic.h`](Source/PortGame/BodyGuard/BodyGuardLogic.h) 상속 구조

---

## 실행 화면

<!-- 스크린샷 / GIF를 이 아래에 추가하세요 -->

---

## 사용 에셋 및 플러그인

이 프로젝트는 외부 플러그인을 사용합니다. 아래 코드는 제가 작성한 것이 아닙니다.

**필요 에셋** `Content/PortGame/Character/Rapi/Init/SK_nikke.uasset`

**VRM4U** — Unreal Engine에서 VRM 파일을 지원하는 플러그인
저작권자 **Haruyoshi Yamamoto** · [GitHub](https://github.com/ruyo/VRM4U)

**Kawaii Physics** — 물리 기반 애니메이션 라이브러리
저작권자 **pafuhana1213** · [GitHub](https://github.com/pafuhana1213/KawaiiPhysics) · MIT License
