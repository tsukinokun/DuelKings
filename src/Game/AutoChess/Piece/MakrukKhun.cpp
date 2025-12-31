//---------------------------------------------------------------------------
//!	@file	MakrukKhun.cpp
//! @brief	マークルックのクンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukKhun.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/MakrukKhunSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukKhun::Init()
{
    __super::Init();
    SetName("MakrukKhun");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/MakrukKhun.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントを追加
    //---------------------------------------------------------------------------------
    AddComponent<MakrukKhunSkill>();
    return true;
}
