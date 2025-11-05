//---------------------------------------------------------------------------
//!	@file	ChaturangaMantri.cpp
//! @brief	チャトランガのマントリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaMantri.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaMantri::Init()
{
    __super::Init();
    SetName("ChaturangaMantri");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChaturangaMantri.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
