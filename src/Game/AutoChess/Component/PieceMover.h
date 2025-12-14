//---------------------------------------------------------------------------
//!	@file	PieceMover.h
//! @brief	ピースの移動機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/MoveStrategy/IMoveStrategy.h>    //戦略の寿命を管理するために必要

USING_PTR(PieceMover);
class PieceMover : public Component
{
public:
    BP_COMPONENT_DECL(PieceMover, u8"ピースの移動機能クラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // ピースの移動処理を設定する関数
    //! @param strategy [in] 移動ストラテジークラス
    //---------------------------------------------------------------------------
    void SetMoveStrategy(std::unique_ptr<IMoveStrategy> strategy);

private:
    std::unique_ptr<IMoveStrategy> move_strategy_;    // 移動ストラテジークラス
};

CEREAL_REGISTER_TYPE(PieceMover)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceMover)
