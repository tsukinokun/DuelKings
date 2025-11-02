#pragma once
//---------------------------------------------------------------------------
//!	@file	ComponentGauge.h
//! @brief	ゲージコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentGauge);

class ComponentGauge : public Component
{
public:
    BP_COMPONENT_DECL(ComponentGauge, u8"ゲージコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //---------------------------------------------------------------------------
    // ゲージの割合を設定する関数
    //! @param size [in] ゲージの幅と高さ
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<ComponentGauge> SetGaugeSize(const int2& size);

    //---------------------------------------------------------------------------
    // 滑らかに変化する時間を設定する関数
    //! @param duration [in] 滑らかに変化する時間
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<ComponentGauge> SetDuration(float duration);

    //---------------------------------------------------------------------------
    // ゲージの割合を設定する関数
    //! @param rate [in] ゲージの割合(0.0~1.0)
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<ComponentGauge> SetGaugeRate(float rate);

    //---------------------------------------------------------------------------
    // ゲージの色を設定する関数
    //! @param color [in] ゲージの色(16進数で)
    //! @param smooth_color [in] 滑らかに変化する色(16進数で)
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<ComponentGauge> SetGaugeColor(int color, int smooth_color = GetColor(255, 191, 0));

    //---------------------------------------------------------------------------
    //	ゲージ座標の補正値を取得する関数
    //! @retval ゲージ座標の補正値
    //---------------------------------------------------------------------------
    float3 GetAdjustment() const;

private:
    float2 gauge_size_          = float2(100.0f, 20.0f);    // ゲージのサイズ(幅と高さ)
    float  elapsed_time_        = 0.0f;                     // 経過時間
    float  duration_            = 0.5f;                     // 滑らかに変化する時間
    float  gauge_rate_          = 1.0f;                     // ゲージの割合(0.0~1.0)
    float  duration_start_rate_ = 1.0f;                     // 滑らかに変化する開始時の割合
    float  smooth_rate_         = 1.0f;                     // 滑らかに変化する割合
    int    color_               = GetColor(0, 255, 0);      // ゲージの色
    int    smooth_color_        = GetColor(255, 191, 0);    // 滑らかに変化する色

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentGauge, 3);
