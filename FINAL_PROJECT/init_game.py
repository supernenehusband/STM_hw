import pygame

def init_game():
    pygame.init()
    screen = pygame.display.set_mode((600, 400))
    clock = pygame.time.Clock()
    font = pygame.font.SysFont(None, 36)

    enemy_image = pygame.image.load("FINAL_PROJECT/pic/pic1.png").convert_alpha()
    enemy_sprite = pygame.transform.scale(enemy_image, (60, 60))
    player_image = pygame.image.load("FINAL_PROJECT/pic/pic2.png").convert_alpha()
    player_sprite = pygame.transform.scale(player_image, (30, 30))
    trap_image = pygame.image.load("FINAL_PROJECT/pic/thorn.png").convert_alpha()
    trap_sprite = pygame.transform.scale(trap_image, (60, 60))
    heart_full_image = pygame.image.load("FINAL_PROJECT/pic/heart_full.png").convert_alpha()
    heart_full_image = pygame.transform.scale(heart_full_image, (20, 20))
    heart_empty_image = pygame.image.load("FINAL_PROJECT/pic/heart_empty.png").convert_alpha()
    heart_empty_image = pygame.transform.scale(heart_empty_image, (20, 20))
    ufo_image = pygame.image.load("FINAL_PROJECT/pic/ufo.png").convert_alpha()
    ufo_sprite = pygame.transform.scale(ufo_image, (60, 60))
    emblem_image = pygame.image.load("FINAL_PROJECT/pic/emblem.png").convert_alpha()
    emblem_sprite = pygame.transform.scale(emblem_image, (30, 30))
    aplus_image = pygame.image.load("FINAL_PROJECT/pic/aplus.png").convert_alpha()
    aplus_sprite = pygame.transform.scale(aplus_image, (100, 100))
    # ✅ 椰林大道背景圖
    background = pygame.image.load("FINAL_PROJECT/pic/ntupath_darker.jpg").convert()
    background = pygame.transform.scale(background, (600, 400))

    emblem_image = pygame.image.load("FINAL_PROJECT/pic/emblem.png").convert_alpha()
    emblem_sprite = pygame.transform.scale(emblem_image, (30, 30))  
    emblem_gray_image = pygame.image.load("FINAL_PROJECT/pic/emblem_gray.png").convert_alpha()
    emblem_gray_sprite = pygame.transform.scale(emblem_gray_image, (30, 30))

    aplus_image = pygame.image.load("FINAL_PROJECT/pic/aplus.png").convert_alpha()
    aplus_sprite = pygame.transform.scale(aplus_image, (60, 60))

    return screen, clock, font, enemy_sprite, player_sprite, trap_sprite, ufo_sprite, heart_full_image, heart_empty_image, background, emblem_sprite, emblem_gray_sprite, aplus_sprite
