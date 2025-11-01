//---------------------------------------------------------------------------
//!	@file	MakrukBia.cpp
//! @brief	マークルックのビアクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukBia.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukBia::Init()
{
    __super::Init();
    SetName("MakrukBia");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/MakrukBia.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
