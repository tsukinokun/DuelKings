//---------------------------------------------------------------------------
//!	@file	UIImage.h
//! @brief	UIの画像オブジェクト
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "UIObject.h"
class ComponentImage;	//ポインタとして使用するための前方宣言
USING_PTR(UIImage);

class UIImage : public UIObject
{
public:
	BP_OBJECT_DECL(UIImage, u8"UIの画像オブジェクト")

	//@}
	bool Init() override;	   //!< 初期化
	void Update() override;	   //!< 更新
	void Draw() override;	   //!< 描画
	void Exit() override;	   //!< 終了
	void GUI() override;	   //!< GUI表示

	//画像の設定
	//! @param image [in] 描画する画像ハンドル
	//! @retval 自身のポインタ
	std::shared_ptr<UIImage> SetImage(int image);

	//画像ハンドルの取得
    //! @retval 画像のハンドル
    int GetImageHandle();
private:
	std::weak_ptr<ComponentImage>
		image_component_;	// 文字コンポーネントへの参照、使う側が毎回GetCompoenentするのも、weak_ptrを保持しておくのもさすがに面倒なので、ここでpublicにして保持しておく
};
