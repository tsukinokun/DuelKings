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

    //-------------------------------------------------
    // 次のレベルまでの必要数を設定する関数
    //! @param next_count 次のレベルまでの必要数
    //! @retval このオブジェクトのshared_ptr
    //-------------------------------------------------
    std::shared_ptr<ComponentSynergyUI> SetNextCount(int next_count);

    //-------------------------------------------------
    // シナジーの数を設定する関数
    //! @param synergy_count シナジーの数
    //! @retval このオブジェクトのshared_ptr
    //-------------------------------------------------
    std::shared_ptr<ComponentSynergyUI> SetSynergyCount(int synergy_count);

private:
    std::weak_ptr<ComponentSynergyUI>
        synergy_component_;    // シナジーコンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
