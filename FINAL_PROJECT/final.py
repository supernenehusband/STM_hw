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

# Start BLE listener once
ble_queue = queue.Queue()
connected_event = threading.Event()
threading.Thread(target=start_ble_listener, args=(
    ble_queue, connected_event), daemon=True).start()

# Run this to restart game


def run_game(is_first_time):
    # Init pygame resources
    screen, clock, font, enemy_sprite, player_sprite, trap_sprite, ufo_sprite, heart_full_image, heart_empty_image, background, emblem_sprite, emblem_gray_sprite, aplus_sprite, connect_bg = init_game()
    wait_font = pygame.font.SysFont(None, 32)

    game_started = False
    ble_connected = False

    if not is_first_time:
        game_started = True
        ble_connected = True

    while not game_started:
        screen.blit(connect_bg, (0, 0))

        if connected_event.is_set():
            ble_connected = True

        if not ble_connected:
            text = wait_font.render(
                "Connecting to BLE device...", True, (255, 255, 255))
            screen.blit(text, (100, 220))
        else:
            text1 = wait_font.render(
                "Connected to BlueNRG!", True, (0, 255, 0))
            text2 = wait_font.render(
                "Shake to start the game", True, (255, 255, 0))
            screen.blit(text1, (120, 180))
            screen.blit(text2, (100, 240))

            try:
                if not ble_queue.empty():
                    cmd = ble_queue.get_nowait()
                    if cmd.startswith(("u", "d", "l", "r", "shake")):
                        game_started = True
            except queue.Empty:
                pass

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        pygame.display.flip()
        clock.tick(30)

    # Game content
    lanes = [200, 300, 400]

    class PlayerModified(Player):
        def __init__(self, lanes, sprite):
            super().__init__(lanes, sprite)
            self.target_x = self.x

        def move_with_strength(self, direction, strength):
            norm = max(0.0, min(1.0, (strength - 750) / (2000 - 750)))
            move_distance = norm * 100
            if direction == 'l':
                self.target_x = max(self.lanes[0], self.x - move_distance)
            elif direction == 'r':
                self.target_x = min(self.lanes[-1], self.x + move_distance)

        def update(self):
            super().update()
            if abs(self.x - self.target_x) > 1:
                self.x += (self.target_x - self.x) * 0.2
            else:
                self.x = self.target_x

    player = PlayerModified(lanes, player_sprite)
    enemy = Enemy(lanes, enemy_sprite, trap_sprite)
    trap_manager = TrapUFOManager(
        lanes, trap_sprite, ufo_sprite, emblem_sprite, aplus_sprite)
    tsmc_sprite = pygame.image.load(
        "FINAL_PROJECT/pic/tsmc.png").convert_alpha()
    tsmc_sprite = pygame.transform.scale(tsmc_sprite, (40, 40))
    tsmc = TSMCPowerUp(lanes, tsmc_sprite)

    lives = 3
    score = 0
    score_timer = 0
    double_score_timer = 0
    multiplier = 1.0

    hud = HUD(font, heart_full_image, heart_empty_image,
              emblem_sprite, emblem_gray_sprite, lives)
    shake = ScreenShake(duration=10, intensity=5)
    ink_overlay = InkOverlayEffect(screen.get_size())

    last_ink_time = pygame.time.get_ticks()
    next_ink_delay = random.randint(30000, 40000)

    running = True
    score_saved = False
    while running:
        if lives <= 0:
            if not score_saved:
                save_score(score)
                score_saved = True
            screen.fill((0, 0, 0))
            text1 = font.render("Game Over", True, (255, 255, 255))
            text2 = font.render(f"Score: {score}", True, (255, 255, 255))
            text3 = font.render("Shake to Restart",
                                True, (255, 255, 255))

            text1_rect = text1.get_rect(center=(300, 50))
            text2_rect = text2.get_rect(center=(300, 100))
            text3_rect = text3.get_rect(center=(300, 150))

            screen.blit(text1, text1_rect)
            screen.blit(text2, text2_rect)
            screen.blit(text3, text3_rect)

            # 顯示前 5 名
            # save_score(score)
            scores = get_scores()
            for i, s in enumerate(scores[:5], start=1):
                rank_text = font.render(f"{i}. {s}", True, (255, 255, 255))
                rank_rect = rank_text.get_rect(center=(300, 150 + i * 40))
                screen.blit(rank_text, rank_rect)

            pygame.display.flip()
            try:
                while not ble_queue.empty():
                    cmd = ble_queue.get_nowait()
                    print(f"[BLE Received] {cmd}")
                    if cmd == "shake":
                        return False
            except queue.Empty:
                pass

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    # running = True
                    return True
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER:
                        # pygame.quit()
                        # os.execv(sys.executable, [
                        #          sys.executable, 'FINAL_PROJECT/final.py'])
                        # running = True
                        # lives = 3
                        # score = 0
                        # score_timer = 0
                        # double_score_timer = 0
                        # multiplier = 1.0
                        # last_ink_time = pygame.time.get_ticks()
                        # next_ink_delay = random.randint(30000, 40000)
                        return False
                    if event.key == pygame.K_ESCAPE:
                        # running = False
                        return True
            continue
        dt = clock.tick(30)
        score_timer += dt
        if score_timer >= 1000:
            score += 2 if double_score_timer > 0 else 1
            score_timer = 0

        current_time = pygame.time.get_ticks()
        if current_time - last_ink_time > next_ink_delay:
            ink_overlay.trigger()
            last_ink_time = current_time
            next_ink_delay = random.randint(30000, 40000)
            multiplier *= 1.1

        enemy.speed = 5 * multiplier
        trap_manager.set_speed(3 * multiplier)
        tsmc.speed = 2 * multiplier

        shake.update()
        offset_x, offset_y = shake.get_offset()
        ink_overlay.update()

        game_surface = pygame.Surface(screen.get_size())
        game_surface.blit(background, (0, 0))
        pygame.draw.rect(game_surface, (100, 100, 100), (150, 0, 300, 400))
        pygame.draw.line(game_surface, (255, 255, 255),
                         (250, 0), (250, 400), 3)
        pygame.draw.line(game_surface, (255, 255, 255),
                         (350, 0), (350, 400), 3)
        for x in range(150, 450, 20):
            color = (255, 255, 255) if (x // 20) % 2 == 0 else (0, 0, 0)
            pygame.draw.rect(game_surface, color, (x, 315, 20, 5))

        try:
            while not ble_queue.empty():
                cmd = ble_queue.get_nowait()
                print(f"[BLE Received] {cmd}")
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
                elif cmd.startswith("u"):
                    try:
                        _ = int(cmd[1:])
                        player.jump()
                    except:
                        pass
                elif cmd.startswith("d"):
                    try:
                        _ = int(cmd[1:])
                        player.slide()
                    except:
                        pass
                elif cmd.startswith(("l", "r")):
                    direction = cmd[0]
                    try:
                        strength = int(cmd[1:])
                        player.move_with_strength(direction, strength)
                    except:
                        pass
        except queue.Empty:
            pass

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
                elif event.key == pygame.K_LEFT:
                    ble_queue.put(f"l{random.randint(750, 2000)}")
                elif event.key == pygame.K_RIGHT:
                    ble_queue.put(f"r{random.randint(750, 2000)}")
                elif event.key == pygame.K_UP:
                    ble_queue.put(f"u{random.randint(750, 2000)}")
                elif event.key == pygame.K_DOWN:
                    ble_queue.put(f"d{random.randint(750, 2000)}")
                elif event.key == pygame.K_s:
                    ink_overlay.receive_shake()

        player.update()
        if double_score_timer > 0:
            double_score_timer -= 1
        enemy.update()
        trap_manager.update()
        tsmc.update()
        hud.lives = lives

        player_rect = player.get_rect()
        if not tsmc.is_invincible():
            if enemy.active and player_rect.colliderect(enemy.get_rect()):
                if enemy.height == 'down' and not player.is_jumping:
                    lives -= 1
                    enemy.active = False
                    shake.start()
                if enemy.height == 'up' and not player.is_sliding:
                    lives -= 1
                    enemy.active = False
                    shake.start()
            if trap_manager.get_collided(player_rect, player.is_jumping, player.is_sliding):
                lives -= 1
                shake.start()

        if trap_manager.collected_emblem(player_rect):
            if hud.add_emblem():
                score += 100 if double_score_timer > 0 else 50
                trap_manager.aplus.start()
                ink_overlay.trigger()

        if tsmc.check_collision(player_rect):
            double_score_timer = 300

            clock.tick(30)
            # return score

        player.draw(game_surface)
        enemy.draw(game_surface)
        trap_manager.draw(game_surface)
        tsmc.draw(game_surface)
        hud.draw_score(game_surface, score)
        hud.draw_lives(game_surface, lives)
        hud.draw_emblems(game_surface)
        ink_overlay.draw(game_surface)

        if tsmc.is_invincible():
            inv_text = font.render(
                f"Invincible: {tsmc.invincible_timer // 30}s", True, (255, 255, 0))
            game_surface.blit(inv_text, (10, 40))
        if double_score_timer > 0:
            bonus_text = font.render(
                f"Double Score: {double_score_timer // 30}s", True, (0, 255, 0))
            game_surface.blit(bonus_text, (10, 70))

        screen.blit(game_surface, (offset_x, offset_y))
        pygame.display.flip()


def save_score(score):
    with open("FINAL_PROJECT/scores.txt", "a") as f:
        f.write(f"{score}\n")


def get_scores():
    try:
        with open("FINAL_PROJECT/scores.txt", "r") as f:
            scores = [int(line.strip()) for line in f]
        return sorted(scores, reverse=True)
    except FileNotFoundError:
        return []


game_over = False
# Main loop to allow restart without BLE reconnect
if __name__ == "__main__":
    is_first_time = True
    while True:

        game_over = run_game(is_first_time)
        is_first_time = False
        if game_over:
            break

        # print(f"Game Over! Final Score: {score}")

        # # Simple game over screen with restart
        # pygame.init()
        # screen = pygame.display.set_mode((600, 400))
        # font = pygame.font.SysFont(None, 48)
        # screen.fill((0, 0, 0))
        # text1 = font.render(f"Game Over! Score: {score}", True, (255, 0, 0))
        # text2 = font.render(
        #     "Press R to restart or ESC to quit", True, (255, 255, 255))
        # screen.blit(text1, (100, 150))
        # screen.blit(text2, (50, 220))
        # pygame.display.flip()

        # waiting = True
        # while waiting:
        #     for event in pygame.event.get():
        #         if event.type == pygame.QUIT:
        #             pygame.quit()
        #             sys.exit()
        #         if event.type == pygame.KEYDOWN:
        #             if event.key == pygame.K_r:
        #                 waiting = False
        #             if event.key == pygame.K_ESCAPE:
        #                 pygame.quit()
        #                 sys.exit()
