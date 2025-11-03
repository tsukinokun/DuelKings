//---------------------------------------------------------------------------
//!	@file	UIGauge.h
//! @brief	UIのゲージオブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentGauge;    //ポインタとして使用するための前方宣言
USING_PTR(UIGauge);

class UIGauge : public UIObject
{
public:
    BP_OBJECT_DECL(UIGauge, u8"UIのゲージオブジェクト")

    //@}
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // ゲージの割合を設定する関数
    //! @param size [in] ゲージの幅と高さ
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UIGauge> SetGaugeSize(const int2& size);

    //---------------------------------------------------------------------------
    // 滑らかに変化する時間を設定する関数
    //! @param duration [in] 滑らかに変化する時間
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UIGauge> SetDuration(float duration);

    //---------------------------------------------------------------------------
    // ゲージの割合を設定する関数
    //! @param rate [in] ゲージの割合(0.0~1.0)
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UIGauge> SetGaugeRate(float rate);

    //---------------------------------------------------------------------------
    // ゲージの色を設定する関数
    //! @param color [in] ゲージの色(16進数で)
    //! @param smooth_color [in] 滑らかに変化する色(16進数で)
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UIGauge> SetGaugeColor(int color, int smooth_color);

private:
    std::weak_ptr<ComponentGauge>
        gauge_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
