import random

class ScreenShake:
    def __init__(self, duration=10, intensity=5):
        self.duration = duration
        self.intensity = intensity
        self.timer = 0

    def start(self):
        self.timer = self.duration

    def update(self):
        if self.timer > 0:
            self.timer -= 1

    def get_offset(self):
        if self.timer > 0:
            offset_x = random.randint(-self.intensity, self.intensity)
            offset_y = random.randint(-self.intensity, self.intensity)
            return offset_x, offset_y
        else:
            return 0, 0
