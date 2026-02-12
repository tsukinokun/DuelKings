//---------------------------------------------------------------------------
//!	@file	ShopPieceEffectManager.h
//! @brief	ショップエフェクト管理クラスの定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
USING_PTR(ShopPieceEffectManager);
class ShopPieceEffectManager : public Component
{
public:
    BP_COMPONENT_DECL(ShopPieceEffectManager, u8"ショップエフェクト管理クラス");

    virtual void Init() override;    //!< 初期化
};

CEREAL_REGISTER_TYPE(ShopPieceEffectManager)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ShopPieceEffectManager)
