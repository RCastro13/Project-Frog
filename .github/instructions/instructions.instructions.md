DEFINIÇÃO DE SPRINTS
Sprint 1 (CRÍTICO - Vertical Slice)
Sistema de Cenas (0.2) - Lott
Menu Principal básico (5.1) - Kayque
Combate básico (1.1, 1.2, 1.3 básico) - Facundo
Assets temporários para testar - Rubens
Mapa básico - Rubens
META: Conseguir iniciar jogo, ir para um combate e jogar 1 turno
Sprint 2 (Core Gameplay)
Completar sistema de combate (1.3 completo)
Sistema de cartas (2.1, 2.2)
Inimigos básicos (3.1 parcial, 3.2 básico)
META: Completar um combate do início ao fim
Sprint 3 (Loop do Jogo)
Sistema de mapa (4.1, 4.2, 4.3 básico)
Fluxo completo (5.2, 5.3 básico)
Múltiplos inimigos
META: Jogar uma run completa do início ao fim
Sprint 4 (Conteúdo)
Mais inimigos e balanceamento (3.1 completo, 3.2 completo)
Mais cartas (2.1 completo)
Boss e eventos (4.3 completo)
META: Jogo completo e jogável
Sprint 5 (Arte & Áudio)
Substituir assets temporários (6.1, 6.2, 6.3)
Adicionar música e SFX (7.1, 7.2)
META: Jogo com identidade visual/sonora
Sprint 6 (Polimento)
Balanceamento final (8.1)
Juice e feedback (8.2)
QoL features (8.3)
Testes e bugs
META: Jogo pronto para apresentação
DEFINIÇÃO DO PROJETO
0.1 Arquitetura Core (COMPLETO)
✅ Sistema de Game Loop
✅ Sistema de Atores e Componentes
✅ Sistema de Renderização (OpenGL/SDL)
✅ Sistema de Input
✅ Sistema de UI (UIScreen, UIButton, UIText, UIImage)
0.2 Sistema de Cenas (PENDENTE)
Implementar GameScene completo (atualmente comentado)
Criar enum de estados: MAIN_MENU, MAP, COMBAT, GAME_OVER, VICTORY
Implementar transições entre cenas
Sistema de carregamento/descarregamento de dados por cena
FASE 1: SISTEMA DE COMBATE
1.1 Classes Base do Combate
Criar classe Card (ataque)
Atributos: nome, tipo (Fogo/Água/Planta/Neutro), valorFixo, dado (D4/D6/D8/D12/D20), cooldown
Métodos: CalcularDano(), GetTipo(), IsDisponivel()
Criar classe Combatant (base para Player e Enemy)
Atributos: nome, vida, vidaMaxima, deck (4 cartas), cooldowns ativos
Métodos: TakeDamage(), Heal(), SelectCard(), GetAvailableCards()
Criar classe Player herda de Combatant
Adicionar inventário, moedas, cartas desbloqueadas
Criar classe Enemy herda de Combatant
Implementar IA para seleção de cartas
Diferentes tipos de inimigos com decks diferentes
Criar classe CombatManager
Gerenciar turnos
Resolver combate (comparar cartas, calcular dano)
Detectar vitória/derrota
1.2 Lógica de Combate
Implementar sistema de tipos (Fogo > Planta > Água > Fogo)
Implementar comparação de cartas do mesmo tipo
Implementar rolagem de dados para dano variável (opcional)
Implementar sistema de cooldown
Implementar opção de defesa (escudo)
Implementar sistema de turnos


1.3 Cena de Combate (CombatScene)
Criar classe CombatScene herda de GameScene
Posicionar Player à esquerda
Posicionar Enemy à direita
Exibir background de combate
Exibir 4 cartas do player na parte inferior
Exibir barras de vida
Implementar seleção de carta com WASD/Setas + Enter
Implementar animação de ataque
Implementar efeitos visuais (dano, cura, escudo)
Implementar feedback visual de vantagem de tipo
Implementar tela de vitória do combate
Implementar tela de derrota do combate
FASE 2: SISTEMA DE CARTAS & PROGRESSÃO
2.1 Biblioteca de Cartas
Definir conjunto inicial de cartas do player (4 básicas)
Criar 15-20 cartas diferentes balanceadas
Distribuir cartas por tipo: Fogo, Água, Planta, Neutro
Criar arquivo JSON com dados das cartas
2.2 Deck Building
Criar tela de escolha de carta (após vitória)
Implementar sistema de troca/upgrade de cartas
Limitar deck a 4 cartas
Salvar deck atual do player
FASE 3: INIMIGOS & BALANCEAMENTO
3.1 Tipos de Inimigos
Criar 3-5 inimigos normais
Criar 1 boss final
Definir vida, deck e comportamento de cada
3.2 IA dos Inimigos
Implementar seleção aleatória básica
Implementar IA intermediária (considera tipo do player) (aleatorio primeiro - para simplificar)
Implementar IA avançada para elites/boss (aleatorio primeiro - para simplificar)
Balancear dificuldade progressiva


FASE 4: SISTEMA DE MAPA
4.1 Classes do Mapa
Criar classe MapNode
Tipos: COMBAT, EVENT, SHOP, REST, BOSS
Estado: LOCKED, AVAILABLE, COMPLETED
Criar classe MapPath (conexão entre nós)
Criar classe MapGenerator
Gerar árvore de nós estilo Slay the Spire
3-4 colunas de nós até boss
Garantir pelo menos 3-5 combates
4.2 Cena do Mapa (MapScene)
Criar classe MapScene herda de GameScene
Renderizar background (pântano/fantasia)
Renderizar todos os nós com ícones
Renderizar caminhos entre nós
Destacar nós disponíveis
Implementar seleção com WASD/Setas
Implementar confirmação com Enter
Animação de movimento do player no mapa
Transição para combate ao selecionar nó
4.3 Tipos de Encontros
Implementar nó de COMBATE normal
Implementar nó de LOJA (comprar cartas)
Implementar nó de EVENTO (escolhas especiais)
(OPCIONAL) Implementar nó de DESCANSO (recuperar vida)
Implementar nó de BOSS
FASE 5: MENU & FLUXO DO JOGO
5.1 Menu Principal
Criar classe MainMenuScene herda de GameScene
Adicionar background do menu
Botão "NOVO JOGO" → inicia MapScene
Botão "SAIR" → fecha o jogo
(OPCIONAL) Botão "OPÇÕES" → volume, controles
(OPCIONAL) Título animado do jogo



5.2 Telas de Fim
Criar tela de GAME OVER
Mostrar causa da derrota
Botão voltar ao menu
Criar tela de VITÓRIA
Mensagem de parabenização
Estatísticas da run (turnos, dano, etc)
Botão voltar ao menu
5.3 Sistema de Recompensas
Tela de recompensa pós-combate
Escolher nova carta OU
Melhorar carta existente OU
Ganhar moedas
Sistema de moedas
Loja de cartas
FASE 6: ARTE & ASSETS
6.1 Sprites de Personagens
Sprite do Player (sapo)
Sprites de 3-5 inimigos normais
Sprite do boss final
Animações idle
Animações de ataque
Animações de dano/morte
6.2 Sprites de UI
Design das cartas (4 variações de tipo)
Ícones de tipos (Fogo, Água, Planta, Neutro)
Ícones do mapa (combate, elite, loja, evento, boss)
Barra de vida
Indicadores de cooldown
Cursor/seleção
6.3 Backgrounds
Background do menu principal
Background de combate (2-3 variações)
Background do mapa
Tela de vitória
Tela de game over


6.4 Efeitos Visuais
Efeito de ataque de fogo
Efeito de ataque de água
Efeito de ataque de planta
Efeito de ataque neutro
Efeito de escudo/defesa
Partículas de dano
Transições de cena
FASE 7: ÁUDIO
7.1 Música
Música do menu principal
Música de combate normal (temática aventuresca)
Música de combate com boss (temática épica)
Música do mapa (temática misteriosa)
Música de vitória
Música de game over
7.2 Efeitos Sonoros
Som de seleção de UI
Som de confirmação
Som de carta sendo jogada
Som de ataque por tipo (4 sons)
Som de dano recebido
Som de defesa/escudo
Som de vitória de turno
Som de derrota de turno
Som de morte do inimigo
Sons ambiente (pântano, natureza)
FASE 8: POLIMENTO & EXTRAS
8.1 Balanceamento
Testar e balancear todas as cartas
Ajustar vida dos inimigos
Ajustar dificuldade progressiva
Testar diferentes estratégias
8.2 Feedback & Juice
Screenshake em ataques poderosos
Slow motion em momentos críticos
Partículas e efeitos visuais extras
Animações de transição suaves
Tooltips explicativos
8.3 Qualidade de Vida
Tutorial/explicação de mecânicas
Indicador de vantagem de tipo
Calculadora de dano antes de jogar
Histórico de turnos
Opções de volume
Confirmação antes de decisões importantes
8.4 (OPCIONAL) Conteúdo Extra
Múltiplos personagens jogáveis
Mais cartas e inimigos
Sistema de conquistas
Estatísticas globais
Diferentes finais
