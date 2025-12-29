//---------------------------------------------------------------------------
//!	@file	ChaturangaPadati.cpp
//! @brief	チャトランガのパダチクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaPadati.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChaturangaPadatiSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaPadati::Init()
{
    __super::Init();
    SetName("ChaturangaPadati");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaPadati.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントを追加
    //---------------------------------------------------------------------------------
    AddComponent<ChaturangaPadatiSkill>();
    return true;
}
