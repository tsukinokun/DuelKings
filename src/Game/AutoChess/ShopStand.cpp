//---------------------------------------------------------------------------
//!	@file	ShopStand.cpp
//! @brief	インゲームシーンの購入可能ピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ShopStand.h"
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Piece/JapaneseChessPawn.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ShopStand::Init()
{
    __super::Init();
    ReloadShopPieces();    //購入可能ピースをリロード
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
//! 購入可能ピースをリロードする関数
//---------------------------------------------------------------------------------
void ShopStand::ReloadShopPieces()
{
    //仮でピースの基底クラスを入れておく
    for(int i = 0; i < shop_pieces_.size(); i++) {
        //既にピースがある場合は消す
        if(auto piece = shop_pieces_[i].lock()) {
            Scene::Object::Release(piece);
        }
        auto shop_piece = Scene::Object::Create<JapaneseChessPawn>();    //ピースを生成
        shop_piece->SetTranslate(float3(0.0f, 1.0f, i * 1.0f));          //位置を初期化
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
