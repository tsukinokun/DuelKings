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
        //--------------------------------------------------
        //! @brief シナジーIDを設定
        //! @param id シナジーID
        //--------------------------------------------------
        Builder& SetID(SynergyID id)
        {
            id_ = id;
            return *this;
        }

        //--------------------------------------------------
        //! @brief 発動しているユニット数を設定
        //! @param count 発動しているユニット数(重複なし)
        //--------------------------------------------------
        Builder& SetSynergyCount(int count)
        {
            synergy_count_ = count;
            return *this;
        }

        //--------------------------------------------------
        //! @brief シナジー名を設定
        //! @param name シナジー名
        //--------------------------------------------------
        Builder& SetName(const std::string& name)
        {
            name_ = name;
            return *this;
        }

        //--------------------------------------------------
        //! @brief シナジー説明文を設定
        //! @param description シナジー説明文
        //--------------------------------------------------
        Builder& SetDescription(const std::string& description)
        {
            description_ = description;
            return *this;
        }

        //--------------------------------------------------
        //! @brief シナジー効果を設定
        //! @param effect シナジー効果
        //--------------------------------------------------
        Builder& SetEffect(const std::shared_ptr<SynergyEffect>& effect)
        {
            effect_ = effect;
            return *this;
        }

        //--------------------------------------------------
        //! @brief Synergy オブジェクトを構築
        //! @return Synergy オブジェクト
        //--------------------------------------------------
        Synergy Build() const { return Synergy(id_, synergy_count_, name_, description_, effect_); }

    private:
        SynergyID                      id_;
        int                            synergy_count_;    //!< 発動しているユニット数(重複なし)
        std::string                    name_;
        std::string                    description_;
        std::shared_ptr<SynergyEffect> effect_;
    };

    //--------------------------------------------------
    // シナジーIDを取得
    //! @return シナジーID
    //--------------------------------------------------
    SynergyID GetID() const;

    //--------------------------------------------------
    // シナジー名を取得
    //! @return シナジー名
    //--------------------------------------------------
    const std::string& GetName() const;

    //--------------------------------------------------
    // シナジー説明文を取得
    //! @return シナジー説明文
    //--------------------------------------------------
    const std::string& GetDescription() const;

    //--------------------------------------------------
    // シナジー効果を取得
    //! @return シナジー効果
    //--------------------------------------------------
    const std::shared_ptr<SynergyEffect>& GetEffect() const;

    //--------------------------------------------------
    // 発動数を取得
    //! @return 発動しているユニット数(重複なし)
    //--------------------------------------------------
    int GetSynergyCount() const;

    //--------------------------------------------------
    // カウントを増やす
    //--------------------------------------------------
    void IncrementSynergyCount();

private:
    //--------------------------------------------------
    //! @brief プライベートコンストラクタ（Builder専用）
    //--------------------------------------------------
    Synergy(SynergyID id, int synergy_count, const std::string& name, const std::string& description, const std::shared_ptr<SynergyEffect>& effect)
        : id_(id)
        , synergy_count_(synergy_count)
        , name_(name)
        , description_(description)
        , effect_(effect)
    {
    }

    SynergyID                      id_;
    int                            synergy_count_;    //!< 発動しているユニット数(重複なし)
    std::string                    name_;
    std::string                    description_;
    std::shared_ptr<SynergyEffect> effect_;
};
