import pygame

class HUD:
    def __init__(self, font, heart_full_image, heart_empty_image, emblem_image, emblem_gray_image, max_lives):
        self.font = font
        self.heart_full = heart_full_image
        self.heart_empty = heart_empty_image
        self.emblem_image = emblem_image
        self.gray_emblem_image = emblem_gray_image
        self.max_lives = max_lives
        self.emblems = 0

    def draw_score(self, screen, score):
        score_text = self.font.render(f"Score: {score}", True, (255, 255, 255))
        screen.blit(score_text, (10, 10))

    def draw_lives(self, screen, lives):
        # 在右上角畫出 full / empty heart
        for i in range(self.max_lives):
            x = 550 - i * 30
            y = 10
            if i < lives:
                screen.blit(self.heart_full, (x, y))
            else:
                screen.blit(self.heart_empty, (x, y))

    def draw_emblems(self, screen):
        for i in range(3):
            x = 490 + i * 30
            y = 40
            if i < self.emblems:
                screen.blit(self.emblem_image, (x, y))
            else:
                screen.blit(self.gray_emblem_image, (x, y))

    def add_emblem(self):
        self.emblems += 1
        if self.emblems >= 3:
            self.emblems = 0
            return True  # 加分
        return False
