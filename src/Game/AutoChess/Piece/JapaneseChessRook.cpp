//---------------------------------------------------------------------------
//!	@file	JapaneseChessRook.cpp
//! @brief	将棋の飛車クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessRook.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessRook::Init()
{
    __super::Init();
    SetName("JapaneseChessRook");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/JapaneseChessRook.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));
    return true;
}
