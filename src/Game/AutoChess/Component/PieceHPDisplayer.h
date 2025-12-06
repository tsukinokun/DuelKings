//---------------------------------------------------------------------------
//!	@file	PieceHPDisplayer.h
//! @brief	ピースのHP表示機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
class Piece;    //前方宣言
USING_PTR(PieceHPDisplayer);

class PieceHPDisplayer : public Component
{
public:
    BP_COMPONENT_DECL(PieceHPDisplayer, u8"ピースのHP表示機能クラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // HPバーの色を設定する関数
    //! @param color [in] HPバーの色(16進数で)
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<PieceHPDisplayer> SetHPBarColor(int color);

private:
    int hp_bar_color = GetColor(0, 255, 0);    //HPバーの色
};

CEREAL_REGISTER_TYPE(PieceHPDisplayer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceHPDisplayer)
