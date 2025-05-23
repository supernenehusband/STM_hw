import pygame

class InkOverlayEffect:
    def __init__(self, screen_size):
        self.active = False
        self.alpha = 0
        self.surface = pygame.Surface(screen_size)
        self.surface.fill((0, 0, 0))
        self.shake_received = False
        self.fading_in = False

    def trigger(self):
        print("[INK] trigger() called")
        self.active = True
        self.alpha = 0
        self.shake_received = False
        self.fading_in = True

    def receive_shake(self):
        if self.active:
            self.shake_received = True

    def update(self):
        if self.active:
            print(f"[INK DEBUG] alpha={self.alpha}, fading_in={self.fading_in}, shake_received={self.shake_received}")
            if self.fading_in:
                self.alpha += 8.5  # 約 1 秒漸暗
                if self.alpha >= 255:
                    self.alpha = 255
                    self.fading_in = False
            elif self.shake_received:
                self.alpha -= 5
                if self.alpha <= 0:
                    self.alpha = 0
                    self.active = False
                    self.shake_received = False

    def draw(self, screen):
        if self.active:
            self.surface.set_alpha(int(self.alpha))
            screen.blit(self.surface, (0, 0))