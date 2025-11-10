//---------------------------------------------------------------------------
//!	@file	UISynergy.h
//! @brief	UIのシナジーオブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentSynergyUI;    //ポインタとして使用するための前方宣言
USING_PTR(UISynergy);

class UISynergy : public UIObject
{
public:
    BP_OBJECT_DECL(UISynergy, u8"UIのシナジーオブジェクト")

    //@}
    bool Init() override;    //!< 初期化

    //-------------------------------------------------
    // シナジー画像の設定関数
    //! @param image_id 画像ID
    //! @retval このオブジェクトのshared_ptr
    //-------------------------------------------------
    std::shared_ptr<ComponentSynergyUI> SetSynergyImage(int image_handle);

private:
    std::weak_ptr<ComponentSynergyUI>
        synergy_component_;    // シナジーコンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
