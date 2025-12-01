//---------------------------------------------------------------------------
//!	@file	ChessBishop.cpp
//! @brief	チェスのビショップクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessBishop.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChessBishopSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessBishop::Init()
{
    __super::Init();
    SetName("ChessBishop");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChessBishop.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // ビショップのスキルコンポーネントをつける
    //---------------------------------------------------------------------------------
    active_skill_ = AddComponent<ChessBishopSkill>();

    return true;
}
