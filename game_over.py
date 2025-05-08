import pygame
import sys
import os

pygame.init()
screen = pygame.display.set_mode((600, 400))
font = pygame.font.SysFont(None, 48)

score = sys.argv[1]
clock = pygame.time.Clock()

running = True
while running:
    screen.fill((0, 0, 0))
    text1 = font.render("Game Over", True, (255, 255, 255))
    text2 = font.render(f"Score: {score}", True, (255, 255, 255))
    text3 = font.render("Press Enter to Restart", True, (255, 255, 255))

    text1_rect = text1.get_rect(center=(300, 100))
    text2_rect = text2.get_rect(center=(300, 180))
    text3_rect = text3.get_rect(center=(300, 260))

    screen.blit(text1, text1_rect)
    screen.blit(text2, text2_rect)
    screen.blit(text3, text3_rect)
    pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER:
                pygame.quit()
                os.execv(sys.executable, [sys.executable, 'final.py'])
            if event.key == pygame.K_ESCAPE:
                running = False

    clock.tick(30)

pygame.quit()
sys.exit()
