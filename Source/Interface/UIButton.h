//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include "../Math.h"
#include "UIText.h"
#include <string>
#include <functional>

class UIButton
{
public:
    void SetText(const std::string& text);
    void SetHighlighted(bool sel);
    bool GetHighlighted() const ;
    bool ContainsPoint(const Vector2& pt) const;
    void OnClick();
private:
    std::function<void()> mOnClick;
    UIText mText;
    bool mHighlighted;
    Vector2 mPosition;
};