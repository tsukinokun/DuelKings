//---------------------------------------------------------------------------
//!	@file	MakrukKhun.cpp
//! @brief	マークルックのクンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukKhun.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukKhun::Init()
{
    __super::Init();
    SetName("MakrukKhun");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/MakrukKhun.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
