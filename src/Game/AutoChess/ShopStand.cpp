//---------------------------------------------------------------------------
//!	@file	ShopStand.cpp
//! @brief	インゲームシーンの購入可能ピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ShopStand.h"
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/PiecePool.h>
#include <Game/AutoChess/PieceFactory.h>
#include <Game/AutoChess/Agent.h>
#include <Game/AutoChess/Info/ShopStandInfo.h>
#include <Game/AutoChess/Info/PieceInfo.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ShopStand::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //!	更新処理を設定
    //---------------------------------------------------------------------------------
    auto update = [this]() {
        //オーナーをロック
        if(auto owner = owner_.lock()) {
            //ショップ情報を取得する
            auto shop_pieces_info = owner->GetShopPieces();
            //ループで中身を見ていく
            for(int i = 0; i < shop_pieces_info.size(); i++) {
                //情報があれば
                if(shop_pieces_info[i].GetTypeName() != "") {
                    //ピースがあれば
                    if(auto shop_piece = shop_pieces_[i].lock()) {
                        //名前を確認
                        if(shop_pieces_info[i].GetTypeName() != shop_piece->GetNameDefault()) {
                            //違ったら削除して生成
                            Scene::Object::Release(shop_piece);
                            shop_pieces_[i] = std::weak_ptr<Piece>();                                                 // 空の weak_ptr を代入
                            auto shop_piece = PieceFactory::CreatePiece(shop_pieces_info[i].GetTypeName().data());    //実体を生成
                            shop_piece->SetTranslate(float3(i * 1.0f, 1.0f, 0.0f));                                   //位置を初期化
                            shop_pieces_[i] = shop_piece;
                        }
                    }
                    else {
                        //なければ生成
                        auto piece = PieceFactory::CreatePiece(shop_pieces_info[i].GetTypeName().data());    //実体を生成
                        piece->SetTranslate(float3(i * 1.0f, 1.0f, 0.0f));                                   //位置を初期化
                        shop_pieces_[i] = piece;
                    }
                }
            }
        }
    };
    SetProc("update", update, ProcTiming::Update, ProcPriority::NONE);
    return true;
}
//---------------------------------------------------------------------------------
//!	オーナーを設定
//---------------------------------------------------------------------------------
void ShopStand::SetOwner(std::weak_ptr<Agent> owner)
{
    owner_ = owner;
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
//--------------------------------------------------------------------------
//! ショップのロック状態を設定する関数
//--------------------------------------------------------------------------
std::shared_ptr<ShopStand> ShopStand::SetLockState(bool is_locked)
{
    is_locked_ = is_locked;
    return dynamic_pointer_cast<ShopStand>(shared_from_this());
}

//--------------------------------------------------------------------------
//! ショップがロックされているかどうかを取得する関数
//--------------------------------------------------------------------------
bool ShopStand::IsLocked() const
{
    return is_locked_;
}
