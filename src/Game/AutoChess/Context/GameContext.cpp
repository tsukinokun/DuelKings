//---------------------------------------------------------------------------
//!	@file	GameContext.cpp
//! @brief	マスターデータ群
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "GameContext.h"
//------------------------------------------------------
//! @brief 駒・シナジーのマスターデータを読み込む
//------------------------------------------------------
bool GameContext::LoadRepositories(const std::string& piece_path)
{
    bool piece_loaded = piece_repo_.LoadFromJson(piece_path);
    return piece_loaded;
}
