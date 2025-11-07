//---------------------------------------------------------------------------
//!	@file	Synergy.h
//! @brief	シナジーの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include "SynergyID.h"
#include <Game/AutoChess/Synergy/SynergyEffect/SynergyEffect.h>
//------------------------------------------------------
//! @brief 発動中のシナジー情報を保持するクラス
//------------------------------------------------------
class Synergy
{
public:
    //--------------------------------------------------
    //! @brief クラス内ビルダーによる構築
    //--------------------------------------------------
    class Builder
    {
    public:
        Builder& SetID(SynergyID id)
        {
            id_ = id;
            return *this;
        }

        Builder& SetName(const std::string& name)
        {
            name_ = name;
            return *this;
        }

        Builder& SetDescription(const std::string& description)
        {
            description_ = description;
            return *this;
        }

        Builder& SetEffect(const std::shared_ptr<SynergyEffect>& effect)
        {
            effect_ = effect;
            return *this;
        }

        Synergy Build() const { return Synergy(id_, name_, description_, effect_); }

    private:
        SynergyID                      id_;
        std::string                    name_;
        std::string                    description_;
        std::shared_ptr<SynergyEffect> effect_;
    };

    //--------------------------------------------------
    //! @brief シナジーIDを取得
    //--------------------------------------------------
    SynergyID GetID() const { return id_; }

    //--------------------------------------------------
    //! @brief シナジー名を取得
    //--------------------------------------------------
    const std::string& GetName() const { return name_; }

    //--------------------------------------------------
    //! @brief シナジー説明文を取得
    //--------------------------------------------------
    const std::string& GetDescription() const { return description_; }

    //--------------------------------------------------
    //! @brief シナジー効果を取得
    //--------------------------------------------------
    const std::shared_ptr<SynergyEffect>& GetEffect() const { return effect_; }

private:
    //--------------------------------------------------
    //! @brief プライベートコンストラクタ（Builder専用）
    //--------------------------------------------------
    Synergy(SynergyID id, const std::string& name, const std::string& description, const std::shared_ptr<SynergyEffect>& effect)
        : id_(id)
        , name_(name)
        , description_(description)
        , effect_(effect)
    {
    }

    SynergyID                      id_;
    std::string                    name_;
    std::string                    description_;
    std::shared_ptr<SynergyEffect> effect_;
};
