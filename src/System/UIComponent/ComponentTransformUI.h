#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentTransformUI);

class ComponentTransformUI : public Component
{
public:
    //何処にUIを配置するかのenum
    enum class Alignment
    {
        UpperLeft = 0,    //上左寄せ
        UpperCenter,      //上中央寄せ
        UpperRight,       //上右寄せ
        MiddleLeft,       //左寄せ
        MiddleCenter,     //中央寄せ
        MiddleRight,      //右寄せ
        LowerLeft,        //下左寄せ
        LowerCenter,      //下中央寄せ
        LowerRight,       //下右寄せ
    };

public:
    BP_COMPONENT_DECL(ComponentTransformUI, u8"UIのコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //描画位置の設定
    //! @param alignment [in] UIの配置位置を設定する
    //! @return	自分のSharedPtr
    std::shared_ptr<ComponentTransformUI> SetAlignment(Alignment alignment);

    // 描画位置の取得
    //!	@return	描画位置
    Alignment GetAlignment() const;
    //! @}

private:
    Alignment alignment_ = Alignment::MiddleCenter;    //配置の設定

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentTransformUI, 3);
