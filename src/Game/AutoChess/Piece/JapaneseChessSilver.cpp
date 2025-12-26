//---------------------------------------------------------------------------
//!	@file	JapaneseChessSilver.cpp
//! @brief	将棋の銀将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessSilver.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessSilverSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessSilver::Init()
{
    __super::Init();
    SetName("JapaneseChessSilver");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/JapaneseChessSilver.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));
    //---------------------------------------------------------------------------------
    // スキルコンポーネントの追加
    //---------------------------------------------------------------------------------
    AddComponent<JapaneseChessSilverSkill>();
    return true;
}
