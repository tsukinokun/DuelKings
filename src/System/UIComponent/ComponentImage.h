#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentImage);

class ComponentImage : public Component
{
public:
    BP_COMPONENT_DECL(ComponentImage, u8"画像コンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //描画処理
    void LateDraw() override;

    //ImGui
    void GUI() override;

    //画像の設定
    //! @param image [in] 描画する画像ハンドル
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentImage> SetImage(int image);
    //! @}

private:
    int image_ = -1;    //!< 描画する画像ハンドル

    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentImage, 3);
