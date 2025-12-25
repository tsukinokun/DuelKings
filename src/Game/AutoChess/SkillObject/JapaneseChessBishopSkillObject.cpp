//---------------------------------------------------------------------------
//!	@file	JapaneseChessBishopSkillObject.cpp
//! @brief	チェスビショップのスキルオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessBishopSkillObject.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessBishopSkillObject::Init()
{
    __super::Init();

    SetName(u8"闇の儀式");
    SetEffect(u8"data/AutoChess/Effect/ChessBishopSkill.efkefc");
    SetEffectPlaySpeed(0.05f);

    return true;
}
