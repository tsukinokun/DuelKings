//---------------------------------------------------------------------------
//!	@file	ChessPawn.cpp
//! @brief	チェスのポーンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessPawn.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChessPawnSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessPawn::Init()
{
    __super::Init();
    SetName("ChessPawn");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChessPawn.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントの追加
    //---------------------------------------------------------------------------------
    AddComponent<ChessPawnSkill>();
    return true;
}
