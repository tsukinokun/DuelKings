//---------------------------------------------------------------------------
//!	@file	JapaneseChessGold.cpp
//! @brief	将棋の金将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessGold.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessGold::Init()
{
    __super::Init();
    SetName("JapaneseChessGold");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/JapaneseChessGold.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
