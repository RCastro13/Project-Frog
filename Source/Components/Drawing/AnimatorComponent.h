//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include <unordered_map>
#include "DrawComponent.h"

class AnimatorComponent : public DrawComponent {
public:
    // (Lower draw order corresponds with further back)
    AnimatorComponent(class Actor* owner, const std::string &texturePath, const std::string &dataPath,
            int width, int height, int drawOrder = 100);
    ~AnimatorComponent() override;

    void Draw(Renderer* renderer) override;
    void Update(float deltaTime) override;

    // Use to change the FPS of the animation
    void SetAnimFPS(float fps) { mAnimFPS = fps; }

    // Set the current active animation
    void SetAnimation(const std::string& name);

    // Setar se a animação é em looping ou nao
    void SetLooping(bool looping) {mIsLooping = looping;};

    // Use to pause/unpause the animation
    void SetIsPaused(bool pause) { mIsPaused = pause; }

    // pra setar um tempo de frame diferente do padrao que segue o FPS
    void SetFrameTime(float seconds);

    // para saber se a animação terminou
    bool IsFinished() const { return mIsFinished; }

    // Add an animation of the corresponding name to the animation map
    void AddAnimation(const std::string& name, const std::vector<int>& images);

private:
    bool LoadSpriteSheetData(const std::string& dataPath);

    // Sprite sheet texture
    class Texture* mSpriteTexture;

    // Vector of sprites
    std::vector<Vector4> mSpriteSheetData;

    // Map of animation name to vector of textures corresponding to the animation
    std::unordered_map<std::string, std::vector<int>> mAnimations;

    // Name of current animation
    std::string mAnimName;

    // A animação é em looping?
    bool mIsLooping;

    // A animação acabou? (só faz sentido para animações que rodam apenas 1 vez)
    bool mIsFinished;

    // Tracks current elapsed time in animation
    float mAnimTimer = 0.0f;

    // The frames per second the animation should run at
    float mAnimFPS = 10.0f;

    // Whether or not the animation is paused (defaults to false)
    bool mIsPaused = false;

    // Size
    int mWidth;
    int mHeight;

    float mTextureFactor;
};

