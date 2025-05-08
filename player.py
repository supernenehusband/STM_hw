import pygame

class Player:
    def __init__(self, lanes, sprite):
        self.lanes = lanes
        self.sprite = sprite
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

    def jump(self):
        if not self.is_jumping:
            self.is_jumping = True
            self.jump_timer = 30

    def slide(self):
        if not self.is_jumping:
            self.is_sliding = True
            self.slide_timer = 30 

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


    def draw(self, screen):
        screen.blit(self.sprite, (self.x - 15, self.y - 15))

    def get_rect(self):
        width = self.sprite.get_width()
        height = self.sprite.get_height() // 2 if self.is_sliding else self.sprite.get_height()
        return pygame.Rect(self.x - width // 2, self.y - height // 2, width, height)


