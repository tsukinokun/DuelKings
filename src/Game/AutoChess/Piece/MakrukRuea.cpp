//---------------------------------------------------------------------------
//!	@file	MakrukRuea.cpp
//! @brief	マークルックのルアクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukRuea.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukRuea::Init()
{
    __super::Init();
    SetName("MakrukRuea");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/MakrukRuea.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
