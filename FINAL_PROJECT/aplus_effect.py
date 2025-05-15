import pygame

class APlusEffect:
    def __init__(self, image):
        self.image = image
        self.timer = 0
        self.max_timer = 90  # 顯示 90 幀
        self.active = False

    def start(self):
        self.timer = self.max_timer
        self.active = True

    def update(self):
        if self.active:
            self.timer -= 1
            if self.timer <= 0:
                self.active = False

    def draw(self, screen):
        if self.active:
            progress = 1 - self.timer / self.max_timer
            scale = 0.5 + progress
            angle = 360 * progress
            w = int(self.image.get_width() * scale)
            h = int(self.image.get_height() * scale)
            img = pygame.transform.rotozoom(self.image, angle, scale)

            # 動畫從右上角(500, 50) → 中央(300, 180)
            start_x, start_y = 500, 50
            end_x, end_y = 300, 180
            current_x = start_x + (end_x - start_x) * progress
            current_y = start_y + (end_y - start_y) * progress

            rect = img.get_rect(center=(current_x, current_y))
            screen.blit(img, rect)