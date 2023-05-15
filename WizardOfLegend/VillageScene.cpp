#include "stdafx.h"
#include "VillageScene.h"


CVillageScene::CVillageScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
{
}


CVillageScene::~CVillageScene()
{
}

const bool CVillageScene::Init()
{
	return false;
}

void CVillageScene::Update()
{
}

void CVillageScene::LateUpdate()
{
}

void CVillageScene::Render(HDC _hDC)
{
}

void CVillageScene::Enter()
{
}

void CVillageScene::Exit()
{
}
