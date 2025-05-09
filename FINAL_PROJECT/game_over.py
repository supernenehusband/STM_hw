import pygame
import sys
import os

# 匯入分數管理
def save_score(score):
    with open("FINAL_PROJECT/scores.txt", "a") as f:
        f.write(f"{score}\n")

def get_scores():
    try:
        with open("FINAL_PROJECT/scores.txt", "r") as f:
            scores = [int(line.strip()) for line in f]
        return sorted(scores, reverse=True)
    except FileNotFoundError:
        return []

pygame.init()
screen = pygame.display.set_mode((600, 400))
font = pygame.font.SysFont(None, 36)

score_value = int(sys.argv[1])

# 儲存分數
save_score(score_value)

# 讀取排行榜
scores = get_scores()

clock = pygame.time.Clock()
running = True

while running:
    screen.fill((0, 0, 0))
    text1 = font.render("Game Over", True, (255, 255, 255))
    text2 = font.render(f"Score: {score_value}", True, (255, 255, 255))
    text3 = font.render("Press Enter to Restart", True, (255, 255, 255))

    text1_rect = text1.get_rect(center=(300, 50))
    text2_rect = text2.get_rect(center=(300, 100))
    text3_rect = text3.get_rect(center=(300, 150))

    screen.blit(text1, text1_rect)
    screen.blit(text2, text2_rect)
    screen.blit(text3, text3_rect)

    # 顯示前 5 名
    for i, s in enumerate(scores[:5], start=1):
        rank_text = font.render(f"{i}. {s}", True, (255, 255, 255))
        rank_rect = rank_text.get_rect(center=(300, 150 + i * 40))
        screen.blit(rank_text, rank_rect)

    pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER:
                pygame.quit()
                os.execv(sys.executable, [sys.executable, 'FINAL_PROJECT/final.py'])
            if event.key == pygame.K_ESCAPE:
                running = False

    clock.tick(30)

pygame.quit()
sys.exit()