import pygame
import random
import time
from emblem import Emblem  
from aplus_effect import APlusEffect  

# 小怪類別（pic1）
class Enemy:
    def __init__(self, lanes, enemy_sprite, trap_sprite):
        self.lanes = lanes
        self.enemy_sprite = enemy_sprite
        self.trap_sprite = trap_sprite
        self.speed = 5
        self.last_spawn_time = time.time()
        self.spawn_interval = 5
        self.active = False
        self.reset()

    def reset(self):
        self.lane = random.choice(self.lanes)
        self.y = -60
        self.height = random.choice(['up', 'down'])
        self.active = True

    def update(self):
        current_time = time.time()
        if not self.active and current_time - self.last_spawn_time >= self.spawn_interval:
            self.reset()
            self.last_spawn_time = current_time

        if self.active:
            self.y += self.speed
            if self.y > 400:
                self.active = False

    def draw(self, screen):
        if self.active:
            width = self.enemy_sprite.get_width()
            height = self.enemy_sprite.get_height()
            screen.blit(self.enemy_sprite, (self.lane - width // 2, self.y - height // 2))
            pygame.draw.rect(screen, (255, 0, 0), self.get_rect(), 2)


    def get_rect(self):
        width = self.enemy_sprite.get_width()
        height = self.enemy_sprite.get_height()
        return pygame.Rect(self.lane - width // 2, self.y - height // 2, width, height)


# 單個陷阱（pic2），位於單一路徑
class Trap:
    def __init__(self, lane, trap_sprite):
        self.lane = lane  # 跑道位置
        self.trap_sprite = trap_sprite
        self.y = -60
        self.speed = 3
        self.active = False
        self.last_spawn_time = time.time()
        self.spawn_interval = random.uniform(5, 7)

    def reset(self):
        self.y = -60
        self.active = True
        self.last_spawn_time = time.time()
        self.spawn_interval = random.uniform(5, 7)

    def update(self):
        current_time = time.time()
        if not self.active and current_time - self.last_spawn_time >= self.spawn_interval:
            self.reset()

        if self.active:
            self.y += self.speed
            if self.y > 400:
                self.active = False

    def draw(self, screen):
        if self.active:
            width = self.trap_sprite.get_width()
            height = self.trap_sprite.get_height()
            screen.blit(self.trap_sprite, (self.lane - width // 2, self.y - height // 2))
            pygame.draw.rect(screen, (0, 255, 0), self.get_bottom_rect(), 2)


    def get_rect(self):
        width = self.trap_sprite.get_width()
        height = self.trap_sprite.get_height()
        return pygame.Rect(self.lane - width // 2, self.y - height // 2, width, height)

    def get_bottom_rect(self):
        rect = self.get_rect()
        new_height = 12
        start_from_bottom = 10
        return pygame.Rect(rect.left, rect.bottom - start_from_bottom - new_height, rect.width, new_height)

class UFO:
    def __init__(self, lane, ufo_sprite):
        self.lane = lane
        self.ufo_sprite = ufo_sprite
        self.y = -60
        self.speed = 3
        self.active = False

    def reset(self):
        self.y = -60
        self.active = True

    def update(self):
        if self.active:
            self.y += self.speed
            if self.y > 400:
                self.active = False

    def draw(self, screen):
        if self.active:
            width = self.ufo_sprite.get_width()
            height = self.ufo_sprite.get_height()
            screen.blit(self.ufo_sprite, (self.lane - width // 2, self.y - height // 2))
            pygame.draw.rect(screen, (0, 0, 255), self.get_bottom_rect(), 2)  # 改這裡

    def get_rect(self):
        width = self.ufo_sprite.get_width()
        height = self.ufo_sprite.get_height()
        return pygame.Rect(self.lane - width // 2, self.y - height // 2, width, height)
    
    def get_bottom_rect(self):
        rect = self.get_rect()
        new_height = 12 
        start_from_bottom = 10
        return pygame.Rect(rect.left, rect.bottom - start_from_bottom - new_height, rect.width, new_height)


class TrapUFOManager:
    def __init__(self, lanes, trap_sprite, ufo_sprite, emblem_sprite, aplus_sprite):
        self.traps = [Trap(lane, trap_sprite) for lane in lanes]
        self.ufos = [UFO(lane, ufo_sprite) for lane in lanes]
        self.last_spawn_time = time.time()
        self.spawn_interval = 5
        self.mode = 'trap'  # 'trap' or 'ufo'

        self.lanes = lanes
        self.emblem = None
        self.emblem_sprite = emblem_sprite
        self.emblem_last_spawn = time.time()
        self.emblem_count = 0
        self.aplus = APlusEffect(aplus_sprite)

    def update(self):
        current_time = time.time()
        if current_time - self.last_spawn_time >= self.spawn_interval:
            self.mode = random.choice(['trap', 'ufo'])
            for obj in (self.traps if self.mode == 'trap' else self.ufos):
                obj.reset()
            self.last_spawn_time = current_time

        for obj in (self.traps if self.mode == 'trap' else self.ufos):
            obj.update()

        # ✅ 校徽碎片生成邏輯（每 10 秒）
        if (self.emblem is None or not self.emblem.active) and (current_time - self.emblem_last_spawn > 10):
            self.emblem = Emblem(self.emblem_sprite, self.lanes)
            self.emblem_last_spawn = current_time

        if self.emblem and self.emblem.active:
            self.emblem.update()

        # ✅ A+ 特效更新
        self.aplus.update()

    def draw(self, screen):
        for obj in (self.traps if self.mode == 'trap' else self.ufos):
            obj.draw(screen)
        if self.emblem and self.emblem.active:
            self.emblem.draw(screen)

        self.aplus.draw(screen)

    def get_collided(self, player_rect, is_jumping, is_sliding):
        for obj in (self.traps if self.mode == 'trap' else self.ufos):
            target_rect = obj.get_bottom_rect()
            if obj.active and player_rect.colliderect(target_rect):
                if self.mode == 'trap' and not is_jumping:
                    obj.active = False
                    return True
                if self.mode == 'ufo' and not is_sliding:
                    obj.active = False
                    return True
        return False

    def collected_emblem(self, player_rect):
        if self.emblem and self.emblem.active and player_rect.colliderect(self.emblem.get_rect()):
            self.emblem.active = False
            return True
        return False

    def set_speed(self, speed):
        for trap in self.traps:
            trap.speed = speed
        for ufo in self.ufos:
            ufo.speed = speed

class TSMCPowerUp:
    def __init__(self, lanes, sprite):
        self.lanes = lanes
        self.sprite = sprite
        self.x = 0
        self.y = -60
        self.active = False
        self.last_spawn_time = time.time()
        self.spawn_interval = random.uniform(25, 30)
        self.invincible_timer = 0

    def update(self):
        current_time = time.time()
        if not self.active and current_time - self.last_spawn_time >= self.spawn_interval:
            self.x = random.choice(self.lanes)
            self.y = -40
            self.active = True
            self.last_spawn_time = current_time

        if self.active:
            self.y += 2
            if self.y > 400:
                self.active = False

        if self.invincible_timer > 0:
            self.invincible_timer -= 1

    def check_collision(self, player_rect):
        if self.active:
            rect = pygame.Rect(self.x - 20, self.y - 20, 40, 40)
            if player_rect.colliderect(rect):
                self.active = False
                self.invincible_timer = 300  # 10 秒無敵
                return True
        return False

    def is_invincible(self):
        return self.invincible_timer > 0

    def draw(self, screen):
        if self.active:
            screen.blit(self.sprite, (self.x - 20, self.y - 20))