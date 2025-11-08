//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include <SDL_stdinc.h>
#include "../Math.h"
#include <functional>
#include <string>

class UIScreen {
public:
    virtual void Update(float deltaTime);
    virtual void Draw(class Shader* shader);
    virtual void ProcessInput(const uint8_t* keys);
    void Open();
    void Close();
    enum UIState { EActive, Eclosing };
    void AddButton(const std::string& name,
    std::function<void()> onClick, const Vector2 &pos);
    void AddText(const std::string& text);
    void AddImage(const std::string& path);
private:
    UIState mState;
};