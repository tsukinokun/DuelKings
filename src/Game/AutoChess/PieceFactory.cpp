//---------------------------------------------------------------------------
//!	@file	PieceFactory.cpp
//! @brief	駒生成クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceFactory.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Piece/JapaneseChessKing.h>
//---------------------------------------------------------------------------
//! ピース名を受け取って対応する駒インスタンスを生成する
//---------------------------------------------------------------------------
std::shared_ptr<Piece> PieceFactory::CreatePiece(const std::string& type)
{
    if(type == "JapaneseChessKing") {
        return Scene::Object::Create<JapaneseChessKing>();
    }
    else {
        return nullptr;    // 未知のユニット
    }
}
