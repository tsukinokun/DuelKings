//---------------------------------------------------------------------------
//!	@file	ShopStand.cpp
//! @brief	インゲームシーンの購入可能ピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ShopStand.h"
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/PiecePool.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ShopStand::Init()
{
    __super::Init();
    RerollShopPieces();    //購入可能ピースをロール
    return true;
}
//---------------------------------------------------------------------------------
//!	オーナーを設定
//---------------------------------------------------------------------------------
void ShopStand::SetOwner(std::weak_ptr<Object> owner)
{
    owner_ = owner;
}
//---------------------------------------------------------------------------------
//! 購入可能ピースをリロールする関数
//---------------------------------------------------------------------------------
void ShopStand::RerollShopPieces()
{
    //仮でピースの基底クラスを入れておく
    for(int i = 0; i < shop_pieces_.size(); i++) {
        //既にピースがある場合は消す
        if(auto piece = shop_pieces_[i].lock()) {
            Scene::Object::Release(piece);
        }
        auto shop_piece = PiecePool::GetRandomPiece();             //ピースを生成
        shop_piece->SetTranslate(float3(0.0f, 1.0f, i * 1.0f));    //位置を初期化
        shop_pieces_[i] = shop_piece;
    }
}
//---------------------------------------------------------------------------------
//! ショップに並んでいるピースを取得する関数
//---------------------------------------------------------------------------------
std::array<std::weak_ptr<Piece>, 5> ShopStand::GetShopPieces()
{
    return shop_pieces_;
}
//--------------------------------------------------------------------------
//! 指定インデックスのピースを購入済みとして無効化する関数
//--------------------------------------------------------------------------
bool ShopStand::InvalidateShopPiece(size_t index)
{
    if(index >= shop_pieces_.size()) {
        return false;    // 範囲外なら失敗
    }
    shop_pieces_[index] = std::weak_ptr<Piece>();    // 空の weak_ptr を代入
    return true;
}
