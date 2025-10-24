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
#include <Game/AutoChess/Piece/ChessPawn.h>
//---------------------------------------------------------------------------
//! ピース名を受け取って対応する駒インスタンスを生成する
//---------------------------------------------------------------------------
std::shared_ptr<Piece> PieceFactory::CreatePiece(const std::string& type)
{
    if(type == "JapaneseChessBishop") {
        //角行
        return Scene::Object::Create<JapaneseChessBishop>();
    }
    else if(type == "JapaneseChessGold") {
        //金将
        return Scene::Object::Create<JapaneseChessGold>();
    }
    else if(type == "JapaneseChessKing") {
        //王将
        return Scene::Object::Create<JapaneseChessKing>();
    }
    else if(type == "JapaneseChessKnight") {
        //桂馬
        return Scene::Object::Create<JapaneseChessKnight>();
    }
    else if(type == "JapaneseChessLance") {
        //香車
        return Scene::Object::Create<JapaneseChessLance>();
    }
    else if(type == "JapaneseChessPawn") {
        //歩兵
        return Scene::Object::Create<JapaneseChessPawn>();
    }
    else if(type == "JapaneseChessRook") {
        //飛車
        return Scene::Object::Create<JapaneseChessRook>();
    }
    else if(type == "JapaneseChessSilver") {
        //銀将
        return Scene::Object::Create<JapaneseChessSilver>();
    }
    else if(type == "ChessPawn") {
        //ポーン
        return Scene::Object::Create<ChessPawn>();
    }
    else {
        return nullptr;    // 未知のユニット
    }
}
