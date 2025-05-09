import pygame

class HUD:
    def __init__(self, font, heart_full_image, heart_empty_image, max_lives):
        self.font = font
        self.heart_full = heart_full_image
        self.heart_empty = heart_empty_image
        self.max_lives = max_lives

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
