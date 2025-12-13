//---------------------------------------------------------------------------
//!	@file	JapaneseChessBishop.cpp
//! @brief	将棋の角行クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessBishop.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessBishop::Init()
{
    __super::Init();
    SetName("JapaneseChessBishop");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/JapaneseChessBishop.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));

    return true;
}
