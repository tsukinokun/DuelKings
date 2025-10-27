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
    bool Init() override;    //!< 初期化
    void Draw() override;    //!< 描画

    //マスの生成
    void CreateSquare();

    //オーナーの名前を設定
    //! @param owner [in] オーナー
    void SetOwner(std::weak_ptr<Object> owner);

    //マスのウィークポインタを取得
    //! @retval マスのウィークポインタ
    std::array<std::array<std::weak_ptr<Square>, 8>, 4> GetSquarePtrArray();

    //マスに置かれている駒の数を取得
    //! @retval 駒の数
    int GetPieceNumOnSquares() const;
    //@}
private:
    const int                                           FILE_MAX_  = 8;    //ファイルの数
    const int                                           FILE_HALF_ = FILE_MAX_ / 2;
    const int                                           RANK_MAX_  = 8;    //ランクの数
    const int                                           RANK_HALF_ = RANK_MAX_ / 2;
    std::weak_ptr<Object>                               owner_;      //オーナーのウィークポインタ
    std::array<std::array<std::weak_ptr<Square>, 8>, 4> squares_;    //ボードのマス管理
};
