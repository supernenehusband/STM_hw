import pygame

class Player:
    def __init__(self, lanes, sprite):
        self.lanes = lanes
        self.sprite_normal = sprite
        self.sprite_sliding = pygame.transform.scale(sprite, (30, 20))
        self.sprite = self.sprite_normal
        self.x = lanes[1]
        self.y = 300
        self.is_jumping = False
        self.jump_timer = 0
        self.is_sliding = False
        self.slide_timer = 0

    def move_left(self):
        current_lane = self.lanes.index(self.x)
        if current_lane > 0:
            self.x = self.lanes[current_lane - 1]

    def move_right(self):
        current_lane = self.lanes.index(self.x)
        if current_lane < 2:
            self.x = self.lanes[current_lane + 1]

    def move_with_strength(self, direction, strength):
        # 移動強度轉換為距離：750 ~ 2000 對應 1/3 格 ~ 2 格寬
        min_val = 750
        max_val = 2000
        min_move = 100 / 3  # 約 33.3 pixels
        max_move = 200      # 約兩格

        # 線性插值計算距離
        norm = max(0.0, min(1.0, (strength - min_val) / (max_val - min_val)))
        move_dist = min_move + norm * (max_move - min_move)

        # 更新 x 座標
        if direction == 'r':
            self.x += move_dist
        elif direction == 'l':
            self.x -= move_dist

        # 限制在畫面邊界範圍（例如跑道從 x=150 到 x=450）
        self.x = max(150, min(self.x, 450))

    def jump(self):
        if not self.is_jumping:
            self.is_jumping = True
            self.jump_timer = 30

    def slide(self):
        if not self.is_jumping:
            self.is_sliding = True
            self.slide_timer = 30
            self.sprite = self.sprite_sliding

    def update(self):
        if self.is_jumping:
            self.y = 250
            self.jump_timer -= 1
            if self.jump_timer <= 0:
                self.is_jumping = False
        elif not self.is_sliding:
            self.y = 300

        if self.is_sliding:
            self.slide_timer -= 1
            if self.slide_timer <= 0:
                self.is_sliding = False
                self.sprite = self.sprite_normal


    def draw(self, screen):
        rect = self.sprite.get_rect()
        rect.center = (self.x, self.y)
        screen.blit(self.sprite, rect.topleft)

    def get_rect(self):
        width = self.sprite.get_width()
        height = self.sprite.get_height()
        return pygame.Rect(self.x - width // 2, self.y - height // 2, width, height)
