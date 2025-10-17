//---------------------------------------------------------------------------
//!	@file	ShopStand.h
//! @brief	インゲームシーンの購入可能ピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class Piece;    //前方宣言
USING_PTR(PieceStand);
class ShopStand : public Object
{
public:
    BP_OBJECT_DECL(ShopStand, u8"インゲームシーンの購入可能ピース置き場")
    bool Init() override;    //!< 初期化

    //オーナーの名前を設定
    //! @param owner [in] オーナー
    void SetOwner(std::weak_ptr<Object> owner);

    //購入可能ピースをリロードする関数
    void ReloadShopPieces();

private:
    std::array<std::weak_ptr<Piece>, 5> shop_pieces_;    //購入可能ピースを格納した配列
    std::weak_ptr<Object>               owner_;          //オーナーのウィークポインタ
};
