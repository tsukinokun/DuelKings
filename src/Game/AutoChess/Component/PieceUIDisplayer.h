//---------------------------------------------------------------------------
//!	@file	PieceUIDisplayer.h
//! @brief	ピースのHP表示機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
class Piece;    //前方宣言
USING_PTR(PieceUIDisplayer);
class UIGauge;
class PieceUIDisplayer : public Component
{
public:
    BP_COMPONENT_DECL(PieceUIDisplayer, u8"ピースのHP表示機能クラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // HPバーの色を設定する関数
    //! @param color [in] HPバーの色(16進数で)
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<PieceUIDisplayer> SetHPBarColor(int color);

private:
    //---------------------------------------------------------------------------
    // スクリーン座標を取得し、指定オフセットを加算する
    //! @param owner  [in] ピースオブジェクト
    //! @param offset [in]スクリーン座標に加えるオフセット（デフォルトは上方向へ30px）
    //! @return オフセットを加えたスクリーン座標
    //---------------------------------------------------------------------------
    float2 GetScreenPositionWithOffset(const Piece* owner, const float2& offset = float2(0.0f, -30.0f));

    //---------------------------------------------------------------------------
    // ゲージオブジェクトを生成する
    //! @param name ゲージの名前
    //! @param owner ピースオブジェクト
    //! @param color ゲージの色
    //! @param size ゲージのサイズ
    //! @return 生成されたUIGaugeの共有ポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UIGauge> CreateGauge(const std::string& name, const std::shared_ptr<Piece>& owner, int color, const int2& size);

    //---------------------------------------------------------------------------
    // 任意のゲージ（HP/MP/スタミナなど）を更新する
    //! @param owner             [in] ピースオブジェクト（ゲージの対象）
    //! @param gauge_name_suffix [in] ゲージ名のサフィックス（例: "hp_ui_name", "mp_ui_name"）
    //! @param current_value     [in] 現在値（HPやMPなど）
    //! @param max_value         [in] 最大値（HPやMPなど）
    //! @param gauge_color       [in] ゲージの色
    //! @param gauge_size        [in] ゲージのサイズ（幅・高さ）
    //! @param offset            [in] スクリーン座標に加えるオフセット（デフォルトは上方向へ30px、ピースのド真ん中に表示されると面倒）
    //! @details
    // - 現在値が最大値未満の場合はゲージを表示・更新する
    // - 現在値が最大値以上の場合はゲージを非表示にする
    // - ゲージが存在しない場合は新規作成する
    //---------------------------------------------------------------------------
    void UpdateGauge(const std::shared_ptr<Piece>& owner,
                     const std::string&            gauge_name_suffix,
                     int                           current_value,
                     int                           max_value,
                     int                           gauge_color,
                     const int2&                   gauge_size,
                     const float2&                 offset = float2(0.0f, -30.0f));

private:
    int hp_bar_color = GetColor(0, 255, 0);    //HPバーの色
};

CEREAL_REGISTER_TYPE(PieceUIDisplayer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceUIDisplayer)
