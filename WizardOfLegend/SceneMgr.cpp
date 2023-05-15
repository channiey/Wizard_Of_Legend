#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "VillageScene.h"
#include "Field_Scene.h"
#include "Boss_Scene.h"
#include "ResultScene.h"
#include "EditorScene.h"

CSceneMgr* CSceneMgr::m_pInst = nullptr;

CSceneMgr::CSceneMgr()
{
	for (int i = 0; i < (UINT)SCENE_TYPE::TYPEEND; ++i)
		m_arrScene[i] = nullptr;
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

const bool CSceneMgr::Init()
{
	m_arrScene[(UINT)SCENE_TYPE::TITLE]				= new CTitleScene	(SCENE_TYPE::TITLE		, VEC2_SCREEN_CENTER);
	m_arrScene[(UINT)SCENE_TYPE::TITLE]->Init();

	m_arrScene[(UINT)SCENE_TYPE::LOADING]			= new CLoadingScene	(SCENE_TYPE::LOADING	, VEC2_SCREEN_CENTER);
	m_arrScene[(UINT)SCENE_TYPE::LOADING]->Init();

	m_arrScene[(UINT)SCENE_TYPE::VILLAGE]			= new CVillageScene	(SCENE_TYPE::VILLAGE	, START_POS_VILLAGE);
	m_arrScene[(UINT)SCENE_TYPE::VILLAGE]->Init();

	m_arrScene[(UINT)SCENE_TYPE::FIELD]				= new CField_Scene	(SCENE_TYPE::FIELD		, START_POS_FIELD);
	m_arrScene[(UINT)SCENE_TYPE::FIELD]->Init();

	m_arrScene[(UINT)SCENE_TYPE::BOSS]				= new CBoss_Scene	(SCENE_TYPE::BOSS		, START_POS_BOSS);
	m_arrScene[(UINT)SCENE_TYPE::BOSS]->Init();

	m_arrScene[(UINT)SCENE_TYPE::RESULT]			= new CResultScene	(SCENE_TYPE::RESULT		, VEC2_SCREEN_CENTER);
	m_arrScene[(UINT)SCENE_TYPE::RESULT]->Init();

	m_arrScene[(UINT)SCENE_TYPE::EDITOR]			= new CEditorScene	(SCENE_TYPE::EDITOR		, VEC2_SCREEN_CENTER);
	m_arrScene[(UINT)SCENE_TYPE::EDITOR]->Init();
	
	
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::TITLE];
	//m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::BOSS];
	m_pCurScene->Init();
	m_pCurScene->Enter();

	return true;
}

void CSceneMgr::Update()
{
	if (nullptr == m_pCurScene) return;

	m_pCurScene->Update();
}

void CSceneMgr::LateUpdate()
{
	if (nullptr == m_pCurScene) return;

	m_pCurScene->LateUpdate();
}

void CSceneMgr::Render(HDC _hDC)
{
	if (nullptr == m_pCurScene) return;

	m_pCurScene->Render(_hDC);
}

void CSceneMgr::Release()
{
	for (int i = 0; i < (UINT)SCENE_TYPE::TYPEEND; ++i)
		Safe_Delete<CScene*>(m_arrScene[i]);
}

const bool CSceneMgr::Change_Scene(const SCENE_TYPE& _eType)
{
	if (nullptr == m_pCurScene) return false;

	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eType];
	m_pCurScene->Enter();

	return true;
}
