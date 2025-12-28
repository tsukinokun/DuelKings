//---------------------------------------------------------------------------
//!	@file	ChessRook.cpp
//! @brief	チェスのルーククラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessRook.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChessRookSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessRook::Init()
{
    __super::Init();
    SetName("ChessRook");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChessRook.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントの追加
    //---------------------------------------------------------------------------------
    AddComponent<ChessRookSkill>();
    return true;
}
