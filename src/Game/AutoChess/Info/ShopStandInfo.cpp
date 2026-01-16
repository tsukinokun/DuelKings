//---------------------------------------------------------------------------
//!	@file	ShopStandInfo.cpp
//! @brief	ショップスタンドの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ShopStandInfo.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/PiecePool.h>
#include <Game/AutoChess/Agent.h>
//---------------------------------------------------------------------------
//!  オーナーのを設定
//---------------------------------------------------------------------------
void ShopStandInfo::SetOwner(const std::weak_ptr<Agent>& owner_agent)
{
    owner_agent_ = owner_agent;
}

//---------------------------------------------------------------------------
//!  購入可能ピースをリロールする関数
//---------------------------------------------------------------------------
void ShopStandInfo::RerollShopPieces()
{
    if(is_locked_) {
        return;    // ショップがロックされている場合、リロールしない
    }
    //仮でピースの基底クラスを入れておく
    for(int i = 0; i < shop_pieces_.size(); i++) {
        //現在のピースをプールに戻す
        PiecePool::ReturnPieceToPool(shop_pieces_[i].GetTypeName(), shop_pieces_[i].GetLevel());
        if(auto owner_agent = owner_agent_.lock()) {
            shop_pieces_[i] = PiecePool::GetRandomPiece(owner_agent->GetAgentLevel());    //ピース情報を更新
        }
    }
    return;
}

//---------------------------------------------------------------------------
//! ショップに並んでいるピースを取得する関数
//---------------------------------------------------------------------------
std::array<PieceInfo, 5> ShopStandInfo::GetShopPieces()
{
    return shop_pieces_;
}

//--------------------------------------------------------------------------
//! 指定インデックスのピースを購入済みとして無効化する関数
//--------------------------------------------------------------------------
bool ShopStandInfo::InvalidateShopPiece(size_t index)
{
    if(index >= shop_pieces_.size()) {
        return false;    // 範囲外なら失敗
    }
    shop_pieces_[index] = PieceInfo();    // 空のピース情報を代入
    return true;
}

//--------------------------------------------------------------------------
//!  ショップのロック状態を設定する関数
//--------------------------------------------------------------------------
void ShopStandInfo::SetLockState(bool is_locked)
{
    is_locked_ = is_locked;
}

//--------------------------------------------------------------------------
//! ショップがロックされているかどうかを取得する関数
//--------------------------------------------------------------------------
bool ShopStandInfo::IsLocked() const
{
    return is_locked_;
}
