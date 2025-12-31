//---------------------------------------------------------------------------
//!	@file	MakrukKhon.cpp
//! @brief	マークルックのコーンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukKhon.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/MakrukKhonSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukKhon::Init()
{
    __super::Init();
    SetName("MakrukKhon");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/MakrukKhon.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントを追加
    //---------------------------------------------------------------------------------
    AddComponent<MakrukKhonSkill>();
    return true;
}
