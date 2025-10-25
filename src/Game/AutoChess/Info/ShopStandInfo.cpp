//---------------------------------------------------------------------------
//!	@file	ShopStandInfo.cpp
//! @brief	ショップスタンドの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ShopStandInfo.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/PiecePool.h>
//---------------------------------------------------------------------------
//  オーナーのを設定
//! @param owner [in] オーナー
//! @retval オーナーエージェントの共有ポインタ
//---------------------------------------------------------------------------
std::shared_ptr<Agent> ShopStandInfo::SetOwner(std::weak_ptr<Agent> owner_agent)
{
    owner_agent_ = owner_agent;
    return owner_agent.lock();
}

//---------------------------------------------------------------------------
//  購入可能ピースをリロールする関数
//! @retval 自身のポインタ
//! @note ショップがロックされていない場合にのみ実行されます。
//---------------------------------------------------------------------------
std::shared_ptr<ShopStandInfo> ShopStandInfo::RerollShopPieces()
{
    if(is_locked_) {
        return shared_from_this();    // ショップがロックされている場合、リロールしない
    }
    //仮でピースの基底クラスを入れておく
    for(int i = 0; i < shop_pieces_.size(); i++) {
        //既にピースがある場合はポインタを無効にする
        if(auto piece = shop_pieces_[i].lock()) {
            shop_pieces_[i].reset();
        }
        auto shop_piece = PiecePool::GetRandomPiece();    //ピースを生成
        //shop_piece->SetTranslate(float3(0.0f, 1.0f, i * 1.0f));    //位置を初期化
        //shop_pieces_[i] = shop_piece;
    }
    return shared_from_this();
}

//---------------------------------------------------------------------------
//ショップに並んでいるピースを取得する関数
//! @retval ショップに並んでいるピース
//---------------------------------------------------------------------------
std::array<std::weak_ptr<PieceInfo>, 5> ShopStandInfo::GetShopPieces()
{
    return shop_pieces_;
}

//--------------------------------------------------------------------------
// 指定インデックスのピースを購入済みとして無効化する関数
//! @param index [in] 無効化するピースのインデックス（0〜4）
//! @return 成功した場合 true、不正なインデックスなら false
//--------------------------------------------------------------------------
bool ShopStandInfo::InvalidateShopPiece(size_t index)
{
    if(index >= shop_pieces_.size()) {
        return false;    // 範囲外なら失敗
    }
    shop_pieces_[index] = std::weak_ptr<PieceInfo>();    // 空の weak_ptr を代入
    return true;
}

//--------------------------------------------------------------------------
//  ショップのロック状態を設定する関数
//! @param is_locked [in] ロック状態（ロックする場合 true、解除する場合 false）
//! @retval 自身のポインタ
//--------------------------------------------------------------------------
std::shared_ptr<ShopStandInfo> ShopStandInfo::SetLockState(bool is_locked)
{
    is_locked_ = is_locked;
    return shared_from_this();
}

//--------------------------------------------------------------------------
// ショップがロックされているかどうかを取得する関数
//! @retval ロックされている場合 true、されていない場合 false
//--------------------------------------------------------------------------
bool ShopStandInfo::IsLocked() const
{
    return is_locked_;
}
