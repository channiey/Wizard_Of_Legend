#include "stdafx.h"
#include "Field_Scene.h"
#include "ObjMgr.h"
#include "EventMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Component.h"
#include "Transform.h"
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
#include "Field_Trigger_1.h"
#include "Field_Trigger_2.h"
#include "Field_Trigger_3.h"

#include "SoundMgr.h"
#include "Portal.h"
#include "Collider.h"

CField_Scene::CField_Scene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
	, m_pTexBack(nullptr)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
{
}

CField_Scene::~CField_Scene()
{
}

const bool CField_Scene::Init()
{
	return true;
}

void CField_Scene::Update()
{
	CScene::Update();
}

void CField_Scene::LateUpdate()
{
	CScene::LateUpdate();
}

void CField_Scene::Render(HDC _hDC)
{
	Render_Background(_hDC);

	CScene::Render(_hDC);
}

void CField_Scene::Enter()
{
	// 개발자 모드 설정
	CGameMgr::Get_Inst()->Set_DevMode(true);

	// C:\Users\ChanYoon\Desktop\WizardOfLegend_6\Data\TileMap\01.Field
	// 배경 맵 로드
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Data/TileMap/01.Field/Map_Field_2.bmp",	L"Map_Field_2");

	// 타일 데이터 로드
	CMapMgr::Get_Inst()->Load_Tile(L"../Data/TileMap/01.Field/Map_Field_2_Tile.dat");

	// 플레이어 생성 및 타겟 설정
	CObj* pPlayer = new CPlayer(OBJ_TYPE::PLAYER);
	pPlayer->Init();
	pPlayer->Get_Transform()->Set_Pos(m_vStartPos);
	CEventMgr::Get_Inst()->Add_Obj(pPlayer);

	CCameraMgr::Get_Inst()->Set_Target(pPlayer);

	// 스폰 트리거 생성
	{
		CObj* pTrigger = new CField_Trigger_1(OBJ_TYPE::TRIGGER);
		pTrigger->Init();
		CEventMgr::Get_Inst()->Add_Obj(pTrigger);

		pTrigger = new CField_Trigger_2(OBJ_TYPE::TRIGGER);
		pTrigger->Init();
		CEventMgr::Get_Inst()->Add_Obj(pTrigger);

		pTrigger = new CField_Trigger_3(OBJ_TYPE::TRIGGER);
		pTrigger->Init();
		CEventMgr::Get_Inst()->Add_Obj(pTrigger);
	}
	

	// 포탈 설정
	CObj* pPortal = new CPortal(OBJ_TYPE::PORTAL);
	pPortal->Init();
	pPortal->Get_Transform()->Set_Pos(START_POS_FIELD);
	pPortal->Get_Collider()->Set_Pos(START_POS_FIELD);
	CEventMgr::Get_Inst()->Add_Obj(pPortal);
	static_cast<CPortal*>(pPortal)->Effect(START_POS_FIELD);


	// 충돌 설정 (Enum 값 기준 작은 것을 left로 호출)
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PLAYER, OBJ_TYPE::TILE);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PLAYER, OBJ_TYPE::TRIGGER);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PLAYER, OBJ_TYPE::PORTAL);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PLAYER, OBJ_TYPE::ITEM);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PROJECTILE, OBJ_TYPE::TILE);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PROJECTILE, OBJ_TYPE::PLAYER);
	CCollisionMgr::Get_Inst()->Check_Group(OBJ_TYPE::PROJECTILE, OBJ_TYPE::ENEMY);


	// UI Set
	CObj* pStatUI = new CStatUI(OBJ_TYPE::UI, UI_TYPE::FIXED, pPlayer);
	pStatUI->Init();
	CEventMgr::Get_Inst()->Add_Obj(pStatUI);

	CObj* pSkillUI = new CSkillUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	pSkillUI->Init();
	CEventMgr::Get_Inst()->Add_Obj(pSkillUI);

	CObj* pGoldUI = new CGoldUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	pGoldUI->Init();
	CEventMgr::Get_Inst()->Add_Obj(pGoldUI);

	CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Inst()->PlayBGM(L"Air.wav", 0.4f);
}

void CField_Scene::Exit()
{
	CCollisionMgr::Get_Inst()->Reset();
}

void CField_Scene::Render_Background(HDC _hDC)
{
	VEC2 vScale = VEC2(6515, 4748);

	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(VEC2(vScale.fX * 0.5f, vScale.fY * 0.5f));
	
	BitBlt(_hDC, int(vPos.fX - (vScale.fX * 0.5f)), int(vPos.fY - (vScale.fY * 0.5f)), int(vScale.fX), int(vScale.fY), CBmpMgr::Get_Inst()->Find_Image(L"Map_Field_2"), 0, 0, SRCCOPY);
}
