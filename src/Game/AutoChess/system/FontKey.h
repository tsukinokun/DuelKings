//---------------------------------------------------------------------------
//!	@file	FontKey.h
//! @brief	フォントのキー構造体
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

struct FontKey
{
    std::string font_name_;
    int         size_;
    int         thickness_;
    int         type_;
    int         edge_size_;

    // コンストラクタ
    FontKey(const std::string& name, int s, int t = 1, int ty = DX_FONTTYPE_NORMAL, int edge_size = 0)
        : font_name_(name)
        , size_(s)
        , thickness_(t)
        , type_(ty)
        , edge_size_(edge_size)
    {
    }

    // 等価比較演算子
    //! @param other [in] 自身と比較したいフォントキー
    //! @retval 等価か否か
    bool operator==(const FontKey& other) const
    {
        return font_name_ == other.font_name_ && size_ == other.size_ && thickness_ == other.thickness_ && type_ == other.type_ &&
               edge_size_ == other.edge_size_;
    }
};

// ハッシュ関数の定義（unordered_map用）
namespace std {
template <>
struct hash<FontKey>
{
    std::size_t operator()(const FontKey& k) const
    {
        return hash<std::string>()(k.font_name_) ^ (hash<int>()(k.size_) << 1) ^ (hash<int>()(k.thickness_) << 2) ^ (hash<int>()(k.type_) << 3) ^
               (hash<int>()(k.edge_size_) << 4);
    }
};
}    // namespace std
