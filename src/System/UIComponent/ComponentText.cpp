//---------------------------------------------------------------------------
//!	@file	ComponentText.cpp
//! @brief	文字列コンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentText.h"
#include "ComponentTransformUI.h"
#include <Game/AutoChess/system/FontBuffer.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentText::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        auto   owner      = GetOwner();                  //オーナーを取得
        float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
        int    font_handle =
            FontBuffer::GetFontHandle(font_name_, font_size_, thick_size_, DX_FONTTYPE_ANTIALIASING_EDGE, edge_size_);    //フォントのハンドルを取得
        if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
            ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
            float                           hight     = 0;    //フォントサイズを取得(=高さ)
            float                           width     = 0;    // 文字列の幅を取得
            //フォントハンドルがあるかデフォルトフォントかで分岐
            if(font_handle != -1) {
                //フォントが存在しているなら
                //配置位置(縦)
                hight = static_cast<float>(GetFontSizeToHandle(font_handle));    //フォントサイズを取得(=高さ)
                //配置位置(横)
                width = static_cast<float>(GetDrawStringWidthToHandle(str_.data(), str_.size(), font_handle));    // 文字列の幅を取得
            }
            else {
                //存在していないなら
                DxLib::SetFontSize(font_size_);    //フォントサイズに合わせる
                //配置位置(縦)
                hight = static_cast<float>(GetFontSize());    //フォントサイズを取得(=高さ)
                //配置位置(横)
                width = static_cast<float>(GetDrawStringWidth(str_.data(), str_.size()));    // 文字列の幅を取得
            }
            //配置位置(縦)
            switch(static_cast<int>(alignment) / 3) {
            case 0:
                adjustment.y = 0.0f;
                break;    // 上寄せ
            case 1:
                adjustment.y = (-hight * 0.5f);
                break;    // 中央寄せ
            case 2:
                adjustment.y = -hight;
                break;    // 下寄せ
            }
            //配置位置(横)
            switch(static_cast<int>(alignment) % 3) {
            case 0:
                adjustment.x = 0.0f;
                break;    // 左寄せ
            case 1:
                adjustment.x = (-width * 0.5f);
                break;    // 中央寄せ
            case 2:
                adjustment.x = -width;
                break;    // 右寄せ
            }
        }
        float3 pos = float3(0.0f, 0.0f, 0.0f);
        pos        = owner->GetTranslate() + adjustment;

        //折り返し処理
        std::string wrapped_text = WrapText(font_handle);
        // 透明度の設定
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
        //フォントが存在しているかで分岐
        if(font_handle != -1) {
            DrawStringToHandle(pos.x, pos.y, wrapped_text.data(), text_color_, font_handle, edge_color_);
        }
        else {
            //存在しない
            DrawString(pos.x, pos.y, wrapped_text.data(), text_color_, edge_color_);
            DxLib::SetFontSize(DEFAULT_FONT_SIZE);    //フォントサイズを元に戻す
        }
        //透明度を元に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NORMAL));
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentText::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"文字列コンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//---------------------------------------------------------------------------
//! @brief コンストラクト関数
//---------------------------------------------------------------------------
void ComponentText::Construct(ObjectPtr owner)
{
    __super::Construct(owner);
    Init();
}

//---------------------------------------------------------------------------
//! @brief	フォントの変更
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetFontName(const std::string& font_name)
{
    font_name_ = font_name;
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	文字列の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetText(const std::string_view& str)
{
    str_ = std::string(str);    // コピーして保持
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	文字列色の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetColor(int text_color, int edge_color)
{
    text_color_ = text_color;    // 文字色を設定
    edge_color_ = edge_color;    // 縁取り色を設定
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	フォントサイズの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetFontSize(int font_size)
{
    font_size_ = font_size;    // フォントサイズを設定
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief  エッジサイズの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetEdgeSize(int edge_size)
{
    edge_size_ = edge_size;
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief 文字の太さを設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetThickSize(int thick_size)
{
    // 太さは1から9の間で設定
    if(thick_size < 0) {
        thick_size = 0;
    }
    else if(thick_size > 9) {
        thick_size = 9;
    }
    thick_size_ = thick_size;
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

size_t Utf8CharLen(unsigned char c)
{
    if((c & 0x80) == 0x00)
        return 1;
    if((c & 0xE0) == 0xC0)
        return 2;
    if((c & 0xF0) == 0xE0)
        return 3;
    return 4;
}

//---------------------------------------------------------------------------
//! @brief テキストの折り返し処理
//---------------------------------------------------------------------------
std::string ComponentText::WrapText(int font_handle)
{
    std::string result;
    std::string current_line;

    size_t i = 0;
    while(i < str_.size()) {
        size_t      len  = Utf8CharLen((unsigned char)str_[i]);
        std::string ch   = str_.substr(i, len);
        i               += len;

        // 改行
        if(ch == "\n") {
            result += current_line + "\n";
            current_line.clear();
            continue;
        }

        // 幅チェック
        std::string test_line = current_line + ch;

        int width = (font_handle != -1) ? GetDrawStringWidthToHandle(test_line.c_str(), test_line.size(), font_handle)
                                        : GetDrawStringWidth(test_line.c_str(), test_line.size());

        if(width > max_width_) {
            result       += current_line + "\n";
            current_line  = ch;
        }
        else {
            current_line = test_line;
        }
    }

    result += current_line;
    return result;
}

//---------------------------------------------------------------------------
//! @brief 折り返し幅を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetWrapWidth(int max_width)
{
    max_width_ = max_width;
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief 透明度を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<ComponentText> ComponentText::SetAlpha(int alpha)
{
    alpha_ = alpha;
    alpha_ = std::clamp(alpha_, 0, 255);
    return dynamic_pointer_cast<ComponentText>(shared_from_this());
}

CEREAL_REGISTER_TYPE(ComponentText)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentText)
