#pragma once
#include "../Math.h"
#include <map>

class Game;
class Card;
class Texture;
class Renderer;
enum class AttackType;

// Classe auxiliar para renderização de elementos de combate
class CombatRenderer
{
public:
    CombatRenderer(Game* game);
    ~CombatRenderer();

    // Renderiza uma carta individual
    void RenderizarCarta(
        Vector2 posicao,
        Vector2 tamanho,
        Card* carta,
        bool selecionada,
        float brilho,
        const std::map<AttackType, Texture*>& texturasAtivas,
        const std::map<AttackType, Texture*>& texturasCooldown,
        Texture* iconeTempo
    );

    // Renderiza barra de HP
    void RenderizarBarraHP(Vector2 posicao, int hpAtual, int hpMax, bool isInimigo);

    // Renderiza número de poder de uma carta
    void RenderizarTextoPoder(Vector2 posicao, int poder, Vector3 cor, int tamanhoFonte);

    // Renderiza textura simples com tint e brilho
    void RenderizarTexturaSimples(Vector2 posicao, Vector2 tamanho, Texture* textura, float brilho = 1.0f);

    // Renderiza moldura de vencedor
    void RenderizarMolduraVencedor(Vector2 posicao, Vector2 tamanhoCard, Texture* moldura);

    // Renderiza ícone de cooldown
    void RenderizarIconeCooldown(Vector2 posicao, float alturaCard, Texture* icone);

private:
    Game* mGame;

    // Métodos auxiliares para barra de HP
    Vector3 CalcularCorHP(float percentual);
    void RenderizarBarraFundo(Vector2 posicao, float largura, float altura);
    void RenderizarBarraPreenchida(Vector2 posicao, float largura, float altura, Vector3 cor);
    void RenderizarBorda(Vector2 posicao, float largura, float altura, float espessura);
    void RenderizarTextoHP(Vector2 posicao, int hp, Vector3 cor);
};

