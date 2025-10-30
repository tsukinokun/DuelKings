//---------------------------------------------------------------------------
//!	@file	PieceSensor.h
//! @brief	ピースの敵探索機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
class Piece;    //前方宣言
USING_PTR(PieceSensor);

class PieceSensor : public Component
{
public:
    BP_COMPONENT_DECL(PieceSensor, u8"ピースの敵探索機能クラス");

    virtual void Init() override;    //!< 初期化

    //----------------------------------------------------------
    // 最も近い敵の駒を取得
    //! @retval 最も近い敵の駒の共有ポインタ
    //----------------------------------------------------------
    std::shared_ptr<Piece> GetNearestEnemy() const;

private:
    std::weak_ptr<Piece> nearest_enemy_;    //weak_ptrで最も近い敵を保持
};

CEREAL_REGISTER_TYPE(PieceSensor)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceSensor)
