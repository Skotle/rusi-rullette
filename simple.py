import random as rd
import time
import pygame
pygame.mixer.init()
o = 6
for i in range(o):
    key = rd.randrange(o-i)
    value = rd.randrange(o-i)
    print('\n\n\n현재 확률 : %.2f%s(%d번 남음)\n' % (100/(o-i),'%',o-i))
    i = input()
    '''##타임슬립 1
    for i in range(5):
        print(5-i)
        time.sleep(1)'''
    if key == value:
        pygame.mixer.music.load("fire.mp3")
        pygame.mixer.music.play()
        print("사망")
        break
    else:
        pygame.mixer.music.load("none.mp3")
        pygame.mixer.music.play()
        print("생존")
    time.sleep(2)
    print('\n\n')


