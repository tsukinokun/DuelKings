//---------------------------------------------------------------------------
//!	@file	PieceSkillUser.h
//! @brief	ピースのスキル使用機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>

USING_PTR(PieceSkillUser);

class PieceSkillUser : public Component
{
public:
    BP_COMPONENT_DECL(PieceSkillUser, u8"ピースのスキル使用機能クラス");

    virtual void Init() override;    //!< 初期化
private:
};

CEREAL_REGISTER_TYPE(PieceSkillUser)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceSkillUser)
