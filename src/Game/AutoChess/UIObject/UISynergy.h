//---------------------------------------------------------------------------
//!	@file	UISynergy.h
//! @brief	UIのシナジーオブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/AutoChess/Synergy/SynergyData.h>
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
    std::shared_ptr<UISynergy> SetSynergyImage(int image_handle);

    //---------------------------------------------------------------------------
    //  マウスをクリックした時に行う処理の設定
    //! @param info [in] クリック時の関数
    //! @return 自身のポインタ
    //---------------------------------------------------------------------------
    std::shared_ptr<UISynergy> SetClickFunc(const std::function<void()>& click_func);

    //-------------------------------------------------
    // 次のレベルまでの必要数を設定する関数
    //! @param next_count 次のレベルまでの必要数
    //! @retval このオブジェクトのshared_ptr
    //-------------------------------------------------
    std::shared_ptr<UISynergy> SetNextCount(int next_count);

    //-------------------------------------------------
    // シナジーの数を設定する関数
    //! @param synergy_count シナジーの数
    //! @retval このオブジェクトのshared_ptr
    //-------------------------------------------------
    std::shared_ptr<UISynergy> SetSynergyCount(int synergy_count);

    //-------------------------------------------------
    // シナジーデータのポインタを設定する関数
    //! @param synergy_data シナジーデータのポインタ
    //! @retval このオブジェクトのshared_ptr
    //-------------------------------------------------
    std::shared_ptr<UISynergy> SetSynergyData(const SynergyData* synergy_data);

private:
    std::weak_ptr<ComponentSynergyUI>
        synergy_component_;    // シナジーコンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
