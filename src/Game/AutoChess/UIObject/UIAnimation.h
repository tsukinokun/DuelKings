//---------------------------------------------------------------------------
//!	@file	UIAnimation.h
//! @brief	UIのアニメーションオブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentAnimUI;    //ポインタとして使用するための前方宣言
USING_PTR(UIAnimation);

class UIAnimation : public UIObject
{
public:
    BP_OBJECT_DECL(UIAnimation, u8"UIのアニメーションオブジェクト")

    //@}
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    //アニメーションステータスの設定
    //! @param gh [in] スプライトのハンドル
    //! @param div_num [in] 分割数
    //! @param ex_rate [in] 拡大率
    //! @param update_frame [in] 何フレームに一回アニメーションの更新を行うか
    //! @retval 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UIAnimation> SetAnimStatus(int gh, int div_num, float ex_rate = 1.0f, int update_frame = 5);

    //--------------------------------------------------------------------
    // 透明度の設定
    //! @param alpha [in] 透明度(0~255)
    //--------------------------------------------------------------------
    std::shared_ptr<UIAnimation> SetAlpha(int alpha);

private:
    std::weak_ptr<ComponentAnimUI>
        anim_component_;    // 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
