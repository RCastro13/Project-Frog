#include "RewardScene.h"
#include "MapScene.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Actors/Player.h"
#include "../Random.h"
#include <SDL.h>
#include <sstream>

#include "../Actors/ChestNPC.h"
#include "../Actors/CoinNPC.h"

// Includes necessários para transitar entre as cenas
#include "../GameScene/GameScene.h"
#include "../GameScene/ShopScene.h"

RewardScene::RewardScene(Game* game, RewardMode mode)
    : GameScene(game)
    , mMode(mode)
    , mRewardType(RewardType::CARD)
    , mState(SceneState::ANIMATING_CHEST)
    , mCoinsAmount(0)
    , mRewardCard(nullptr)
    , mBackgroundTexture(nullptr)
    , mTextTexture(nullptr)
    , mChestNPC(nullptr)
    , mCoinNPC(nullptr)
    , mCoinSpawned(false)
    , mSelectedDeckIndex(0)
    , mKeyWasPressed(false)
{
}

RewardScene::~RewardScene()
{
}

void RewardScene::Enter()
{
    mStateTime = 0.0f;
    mFadeAlpha = 1.0f;
    mFadeTimer = 0.0f;
    mFadingIn = true;
    mCoinSpawned = false;
    mState = SceneState::ANIMATING_CHEST;

    // Carregar background
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Rewards/rewards.png");
    if (!mBackgroundTexture)
    {
        // Fallback
        mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Combat/floresta.png");
    }

    // carregar texturas de cartas
    LoadCardTextures();

    //criar e posicionar o bau
    mChestNPC = new ChestNPC(mGame);
    mChestNPC->SetPosition(Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 + 50));
    mChestNPC->Open();

    DetermineReward();
}

void RewardScene::LoadCardTextures()
{
    // Carregar apenas as texturas ativas para visualização
    mCardTextures[AttackType::Fire] = mGame->GetRenderer()->GetTexture("../Assets/Cards/fire-active.png");
    mCardTextures[AttackType::Water] = mGame->GetRenderer()->GetTexture("../Assets/Cards/water-active.png");
    mCardTextures[AttackType::Plant] = mGame->GetRenderer()->GetTexture("../Assets/Cards/plant-active.png");
    mCardTextures[AttackType::Neutral] = mGame->GetRenderer()->GetTexture("../Assets/Cards/neutral-active.png");
}

Texture* RewardScene::GetCardTexture(AttackType type)
{
    auto it = mCardTextures.find(type);
    if (it != mCardTextures.end()) {
        return it->second;
    }
    return nullptr;
}

void RewardScene::GenerateRewardCard()
{
    // gera uma carta aleatória melhorzinha que o básico
    int dmg = Random::GetIntRange(4, 8);
    int cost = Random::GetIntRange(1, 3);

    // Tipo aleatório
    int typeInt = Random::GetIntRange(0, 3);
    AttackType type = static_cast<AttackType>(typeInt);

    std::string name;
    switch(type) {
        case AttackType::Fire: name = "Mega Fire"; break;
        case AttackType::Water: name = "Super Aqua"; break;
        case AttackType::Plant: name = "Vine Whip"; break;
        case AttackType::Neutral: name = "Iron Fist"; break;
    }

    mRewardCard = new Card(name, type, dmg, cost, nullptr);
}

void RewardScene::DecideRewardSpawnLogic(float deltatime) {
    //logica de spawn
    if (!mCoinSpawned && mChestNPC && mChestNPC->IsOpenFinished()) // Só vai entrar nesse if se o baú tiver terminado de abrir
    {
        mCoinSpawned = true; // Trava para não entrar aqui de novo

        if (mMode == RewardMode::COMBAT_VICTORY)
        {
            // spawno a animação da moeda girando
            mCoinNPC = new CoinNPC(mGame);
            mCoinNPC->SetPosition(Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 - 100));

            std::stringstream ss;
            ss << "Você ganhou " << mCoinsAmount << " moedas! (ENTER)";
            SpawnText(ss.str());

            mState = SceneState::SHOWING_REWARD;
        }
        else // TREASURE_CHEST ou SHOP_TREASURE_CHEST
        {
            if (mRewardType == RewardType::CARD)
            {
                GenerateRewardCard();

                std::stringstream ss;
                ss << "Carta Nova! [1] Aceitar [2] Recusar";
                SpawnText(ss.str());

                mState = SceneState::SHOWING_REWARD;
            }
            else
            {
                mCoinNPC = new CoinNPC(mGame);
                mCoinNPC->SetPosition(Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 - 100));

                std::stringstream ss;
                ss << "Você ganhou " << mCoinsAmount << " moedas! (ENTER)";
                SpawnText(ss.str());

                mState = SceneState::SHOWING_REWARD;
            }
        }
    }
}

void RewardScene::SpawnText(std::string text) {
    if (!mGame->GetFont()) return;
    //limpando texturas antigas
    if (mTextTexture) {
        delete mTextTexture;
        mTextTexture = nullptr;
    }
    mTextTexture = mGame->GetFont()->RenderText(text, Vector3(1,1,1), 20, 500);
}

void RewardScene::DetermineReward()
{
    // Verifica se é baú (seja do mapa ou da loja)
    if (mMode == RewardMode::TREASURE_CHEST || mMode == RewardMode::SHOP_TREASURE_CHEST)
    {
        // 60% chance de carta, 40% chance de moedas
        int roll = Random::GetIntRange(0, 99);
        if (roll < 60) {
            mRewardType = RewardType::CARD;
        } else {
            mRewardType = RewardType::COINS;
            mCoinsAmount = Random::GetIntRange(50, 80);
        }
    }
    else //ganhou uma luta
    {
        mRewardType = RewardType::COINS;
        mCoinsAmount = 30;
    }
}

void RewardScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);
    DecideRewardSpawnLogic(deltaTime);
}

void RewardScene::ProcessInput(const Uint8* keyState)
{
    // bloquear inputs durante fade
    if (mFadeAlpha > 0.0f) return;

    // se a animação do baú ainda não acabou, não processa input
    if (mState == SceneState::ANIMATING_CHEST) return;

    if (mState == SceneState::SHOWING_REWARD)
    {
        // CASO 1: ganhou Moedas (combate ou baú)
        if (mRewardType == RewardType::COINS)
        {
            if (keyState[SDL_SCANCODE_RETURN] && !mKeyWasPressed) {
                GiveRewardToPlayer();
                mKeyWasPressed = true;

                // Lógica de Retorno Ajustada:
                // Se for SHOP_TREASURE_CHEST -> volta pra Loja
                // Se for TREASURE_CHEST (mapa) ou COMBAT_VICTORY -> volta pro Mapa
                if (mMode == RewardMode::SHOP_TREASURE_CHEST) {
                    mGame->SetScene(new ShopScene(mGame));
                } else {
                    mGame->SetScene(new MapScene(mGame));
                }
            }
        }
        // CASO 2: ganhou Carta (baú)
        else if (mRewardType == RewardType::CARD)
        {
            // [1] aceitar -> vai para seleção de troca
            if (keyState[SDL_SCANCODE_1] && !mKeyWasPressed)
            {
                mState = SceneState::CHOOSING_REPLACEMENT;
                SpawnText("Escolha uma carta para trocar (ENTER)");
                mKeyWasPressed = true;
            }
            // [2] recusar -> sai
            else if (keyState[SDL_SCANCODE_2] && !mKeyWasPressed)
            {
                mKeyWasPressed = true;

                // Retorno
                if (mMode == RewardMode::SHOP_TREASURE_CHEST) {
                    mGame->SetScene(new ShopScene(mGame));
                } else {
                    mGame->SetScene(new MapScene(mGame));
                }
            }
        }
    }
    else if (mState == SceneState::CHOOSING_REPLACEMENT)
    {
        // navegação esquerda/direita
        if ((keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) && !mKeyWasPressed)
        {
            mSelectedDeckIndex = (mSelectedDeckIndex + 1) % 4;
            mKeyWasPressed = true;
        }
        else if ((keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) && !mKeyWasPressed)
        {
            mSelectedDeckIndex = (mSelectedDeckIndex - 1 + 4) % 4;
            mKeyWasPressed = true;
        }
        // confirmar troca
        else if ((keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed)
        {
            Player* player = mGame->GetPlayer();
            if (player && mRewardCard)
            {
                std::vector<Card*>& deck = player->GetDeck();
                if (mSelectedDeckIndex >= 0 && mSelectedDeckIndex < deck.size())
                {
                    Card* oldCard = deck[mSelectedDeckIndex];

                    SDL_Log("Trocando %s por %s", oldCard->GetName().c_str(), mRewardCard->GetName().c_str());

                    // Substituir diretamente no índice para manter a ordem
                    deck[mSelectedDeckIndex] = mRewardCard;
                    mRewardCard->SetOwner(player);
                    delete oldCard; // deleto a carta antiga

                    mRewardCard = nullptr; // o player agora é dono, então, n precisa deletar no Exit
                }
            }

            mKeyWasPressed = true;

            // Retorno
            if (mMode == RewardMode::SHOP_TREASURE_CHEST) {
                mGame->SetScene(new ShopScene(mGame));
            } else {
                mGame->SetScene(new MapScene(mGame));
            }
        }
        // cancelar (volta para opção aceitar/recusar da carta)
        else if (keyState[SDL_SCANCODE_ESCAPE] && !mKeyWasPressed)
        {
             mState = SceneState::SHOWING_REWARD;
             SpawnText("Carta Rara! [1] Aceitar [2] Recusar");
             mKeyWasPressed = true;
        }
    }

    if (!keyState[SDL_SCANCODE_1] && !keyState[SDL_SCANCODE_2] &&
        !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE] &&
        !keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT] &&
        !keyState[SDL_SCANCODE_ESCAPE])
    {
        mKeyWasPressed = false;
    }
}

void RewardScene::GiveRewardToPlayer()
{
    if (mRewardType == RewardType::COINS)
    {
        SDL_Log("[REWARD] Player ganhou %d moedas!", mCoinsAmount);
        mGame->GetPlayer()->GiveCoins(mCoinsAmount);
    }
}

void RewardScene::RenderBackground()
{
    if (mBackgroundTexture)
    {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 224.0f), Vector2(640.0f, 448.0f), 0.0f,
            Vector3(1.0f, 1.0f, 1.0f), mBackgroundTexture,
            Vector4::UnitRect, Vector2::Zero
        );
    }
}

void RewardScene::RenderRewardCard()
{
    if (!mRewardCard) return;

    // desenha a carta ganha no centro (um pouco acima do texto)
    Texture* tex = GetCardTexture(mRewardCard->GetType());
    if (tex) {
        // aumentei um pouco a escala para dar destaque
        Vector2 cardSize = Vector2(RewardConstants::CARD_WIDTH * 1.5f, RewardConstants::CARD_HEIGHT * 1.5f);
        Vector2 cardPosition = Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 - 40);
        mGame->GetRenderer()->DrawTexture(
            cardPosition,
            cardSize,
            0.0f, Vector3(1,1,1), tex, Vector4::UnitRect, Vector2::Zero
        );

        //renderizar o valor de poder da carta
        if (mRewardCard->IsAvailable())
        {
            RenderCardInfo(mRewardCard);
        }
    }
}

void RewardScene::RenderCardInfo(Card* card, Vector2 customPos, Vector2 customSize) {
    // aumentei um pouco a escala para dar destaque na carta unica antes da selecao
    Vector2 cardSize = Vector2(RewardConstants::CARD_WIDTH * 1.5f, RewardConstants::CARD_HEIGHT * 1.5f);
    if (customSize.x || customSize.y) {
        cardSize.x = customSize.x;
        cardSize.y = customSize.y;
    }

    Vector2 cardPosition = Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2 - 40);
    if (customPos.x || customPos.y) {
        cardPosition.x = customPos.x;
        cardPosition.y = customPos.y;
    }

    // Determinar cor do texto
    Vector3 corTexto = Vector3(1.0f, 1.0f, 1.0f); // Branco

    // Determinar tamanho da fonte baseado no tamanho da carta
    int tamanhoFonte = (cardSize.x > 100.0f) ?
        FontSizes::POWER_LARGE :
        FontSizes::POWER_SMALL;

    // Determinar offset baseado no tamanho da carta
    float offsetBase = (cardSize.x > 100.0f) ?
        Offsets::POWER_TEXT_FROM_BOTTOM_LARGE :
        Offsets::POWER_TEXT_FROM_BOTTOM;

    RenderCardValueText(
        Vector2(cardPosition.x, cardPosition.y + cardSize.y / 2.0f - offsetBase),
        card->GetDamage(),
        corTexto,
        tamanhoFonte
    );

    //se quisermos renderizar mais coisas de texto aq, é só criar mais uma função parecida com o RenderCardValueText
}

void RewardScene::RenderCardValueText(Vector2 pos, int value, Vector3 color, int fontSize)
{
    if (!mGame->GetFont()) return;

    char texto[16];
    snprintf(texto, sizeof(texto), "%d", value);

    Texture* textura = mGame->GetFont()->RenderText(texto, color, fontSize);
    if (textura)
    {
        mGame->GetRenderer()->DrawTexture(
            pos,
            Vector2(textura->GetWidth(), textura->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            textura,
            Vector4::UnitRect,
            Vector2::Zero
        );
        delete textura;
    }
}

void RewardScene::RenderDeckSelection()
{
    Player* player = mGame->GetPlayer();
    if (!player) return;

    std::vector<Card*> deck = player->GetDeck();

    // desenha a carta NOVA pequena no topo para referência
    if (mRewardCard) {
        Texture* newTex = GetCardTexture(mRewardCard->GetType());
        if (newTex) {
            Vector2 cardPosition = Vector2(Game::WINDOW_WIDTH / 2, 100);
            Vector2 cardSize = Vector2(RewardConstants::CARD_WIDTH, RewardConstants::CARD_HEIGHT);

            mGame->GetRenderer()->DrawTexture(
               cardPosition,
               cardSize,
               0.0f, Vector3(1,1,1), newTex, Vector4::UnitRect, Vector2::Zero
           );

            if (mRewardCard->IsAvailable()) {
                RenderCardInfo(mRewardCard, cardPosition, cardSize);
            }
        }
    }

    // desenha o deck em baixo
    float totalWidth = (RewardConstants::CARD_WIDTH * 4) + (RewardConstants::SPACING * 3);
    float startX = (Game::WINDOW_WIDTH - totalWidth) / 2.0f + RewardConstants::CARD_WIDTH / 2.0f;

    for (int i = 0; i < deck.size(); i++) {
        Card* card = deck[i];
        Texture* tex = GetCardTexture(card->GetType());

        float x = startX + (i * (RewardConstants::CARD_WIDTH + RewardConstants::SPACING));
        float y = RewardConstants::DECK_Y;

        // destaque se selecionado
        if (i == mSelectedDeckIndex) {
            y += RewardConstants::SELECTED_OFFSET;
        }

        if (tex) {
            mGame->GetRenderer()->DrawTexture(
                Vector2(x, y),
                Vector2(RewardConstants::CARD_WIDTH, RewardConstants::CARD_HEIGHT),
                0.0f, Vector3(1,1,1), tex, Vector4::UnitRect, Vector2::Zero
            );

            if (card->IsAvailable()) {
                RenderCardInfo(card, Vector2(x, y), Vector2(RewardConstants::CARD_WIDTH, RewardConstants::CARD_HEIGHT));
            }
        }
    }
}

void RewardScene::Render()
{
    // renderizar texto de instrução
    if (mTextTexture) {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 400.0f), // texto mais embaixo
            Vector2(mTextTexture->GetWidth(), mTextTexture->GetHeight()),
            0.0f, Vector3(1.0f, 1.0f, 1.0f), mTextTexture,
            Vector4::UnitRect, Vector2::Zero
        );
    }

    if (mState == SceneState::SHOWING_REWARD && mRewardType == RewardType::CARD) {
        RenderRewardCard();
    }
    else if (mState == SceneState::CHOOSING_REPLACEMENT) {
        RenderDeckSelection();
    }

    RenderFade();
}

void RewardScene::Exit()
{
    mBackgroundTexture = nullptr;
    if (mTextTexture) {
        delete mTextTexture;
        mTextTexture = nullptr;
    }

    // se a carta de recompensa não foi pega (e não é null), deleto para não vazar memória
    if (mRewardCard) {
        delete mRewardCard;
        mRewardCard = nullptr;
    }
}