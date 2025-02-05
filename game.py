import random as rd
import time
import pygame



prob = int(input("탄창 슬롯 : "))  # 탄창 슬롯 개수
fire = []  # 총알 위치
o = int(input("총알 수 : "))  # 총알 개수
man = int(input("참여자 수 : "))
# 생존자 수
er = [i + 1 for i in range(man)]  # 생존자 리스트

# 랜덤 총알 배치 (중복 없음)
while len(fire) < o:
    num = rd.randrange(1, prob + 1)
    if num not in fire:
        fire.append(num)

# 장전 효과
for _ in range(5):
    time.sleep(1)
    print('.\n')

pygame.mixer.init()
pygame.mixer.music.load("revol.mp3")
pygame.mixer.music.play()
print("총알이 장전됨.\n")
print('.')

# 게임 시작
for i in range(prob):
    print("\n남은 총알 수 : %d발 / 남은 격발 횟수 : %d" % (len(fire), prob - i))
    print("(%.2f%s)\n"%(len(fire)/(prob - i)*100,'%'))
    # 플레이어 선택 (혼자일 경우 건너뜀)
    if man > 1:
        print("생존자 [", end=" ")
        for p in er:
            print("%d번" % p, end=" ")
        print("]")

        turn = er[rd.randrange(len(er))]  # 랜덤 생존자 선택
        print("턴 : %d번" % turn)

    # 사용자 입력 받기
    while True:
        value = input('>> ')
        if value in ('1', '0'):
            value = int(value)
            break
        if value == "stop":
            break
    if value == "stop":
            break

    # 총알 여부 확인 (i+1을 사용하여 탄창과 비교)
    if (i + 1) not in fire:
        pygame.mixer.music.load("none.mp3")
        pygame.mixer.music.play()
        print("총알 없음, ", end="")

        if value == 0:
            print("예측 성공\n생존")
        else:
            print("예측 실패")
            if man > 1:
                er.remove(turn)
            time.sleep(2)
            print("처형")
            pygame.mixer.music.load("fire.mp3")
            pygame.mixer.music.play()
            if man == 1:
                break
    else:
        pygame.mixer.music.load("fire.mp3")
        pygame.mixer.music.play()
        fire.remove(i + 1)  # 총알 소모
        print("총알 발사됨, ", end="")

        if value == 1:
            print("예측 성공\n생존")
        else:
            print("예측 실패\n사망")
            if man > 1:
                er.remove(turn)
            if man == 1:
                break

    time.sleep(2)

    # 게임 종료 조건
    if len(er)==0 or len(fire)==0:
        break
    '''
    if man > 1 and len(er) == 1:
        break'''
    print("\n\n\n\n\n\n\n\n\n\n\n\n")

# 게임 결과 출력
if len(er) > 0 and man!=1:
    print("생존자 :", er)
    print("(%d명)"%(len(er)))
else:
    print("전원 사망")
