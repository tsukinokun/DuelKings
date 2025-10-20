//---------------------------------------------------------------------------
//!	@file	PieceFactory.cpp
//! @brief	駒生成クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceFactory.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Piece/JapaneseChessBishop.h>
#include <Game/AutoChess/Piece/JapaneseChessGold.h>
#include <Game/AutoChess/Piece/JapaneseChessKing.h>
#include <Game/AutoChess/Piece/JapaneseChessKnight.h>
#include <Game/AutoChess/Piece/JapaneseChessLance.h>
#include <Game/AutoChess/Piece/JapaneseChessPawn.h>
#include <Game/AutoChess/Piece/JapaneseChessRook.h>
#include <Game/AutoChess/Piece/JapaneseChessSilver.h>
//---------------------------------------------------------------------------
//! ピース名を受け取って対応する駒インスタンスを生成する
//---------------------------------------------------------------------------
std::shared_ptr<Piece> PieceFactory::CreatePiece(const std::string& type)
{
    if(type == "JapaneseChessBishop") {
        return Scene::Object::Create<JapaneseChessBishop>();
    }
    else if(type == "JapaneseChessGold") {
        return Scene::Object::Create<JapaneseChessGold>();
    }
    else if(type == "JapaneseChessKing") {
        return Scene::Object::Create<JapaneseChessKing>();
    }
    else if(type == "JapaneseChessKnight") {
        return Scene::Object::Create<JapaneseChessKnight>();
    }
    else if(type == "JapaneseChessLance") {
        return Scene::Object::Create<JapaneseChessLance>();
    }
    else if(type == "JapaneseChessPawn") {
        return Scene::Object::Create<JapaneseChessPawn>();
    }
    else if(type == "JapaneseChessRook") {
        return Scene::Object::Create<JapaneseChessRook>();
    }
    else if(type == "JapaneseChessSilver") {
        return Scene::Object::Create<JapaneseChessSilver>();
    }
    else {
        return nullptr;    // 未知のユニット
    }
}
