//---------------------------------------------------------------------------
//!	@file	ChessBoard.h
//! @brief	インゲームシーンのチェスボード
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class Square;    //前方宣言
USING_PTR(ChessBoard);
class ChessBoard : public Object
{
public:
    BP_OBJECT_DECL(ChessBoard, u8"インゲームシーンのチェスボード")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //@}
private:
    const int                                           FILE_MAX_  = 8;    //ファイルの数
    const int                                           FILE_HALF_ = FILE_MAX_ / 2;
    const int                                           RANK_MAX_  = 8;    //ランクの数
    const int                                           RANK_HALF_ = RANK_MAX_ / 2;
    std::array<std::array<std::weak_ptr<Square>, 8>, 8> squares_;    //ボードのマス管理
};
