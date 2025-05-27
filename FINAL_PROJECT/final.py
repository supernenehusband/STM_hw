import pygame
from enemy import Enemy, TrapUFOManager, TSMCPowerUp
from init_game import init_game
from player import Player
import sys
import os
from hud import HUD
from shake import ScreenShake
from ble_listener import start_ble_listener
import threading
import queue
from ink_overlay import InkOverlayEffect
import random


# 初始化所有資源（圖片、音效、螢幕等）
screen, clock, font, enemy_sprite, player_sprite, trap_sprite, ufo_sprite, heart_full_image, heart_empty_image, background, emblem_sprite, emblem_gray_sprite, aplus_sprite, connect_bg = init_game()
wait_font = pygame.font.SysFont(None, 32)
game_started = False
ble_connected = False
ble_queue = queue.Queue()
connected_event = threading.Event()
threading.Thread(target=start_ble_listener, args=(
    ble_queue, connected_event), daemon=True).start()

while not game_started:
    screen.blit(connect_bg, (0, 0))

    # 如果 BLE 傳入任何訊息，視為已連線
    # if not ble_connected and not ble_queue.empty():
    #     peek = ble_queue.queue[0]
    #     if peek:
    #         ble_connected = True

    if connected_event.is_set():
        ble_connected = True

    if not ble_connected:
        text = wait_font.render(
            "Connecting to BLE device...", True, (255, 255, 255))
        screen.blit(text, (100, 220))
    else:
        text1 = wait_font.render("Connected to BlueNRG!", True, (0, 255, 0))
        text2 = wait_font.render(
            "Shake to start the game", True, (255, 255, 0))
        screen.blit(text1, (120, 180))
        screen.blit(text2, (100, 240))

        try:
            if not ble_queue.empty():
                cmd = ble_queue.get_nowait()
                if cmd in ("up", "down", "left", "right"):
                    game_started = True
        except queue.Empty:
            pass

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    pygame.display.flip()
    clock.tick(30)

# 跑道位置
lanes = [200, 300, 400]

# 初始化遊戲物件
player = Player(lanes, player_sprite)
enemy = Enemy(lanes, enemy_sprite, trap_sprite)
trap_manager = TrapUFOManager(
    lanes, trap_sprite, ufo_sprite, emblem_sprite, aplus_sprite
)
tsmc_sprite = pygame.image.load("FINAL_PROJECT/pic/tsmc.png").convert_alpha()
tsmc_sprite = pygame.transform.scale(tsmc_sprite, (40, 40))
tsmc = TSMCPowerUp(lanes, tsmc_sprite)

lives = 3
score = 0
score_timer = 0
double_score_timer = 0
# 新增 multiplier 初始值
multiplier = 1.0

# 初始化 HUD 與螢幕震動
hud = HUD(font, heart_full_image, heart_empty_image,
          emblem_sprite, emblem_gray_sprite, lives)
shake = ScreenShake(duration=10, intensity=5)
ink_overlay = InkOverlayEffect(screen.get_size())

# 新增：初始化墨魚汁自動觸發計時
last_ink_time = pygame.time.get_ticks()
next_ink_delay = random.randint(30000, 40000)

running = True
while running:
    dt = clock.tick(30)
    score_timer += dt
    if score_timer >= 1000:
        if double_score_timer > 0:
            score += 2
        else:
            score += 1
        score_timer = 0

    # 每隔固定時間自動觸發墨魚汁效果
    current_time = pygame.time.get_ticks()
    if current_time - last_ink_time > next_ink_delay:
        ink_overlay.trigger()
        last_ink_time = current_time
        next_ink_delay = random.randint(30000, 40000)
        multiplier *= 1.1  # 每次觸發墨魚汁就加速

    # 速度調整區
    base_enemy_speed = 5
    base_trap_speed = 3
    base_tsmc_speed = 2
    # 移除每 100 分加速的 multiplier 計算
    # multiplier = 1.1 ** (score // 100)
    enemy.speed = base_enemy_speed * multiplier
    trap_manager.set_speed(base_trap_speed * multiplier)
    tsmc.speed = base_tsmc_speed * multiplier

    # 更新震動效果
    shake.update()
    offset_x, offset_y = shake.get_offset()
    ink_overlay.update()

    # 建立畫布並畫出背景與跑道
    game_surface = pygame.Surface(screen.get_size())
    game_surface.blit(background, (0, 0))

    pygame.draw.rect(game_surface, (100, 100, 100), (150, 0, 300, 400))

    pygame.draw.line(game_surface, (255, 255, 255), (250, 0), (250, 400), 3)
    pygame.draw.line(game_surface, (255, 255, 255), (350, 0), (350, 400), 3)
    for x in range(150, 450, 20):
        color = (255, 255, 255) if (x // 20) % 2 == 0 else (0, 0, 0)
        pygame.draw.rect(game_surface, color, (x, 315, 20, 5))

    # BLE 控制輸入處理
    try:
        while not ble_queue.empty():
            cmd = ble_queue.get_nowait()
            if cmd == "up":
                player.jump()
            elif cmd == "down":
                player.slide()
            elif cmd == "left":
                player.move_left()
            elif cmd == "right":
                player.move_right()
            elif cmd == "shake":
                ink_overlay.receive_shake()
    except queue.Empty:
        pass

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
            if event.key == pygame.K_s:
                ink_overlay.receive_shake()

    # 更新遊戲邏輯
    player.update()
    # 雙倍計時獨立倒數
    if double_score_timer > 0:
        double_score_timer -= 1
    enemy.update()
    trap_manager.update()
    tsmc.update()
    hud.lives = lives

    player_rect = player.get_rect()
    enemy_rect = enemy.get_rect()

    if not tsmc.is_invincible():
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

    # 校徽撿取檢查與加分
    if trap_manager.collected_emblem(player_rect):
        if hud.add_emblem():
            if double_score_timer > 0:
                score += 100
            else:
                score += 50
            trap_manager.aplus.start()  # ✅ 啟動 A+ 特效動畫
            ink_overlay.trigger()

    # 碰撞處理區新增
    if tsmc.check_collision(player_rect):
        double_score_timer = 300  # 約 10 秒雙倍分數

    # 遊戲結束條件
    if lives <= 0:
        pygame.quit()
        os.execv(sys.executable, [sys.executable,
                 'FINAL_PROJECT/game_over.py', str(score)])

    # 畫出所有遊戲元素
    player.draw(game_surface)
    enemy.draw(game_surface)
    trap_manager.draw(game_surface)
    tsmc.draw(game_surface)
    hud.draw_score(game_surface, score)
    hud.draw_lives(game_surface, lives)
    hud.draw_emblems(game_surface)
    # 最後畫上墨魚汁效果（蓋在遊戲畫面上）
    ink_overlay.draw(game_surface)

    # Show TSMC invincible/double score timer hints (English)
    if tsmc.is_invincible():
        inv_sec = tsmc.invincible_timer // 30
        inv_text = font.render(f"Invincible: {inv_sec}s", True, (255, 255, 0))
        game_surface.blit(inv_text, (10, 40))

    if double_score_timer > 0:
        bonus_sec = double_score_timer // 30
        bonus_text = font.render(
            f"Double Score: {bonus_sec}s", True, (0, 255, 0))
        game_surface.blit(bonus_text, (10, 70))

    # 顯示畫面與震動偏移
    screen.blit(game_surface, (offset_x, offset_y))
    pygame.display.flip()

pygame.quit()
sys.exit()
