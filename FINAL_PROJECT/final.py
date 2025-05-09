import pygame
from enemy import Enemy, TrapUFOManager
from init_game import init_game
from player import Player
import sys
import os
from hud import HUD
from shake import ScreenShake

# 初始化所有資源（圖片、音效、螢幕等）
screen, clock, font, enemy_sprite, player_sprite, trap_sprite, ufo_sprite, heart_full_image, heart_empty_image = init_game()

# 跑道位置
lanes = [200, 300, 400]

# 初始化遊戲物件
player = Player(lanes, player_sprite)
enemy = Enemy(lanes, enemy_sprite, trap_sprite)
trap_manager = TrapUFOManager(lanes, trap_sprite, ufo_sprite)

lives = 3
score = 0
score_timer = 0

# 初始化 HUD 與螢幕震動
hud = HUD(font, heart_full_image, heart_empty_image, lives)
shake = ScreenShake(duration=10, intensity=5)

running = True
while running:
    dt = clock.tick(30)
    score_timer += dt
    if score_timer >= 1000:
        score += 1
        score_timer = 0

    # 更新震動效果
    shake.update()
    offset_x, offset_y = shake.get_offset()

    # 建立畫布並畫出背景與跑道
    game_surface = pygame.Surface(screen.get_size())
    game_surface.fill((30, 30, 30))
    pygame.draw.rect(game_surface, (100, 100, 100), (150, 0, 300, 400))
    pygame.draw.line(game_surface, (255, 255, 255), (250, 0), (250, 400), 3)
    pygame.draw.line(game_surface, (255, 255, 255), (350, 0), (350, 400), 3)
    for x in range(150, 450, 20):
        color = (255, 255, 255) if (x // 20) % 2 == 0 else (0, 0, 0)
        pygame.draw.rect(game_surface, color, (x, 315, 20, 5))

    # 處理鍵盤與退出事件
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
            if event.key == pygame.K_LEFT:
                player.move_left()
            if event.key == pygame.K_RIGHT:
                player.move_right()
            if event.key == pygame.K_UP:
                player.jump()
            if event.key == pygame.K_DOWN:
                player.slide()

    # 更新遊戲邏輯
    player.update()
    enemy.update()
    trap_manager.update()
    hud.lives = lives

    player_rect = player.get_rect()
    enemy_rect = enemy.get_rect()

    # 判斷小怪碰撞（需要跳或滑）
    if enemy.active and player_rect.colliderect(enemy.get_rect()):
        if enemy.height == 'down' and not player.is_jumping:
            lives -= 1
            enemy.active = False
            shake.start()
        if enemy.height == 'up' and not player.is_sliding:
            lives -= 1
            enemy.active = False
            shake.start()

    # 判斷是否踩到陷阱
    if trap_manager.get_collided(player_rect, player.is_jumping, player.is_sliding):
        lives -= 1
        shake.start()

    # 遊戲結束條件
    if lives <= 0:
        pygame.quit()
        os.execv(sys.executable, [sys.executable, 'FINAL_PROJECT/game_over.py', str(score)])

    # 畫出所有遊戲元素
    player.draw(game_surface)
    enemy.draw(game_surface)
    trap_manager.draw(game_surface)
    hud.draw_score(game_surface, score)
    hud.draw_lives(game_surface, lives)
    pygame.draw.rect(game_surface, (255, 0, 0), player_rect, 2)
    if enemy.active:
        pygame.draw.rect(game_surface, (0, 255, 0), enemy_rect, 2)

    # 顯示畫面與震動偏移
    screen.blit(game_surface, (offset_x, offset_y))
    pygame.display.flip()

pygame.quit()
sys.exit()
