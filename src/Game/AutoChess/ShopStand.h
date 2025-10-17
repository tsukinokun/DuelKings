//---------------------------------------------------------------------------
//!	@file	ShopStand.h
//! @brief	インゲームシーンの購入可能ピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
USING_PTR(PieceStand);
class ShopStand : public Object
{
public:
    BP_OBJECT_DECL(PieceStand, u8"インゲームシーンの購入可能ピース置き場")
    bool Init() override;    //!< 初期化

    //オーナーの名前を設定
    //! @param owner [in] オーナー
    void SetOwner(std::weak_ptr<Object> owner);

private:
    std::weak_ptr<Object> owner_;    //オーナーのウィークポインタ
};
