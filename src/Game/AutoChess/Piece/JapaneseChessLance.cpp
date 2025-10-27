//---------------------------------------------------------------------------
//!	@file	JapaneseChessLance.cpp
//! @brief	将棋の王将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessLance.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessLance::Init()
{
    __super::Init();
    SetName("JapaneseChessLance");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/JapaneseChessLance.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));

    return true;
}
