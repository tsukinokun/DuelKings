//---------------------------------------------------------------------------
//!	@file	ModifierStatus.h
//! @brief	バフ・デバフ状態クラス定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/Component.h>
#include <Game/AutoChess/Component/StatusEffect/StatusEffectBase.h>
#include <Game/AutoChess/Info/PieceStatusModifier.h>
USING_PTR(ModifierStatus);
//---------------------------------------------------------------------------
//! @class  ModifierStatus
//! @brief  スタン状態を管理するコンポーネント
//---------------------------------------------------------------------------
class ModifierStatus : public StatusEffectBase
{
public:
    BP_COMPONENT_DECL(ModifierStatus, u8"バフ・デバフ状態クラス");

    //---------------------------------------------------------------------------
    // 初期化関数
    //---------------------------------------------------------------------------
    virtual void Init() override;

    //---------------------------------------------------------------------------
    // Create内で呼ばれる初期化関数
    //! @param owner 所有オブジェクトのポインタ
    //! @param effect_duration スタン時間
    //---------------------------------------------------------------------------
    void Construct(ObjectPtr owner, const PieceStatusModifier& piece_modifier, float effect_duration = 0.0f);

    //---------------------------------------------------------------------------
    // 解除時に呼ばれる関数
    //! @note バフ・デバフ効果を直接的に解除する処理を行う
    //---------------------------------------------------------------------------
    void OnRemove() override;

private:
    PieceStatusModifier status_modifier_;    // ステータス修正情報
};

CEREAL_REGISTER_TYPE(ModifierStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ModifierStatus)
