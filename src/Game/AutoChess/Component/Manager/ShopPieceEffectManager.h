//---------------------------------------------------------------------------
//!	@file	ShopPieceEffectManager.h
//! @brief	ショップエフェクト管理クラスの定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
USING_PTR(ShopPieceEffectManager);
class Piece;    //前方宣言
class ShopPieceEffectManager : public Component
{
public:
    BP_COMPONENT_DECL(ShopPieceEffectManager, u8"ショップエフェクト管理クラス");

    virtual void Init() override;    //!< 初期化
};

CEREAL_REGISTER_TYPE(ShopPieceEffectManager)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ShopPieceEffectManager)
