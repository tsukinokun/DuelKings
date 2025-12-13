//---------------------------------------------------------------------------
//!	@file	JapaneseChessKnight.cpp
//! @brief	将棋の桂馬クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessKnight.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessKnight::Init()
{
    __super::Init();
    SetName("JapaneseChessKnight");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/JapaneseChessKnight.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));

    return true;
}
