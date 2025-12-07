//---------------------------------------------------------------------------
//!	@file	PieceStatusModifier.cpp
//! @brief	ピースのステータスクラス、この情報バッファをピースの実体がインスタンスとして持つ
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceStatusModifier.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------
//! @brief リスナーを設定する
//---------------------------------------------------------------------------
void PieceStatusModifier::SetListener(IStatusModifierListener* listener)
{
    listener_ = listener;
}

//---------------------------------------------------------------------------
//! @brief 最大HP加算値を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddMaxHP(int value)
{
    add_max_hp_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 最大HP加算値を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveMaxHP(int value)
{
    add_max_hp_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 最大HP加算値を取得する
//---------------------------------------------------------------------------
int PieceStatusModifier::GetAddMaxHP() const
{
    return add_max_hp_;
}

//---------------------------------------------------------------------------
//! @brief 最大HP倍率を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddRateMaxHP(float value)
{
    rate_max_hp_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 最大HP倍率を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveRateMaxHP(float value)
{
    rate_max_hp_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 最大HP倍率を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetRateMaxHP() const
{
    return rate_max_hp_;
}

//---------------------------------------------------------------------------
//! @brief 攻撃力加算値を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddAttackPower(int value)
{
    add_attack_power_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃力加算値を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveAttackPower(int value)
{
    add_attack_power_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃力加算値を取得する
//---------------------------------------------------------------------------
int PieceStatusModifier::GetAddAttackPower() const
{
    return add_attack_power_;
}

//---------------------------------------------------------------------------
//! @brief 攻撃力倍率を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddRateAttackPower(float value)
{
    rate_attack_power_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃力倍率を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveRateAttackPower(float value)
{
    rate_attack_power_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃力倍率を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetRateAttackPower() const
{
    return rate_attack_power_;
}

//---------------------------------------------------------------------------
//! @brief 攻撃間隔加算値を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddAttackInterval(float value)
{
    add_attack_interval_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃間隔加算値を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveAttackInterval(float value)
{
    add_attack_interval_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃間隔加算値を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetAddAttackInterval() const
{
    return add_attack_interval_;
}

//---------------------------------------------------------------------------
//! @brief 攻撃間隔倍率を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddRateAttackInterval(float value)
{
    rate_attack_interval_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃間隔倍率を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveRateAttackInterval(float value)
{
    rate_attack_interval_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 攻撃間隔倍率を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetRateAttackInterval() const
{
    return rate_attack_interval_;
}

//---------------------------------------------------------------------------
//! @brief 物理防御力加算値を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddPhysicalDefense(int value)
{
    add_physical_defense_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 物理防御力加算値を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemovePhysicalDefense(int value)
{
    add_physical_defense_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 物理防御力加算値を取得する
//---------------------------------------------------------------------------
int PieceStatusModifier::GetAddPhysicalDefense() const
{
    return add_physical_defense_;
}

//---------------------------------------------------------------------------
//! @brief 物理防御力倍率を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddRatePhysicalDefense(float value)
{
    rate_physical_defense_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 物理防御力倍率を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveRatePhysicalDefense(float value)
{
    rate_physical_defense_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 物理防御力倍率を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetRatePhysicalDefense() const
{
    return rate_physical_defense_;
}

//---------------------------------------------------------------------------
//! @brief 魔法防御力加算値を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddMagicalDefense(float value)
{
    add_magical_defense_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 魔法防御力加算値を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveMagicalDefense(float value)
{
    add_magical_defense_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 魔法防御力加算値を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetAddMagicalDefense() const
{
    return add_magical_defense_;
}

//---------------------------------------------------------------------------
//! @brief 魔法防御力倍率を増やす
//---------------------------------------------------------------------------
void PieceStatusModifier::AddRateMagicalDefense(float value)
{
    rate_magical_defense_ += value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 魔法防御力倍率を減らす
//---------------------------------------------------------------------------
void PieceStatusModifier::RemoveRateMagicalDefense(float value)
{
    rate_magical_defense_ -= value;
    NotifyChanged();
}

//---------------------------------------------------------------------------
//! @brief 魔法防御力倍率を取得する
//---------------------------------------------------------------------------
float PieceStatusModifier::GetRateMagicalDefense() const
{
    return rate_magical_defense_;
}

//---------------------------------------------------------------------------
//! @brief ステータス変更通知をリスナーへ送る
//---------------------------------------------------------------------------
void PieceStatusModifier::NotifyChanged()
{
    if(listener_)
        listener_->OnModifierChanged();
}
