//---------------------------------------------------------------------------
//!	@file	ComponentSkill.h
//! @brief	スキルのベースコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>

USING_PTR(ComponentSkill);

class ComponentSkill : public Component
{
public:
    BP_COMPONENT_DECL(ComponentSkill, u8"スキルのベースコンポーネントクラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------
    //! @brief スキル発動処理
    //---------------------------------------------------------
    virtual void Activate() = 0;

private:
};

CEREAL_REGISTER_TYPE(ComponentSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentSkill)
