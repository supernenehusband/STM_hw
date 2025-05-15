import pygame
import random

class Emblem:
    def __init__(self, image, lanes):
        self.image = image
        self.x = random.choice(lanes)
        self.y = -30
        self.speed = 3
        self.active = True

    def update(self):
        if self.active:
            self.y += self.speed
            if self.y > 400:
                self.active = False  # 超出畫面則視為消失

    def draw(self, screen):
        if self.active:
            screen.blit(self.image, (self.x - 15, self.y - 15))  # 圖片中心對齊角色

    def get_rect(self):
        return pygame.Rect(self.x - 15, self.y - 15, 30, 30)  # 提供碰撞區域