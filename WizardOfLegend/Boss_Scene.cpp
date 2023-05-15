#include "stdafx.h"
#include "Boss_Scene.h"


#include "stdafx.h"
#include "ObjMgr.h"
#include "EventMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Component.h"
#include "Transform.h"
#include "Animator.h"
#include "CameraMgr.h"
#include "MainGame.h"
#include "GameMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "MapMgr.h"
#include "BmpMgr.h"
#include "MyBmp.h"
#include "CollisionMgr.h"
#include "UI.h"
#include "StatUI.h"
#include "SkillUI.h"
#include "GoldUI.h"

#include "BossSpawnTrigger.h"
#include "BossStageMgr.h"

#include "SoundMgr.h"

CBoss_Scene::CBoss_Scene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
	, m_pTexBack(nullptr)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
{
}

CBoss_Scene::~CBoss_Scene()
{
}

const bool CBoss_Scene::Init()
{
	return true;
}

void CBoss_Scene::Update()
{
	CScene::Update();

	CBossStageMgr::Get_Inst()->Update();
}

void CBoss_Scene::LateUpdate()
{
	CScene::LateUpdate();

	CBossStageMgr::Get_Inst()->LateUpdate();
}

void CBoss_Scene::Render(HDC _hDC)
{
	Render_Background(_hDC);

	CScene::Render(_hDC);
}

void CBoss_Scene::Enter()
{
	// 개발자 모드 설정
	CGameMgr::Get_Inst()->Set_DevMode(true);

	// 배경 맵 로드
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Data/TileMap/02.Boss/Boss_Stage_Large.bmp", L"Map_Boss");

	// 타일 데이터 로드
	CMapMgr::Get_Inst()->Load_Tile(L"../Data/TileMap/02.Boss/Boss_Stage_Large_Tile.dat");

	//// 플레이어 생성 및 타겟 설정
	//CObj* pPlayer = new CPlayer(OBJ_TYPE::PLAYER);
	//pPlayer->Init();
	//pPlayer->Get_Transform()->Set_Pos(m_vStartPos);
	//CEventMgr::Get_Inst()->Add_Obj(pPlayer);

	//CCameraMgr::Get_Inst()->Set_Target(pPlayer);


	CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();
	pPlayer->Get_Transform()->Set_Pos(START_POS_BOSS);
	
	// 충돌 설정 (Enum 값 기준 작은 것을 left로 호출)
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PLAYER, OBJ_TYPE::TILE);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PROJECTILE, OBJ_TYPE::TILE);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PROJECTILE, OBJ_TYPE::PLAYER);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PROJECTILE, OBJ_TYPE::ENEMY);

	//// UI Set
	//CObj* pStatUI = new CStatUI(OBJ_TYPE::UI, UI_TYPE::FIXED, pPlayer);
	//pStatUI->Init();
	//CEventMgr::Get_Inst()->Add_Obj(pStatUI);

	//CObj* pSkillUI = new CSkillUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	//pSkillUI->Init();
	//CEventMgr::Get_Inst()->Add_Obj(pSkillUI);

	//CObj* pGoldUI = new CGoldUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	//pGoldUI->Init();
	//CEventMgr::Get_Inst()->Add_Obj(pGoldUI);

	CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Inst()->PlayBGM(L"Ice.wav", 1.f);
}

void CBoss_Scene::Exit()
{
	CCollisionMgr::Get_Inst()->Reset();
}

void CBoss_Scene::Render_Background(HDC _hDC)
{
	// 2197 1737

	VEC2 vScale = VEC2(2197, 2197);

	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(VEC2(vScale.fX * 0.5f, vScale.fY * 0.5f));

	BitBlt(_hDC, int(vPos.fX - (vScale.fX * 0.5f)), int(vPos.fY - (vScale.fY * 0.5f)), int(vScale.fX), int(vScale.fY), CBmpMgr::Get_Inst()->Find_Image(L"Map_Boss"), 0, 0, SRCCOPY);

}
