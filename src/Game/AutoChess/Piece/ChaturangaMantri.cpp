//---------------------------------------------------------------------------
//!	@file	ChaturangaMantri.cpp
//! @brief	チャトランガのマントリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaMantri.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChaturangaMantriSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaMantri::Init()
{
    __super::Init();
    SetName("ChaturangaMantri");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaMantri.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントの追加
    //---------------------------------------------------------------------------------
    AddComponent<ChaturangaMantriSkill>();
    return true;
}
