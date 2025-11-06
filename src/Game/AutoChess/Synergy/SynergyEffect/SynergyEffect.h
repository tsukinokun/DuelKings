//---------------------------------------------------------------------------
//!	@file	SynergyEffect.h
//! @brief	シナジー効果のインターフェースクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <Game/AutoChess/Agent.h>
class SynergyEffect
{
public:
    virtual ~SynergyEffect() = default;

    //--------------------------------------------------
    //! @brief Agent に効果を適用する
    //! @param agent [in] 対象のユニット（駒の振る舞いを持つ）
    //--------------------------------------------------
    virtual void ApplyTo(Agent& agent) const = 0;

    //--------------------------------------------------
    //! @brief Agent に適用した効果を解除する
    //! @param agent [in] 対象のユニット
    //--------------------------------------------------
    virtual void RemoveFrom(Agent& agent) const = 0;

    //--------------------------------------------------
    //! @brief 効果の説明文（UI表示用）
    //--------------------------------------------------
    virtual std::string GetDescription() const = 0;
};
