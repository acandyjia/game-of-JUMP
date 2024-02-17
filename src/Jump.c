#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define NUM 11
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

SDL_Window *window;
SDL_Renderer *renderer;
// surface
SDL_Surface *surface_background;
SDL_Surface *surface_title;
SDL_Surface *surface_gameover;
SDL_Surface *surface_goodfeeling;
SDL_Surface *surface_badfeeling;
SDL_Surface *surface_score_plus;
SDL_Surface *surface_score_minus;
SDL_Surface *surface_life_plus;
SDL_Surface *surface_life_minus;
SDL_Surface *surface_life;
SDL_Surface *surface_fox;
SDL_Surface *surface_rect_land[15];
SDL_Surface *surface_score;
SDL_Surface *surface_maxscore;
SDL_Surface *surface_continue;
SDL_Surface *surface_restart;
// texture
SDL_Texture *texture_background;
SDL_Texture *texture_title;
SDL_Texture *texture_gameover;
SDL_Texture *texture_goodfeeling;
SDL_Texture *texture_badfeeling;
SDL_Texture *texture_score_plus;
SDL_Texture *texture_score_minus;
SDL_Texture *texture_life_plus;
SDL_Texture *texture_life_minus;
SDL_Texture *texture_life;
SDL_Texture *texture_fox;
SDL_Texture *texture_rect_land[15];
SDL_Texture *texture_score;
SDL_Texture *texture_maxscore;
SDL_Texture *texture_continue;
SDL_Texture *texture_restart;
// rect
SDL_Rect rect_title = {.x = 150, .y = 150};
SDL_Rect rect_gameover = {.x = 150, .y = 550};
SDL_Rect rect_life = {.x = 750, .y = 50};
SDL_Rect rect_continue = {.x = 150, .y = 250};
SDL_Rect rect_continue1 = {.x = 150, .y = 250};
SDL_Rect rect_restart = {.x = 150, .y = 350};
SDL_Rect rect_restart1 = {.x = 150, .y = 350};
SDL_Rect rect_fox = {480, 280, 50, 80};
SDL_Rect rect_goodfeeling = {480, 230, 50, 50};
SDL_Rect rect_badfeeling = {480, 230, 50, 50};
SDL_Rect rect_score_plus = {.x = 520, .y = 230};
SDL_Rect rect_score_minus = {.x = 520, .y = 230};
SDL_Rect rect_life_plus = {.x = 520, .y = 230};
SDL_Rect rect_life_minus = {.x = 520, .y = 230};
SDL_Rect rect_land[10];
SDL_Rect rect_c_land[10];
SDL_Rect rect_power = {200, 650, 50, 10};
SDL_Rect rect_power_back = {200, 150, 50, 510};
SDL_Rect rect_score = {.x = 50, .y = 100};
SDL_Rect rect_maxscore = {.x = 50, .y = 50};
// event
SDL_Event event;

// font
TTF_Font *font_title;
TTF_Font *font_stop;
// color
SDL_Color color_title = {0x50, 0x30, 0x40, 0xff};
// text
char text_score[20];
char text_maxscore[20];
char text_life[20];
char text_score_plus[20];
char text_score_minus[20];
char text_life_plus[20];
char text_life_minus[20];

int power;
int rand1;
int flag_play, flag_jump, flag_stop, flag_wait;
int score, maxscore;
int rand_x1, rand_y1, rand_w1, rand_index1;
int rand_x2, rand_y2, rand_w2, rand_index2;
double timer;
int life = 1;

void Load();
void Quit();
void QiDong();
int Play();
void PutRect(int w, int x, int y, int index);
int Stop();
int AccumulatePower();
int Jump(int power);
void Reset();
int Rand(int min, int max);
void Change();
int Wait();
void Move(int rand_index);

#undef main
int main(void)
{
    QiDong();
    int flag = 1;
    while (flag)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                flag = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    flag = 0;
                    break;
                case SDLK_SPACE:
                    flag_play = Play();
                    if (flag_play == -1)
                        flag = 0;
                    else if (flag_play == 1)
                        Reset();
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            fflush(stdout);
        }
        SDL_Delay(5);
    }
    Quit();
    return 0;
}

void QiDong()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();
    Load();
    Reset();
}
void Reset()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture_background, NULL, NULL);

    rect_fox.x = 480;
    rect_fox.y = 280;
    rect_fox.w = 50;
    rect_fox.h = 80;

    rand_x1 = rand_y1 = 0;
    rand_w1 = 100;
    rand_index1 = 0;
    PutRect(rand_w1, rand_x1, rand_y1, rand_index1);

    SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);

    // font
    SDL_QueryTexture(texture_title, NULL, NULL, &rect_title.w, &rect_title.h);
    SDL_RenderCopy(renderer, texture_title, NULL, &rect_title);
    SDL_QueryTexture(texture_life_minus, NULL, NULL, &rect_life_minus.w, &rect_life_minus.h);
    SDL_QueryTexture(texture_life_plus, NULL, NULL, &rect_life_plus.w, &rect_life_plus.h);

    score = 0;
    life = 1;
    sprintf(text_score, "Score: %d", score);
    surface_score = TTF_RenderText_Blended(font_title, text_score, color_title);
    texture_score = SDL_CreateTextureFromSurface(renderer, surface_score);
    SDL_QueryTexture(texture_score, NULL, NULL, &rect_score.w, &rect_score.h);
    SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
    sprintf(text_maxscore, "MaxScore: %d", maxscore);
    surface_maxscore = TTF_RenderText_Blended(font_title, text_maxscore, color_title);
    texture_maxscore = SDL_CreateTextureFromSurface(renderer, surface_maxscore);
    SDL_QueryTexture(texture_maxscore, NULL, NULL, &rect_maxscore.w, &rect_maxscore.h);
    SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
    sprintf(text_life, "Life: %d", life);
    surface_life = TTF_RenderText_Blended(font_title, text_life, color_title);
    texture_life = SDL_CreateTextureFromSurface(renderer, surface_life);
    SDL_QueryTexture(texture_life, NULL, NULL, &rect_life.w, &rect_life.h);
    SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);

    SDL_RenderPresent(renderer);
}
void Load()
{
    window = SDL_CreateWindow("JUMP", 50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // font
    font_title = TTF_OpenFont("font/ttf.ttf", 32);
    font_stop = TTF_OpenFont("font/ttf.ttf", 64);
    // surface
    surface_background = IMG_Load("image/background.png");
    surface_title = TTF_RenderText_Blended(font_title, "Press space to start the game", color_title);
    surface_gameover = TTF_RenderText_Blended(font_title, "Game over!     Press space to restart", color_title);
    surface_continue = TTF_RenderText_Blended(font_stop, "Continue", color_title);
    surface_restart = TTF_RenderText_Blended(font_stop, "Restart", color_title);
    surface_life_minus = TTF_RenderText_Blended(font_title, "Life -1", color_title);
    surface_life_plus = TTF_RenderText_Blended(font_title, "Life +1", color_title);
    surface_score = TTF_RenderText_Blended(font_title, text_score, color_title);
    surface_fox = IMG_Load("image/fox.png");
    surface_goodfeeling = IMG_Load("image/good.png");
    surface_badfeeling = IMG_Load("image/bad.png");
    surface_rect_land[0] = IMG_Load("image/re-land0.png");
    surface_rect_land[1] = IMG_Load("image/re-land1.png");
    surface_rect_land[2] = IMG_Load("image/re-land2.png");
    surface_rect_land[3] = IMG_Load("image/re-land3.png");
    surface_rect_land[4] = IMG_Load("image/re-land4.png");
    surface_rect_land[5] = IMG_Load("image/re-land5.png");
    surface_rect_land[6] = IMG_Load("image/re-land6.png");
    surface_rect_land[7] = IMG_Load("image/re-land7.png");
    surface_rect_land[8] = IMG_Load("image/re-land8.png");
    surface_rect_land[9] = IMG_Load("image/re-land9.png");
    surface_rect_land[10] = IMG_Load("image/re-land10.png");
    surface_rect_land[11] = IMG_Load("image/re-land11.png");
    // texture
    texture_background = SDL_CreateTextureFromSurface(renderer, surface_background);
    texture_title = SDL_CreateTextureFromSurface(renderer, surface_title);
    texture_gameover = SDL_CreateTextureFromSurface(renderer, surface_gameover);
    texture_fox = SDL_CreateTextureFromSurface(renderer, surface_fox);
    texture_badfeeling = SDL_CreateTextureFromSurface(renderer, surface_badfeeling);
    texture_goodfeeling = SDL_CreateTextureFromSurface(renderer, surface_goodfeeling);
    texture_life_minus = SDL_CreateTextureFromSurface(renderer, surface_life_minus);
    texture_life_plus = SDL_CreateTextureFromSurface(renderer, surface_life_plus);
    for (int i = 0; i <= NUM; i++)
        texture_rect_land[i] = SDL_CreateTextureFromSurface(renderer, surface_rect_land[i]);
}
void PutRect(int w, int x, int y, int index)
{
    rect_land[index].w = rect_land[index].h = w;
    rect_land[index].x = WINDOW_WIDTH / 2 - w / 2 + x;
    rect_land[index].y = WINDOW_HEIGHT / 2 - w / 2 - y;
    SDL_RenderCopy(renderer, texture_rect_land[index], NULL, &rect_land[index]);
}
int Play()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture_background, NULL, NULL);
    rand_x1 = rand_y1 = 0;
    rand_w1 = 100;
    rand_index1 = 0;
    PutRect(rand_w1, rand_x1, rand_y1, rand_index1);
    SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);

    rand_x2 = Rand(150, 300);
    rand_w2 = Rand(100, 150);
    rand_y2 = 0;
    rand1 = 0;
    do
        rand_index2 = Rand(0, NUM);
    while (rand_index2 == rand_index1);
    PutRect(rand_w2, rand_x2, rand_y2, rand_index2);

    sprintf(text_score, "Score: %d", score);
    surface_score = TTF_RenderText_Blended(font_title, text_score, color_title);
    texture_score = SDL_CreateTextureFromSurface(renderer, surface_score);
    SDL_QueryTexture(texture_score, NULL, NULL, &rect_score.w, &rect_score.h);
    SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
    sprintf(text_maxscore, "MaxScore: %d", maxscore);
    surface_maxscore = TTF_RenderText_Blended(font_title, text_maxscore, color_title);
    texture_maxscore = SDL_CreateTextureFromSurface(renderer, surface_maxscore);
    SDL_QueryTexture(texture_maxscore, NULL, NULL, &rect_maxscore.w, &rect_maxscore.h);
    SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
    SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);

    SDL_RenderPresent(renderer);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    flag_stop = Stop();
                    if (flag_stop == -1)
                        return -1;
                    break;
                case SDLK_SPACE:
                    power = AccumulatePower();
                    flag_jump = Jump(power);
                    if (flag_jump == -1)
                        return -1;
                    else if (flag_jump == 1)
                        return 1;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            fflush(stdout);
        }
        SDL_Delay(5);
    }
}
void Quit()
{
    // surface
    SDL_FreeSurface(surface_background);
    for (int i = 0; i <= NUM; i++)
        SDL_FreeSurface(surface_rect_land[i]);
    SDL_FreeSurface(surface_fox);
    SDL_FreeSurface(surface_title);
    SDL_FreeSurface(surface_gameover);
    SDL_FreeSurface(surface_goodfeeling);
    SDL_FreeSurface(surface_badfeeling);
    SDL_FreeSurface(surface_score);
    SDL_FreeSurface(surface_maxscore);
    SDL_FreeSurface(surface_continue);
    SDL_FreeSurface(surface_restart);
    SDL_FreeSurface(surface_score_plus);
    SDL_FreeSurface(surface_score_minus);
    SDL_FreeSurface(surface_life_plus);
    SDL_FreeSurface(surface_life_minus);
    // font
    TTF_CloseFont(font_title);
    TTF_CloseFont(font_stop);
    // texture
    SDL_DestroyTexture(texture_background);
    for (int i = 0; i <= NUM; i++)
        SDL_DestroyTexture(texture_rect_land[i]);
    SDL_DestroyTexture(texture_fox);
    SDL_DestroyTexture(texture_title);
    SDL_DestroyTexture(texture_gameover);
    SDL_DestroyTexture(texture_goodfeeling);
    SDL_DestroyTexture(texture_badfeeling);
    SDL_DestroyTexture(texture_score);
    SDL_DestroyTexture(texture_maxscore);
    SDL_DestroyTexture(texture_continue);
    SDL_DestroyTexture(texture_restart);
    SDL_DestroyTexture(texture_score_plus);
    SDL_DestroyTexture(texture_score_minus);
    SDL_DestroyTexture(texture_life_plus);
    SDL_DestroyTexture(texture_life_minus);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
int Stop()
{
    texture_continue = SDL_CreateTextureFromSurface(renderer, surface_continue);
    SDL_QueryTexture(texture_continue, NULL, NULL, &rect_continue.w, &rect_continue.h);
    SDL_QueryTexture(texture_continue, NULL, NULL, &rect_continue1.w, &rect_continue1.h);
    SDL_SetRenderDrawColor(renderer, 0x60, 0xc0, 0xa0, 0xff);
    SDL_RenderFillRect(renderer, &rect_continue1);
    SDL_RenderCopy(renderer, texture_continue, NULL, &rect_continue);
    texture_restart = SDL_CreateTextureFromSurface(renderer, surface_restart);
    SDL_QueryTexture(texture_restart, NULL, NULL, &rect_restart.w, &rect_restart.h);
    SDL_QueryTexture(texture_restart, NULL, NULL, &rect_restart1.w, &rect_restart1.h);
    SDL_SetRenderDrawColor(renderer, 0x60, 0xc0, 0xa0, 0xff);
    SDL_RenderFillRect(renderer, &rect_restart1);
    SDL_RenderCopy(renderer, texture_restart, NULL, &rect_restart);
    SDL_RenderPresent(renderer);

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return -1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >= rect_continue1.x && event.button.x <= rect_continue1.x + rect_continue1.w &&
                    event.button.y >= rect_continue1.y && event.button.y <= rect_continue1.y + rect_continue1.h)
                {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture_background, NULL, NULL);
                    rand_x1 = rand_y1 = 0;
                    PutRect(rand_w1, rand_x1, rand_y1, rand_index1);
                    PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
                    SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
                    SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
                    SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
                    SDL_RenderPresent(renderer);
                    return 1;
                }
                else if (event.button.x >= rect_restart1.x && event.button.x <= rect_restart1.x + rect_restart1.w &&
                         event.button.y >= rect_restart1.y && event.button.y <= rect_restart1.y + rect_restart1.h)
                {
                    Reset();
                    return 1;
                }
                break;

            default:
                break;
            }
            fflush(stdout);
        }
        SDL_Delay(5);
    }
}
int AccumulatePower()
{
    int power = 0;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    SDL_RenderFillRect(renderer, &rect_power_back);
                    SDL_SetRenderDrawColor(renderer, 0x60, 0xc0, 0xa0, 0xff);
                    SDL_RenderFillRect(renderer, &rect_power);
                    SDL_RenderPresent(renderer);
                    if (power < 50)
                    {
                        rect_power.y -= 10;
                        power++;
                    }
                }
                break;
            case SDL_KEYUP:
                rect_power.y = 650;
                SDL_Delay(500);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture_background, NULL, NULL);
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &rect_power_back);
                PutRect(rand_w1, rand_x1, rand_y1, rand_index1);
                PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
                SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
                SDL_RenderPresent(renderer);
                return power * 7 / 5;
            default:
                break;
            }
            fflush(stdout);
        }
        SDL_Delay(5);
    }
}
int Jump(int power)
{
    int tmp_y = rect_fox.y;
    int tmp_x = rect_fox.x;
    for (int i = 0; i <= power; i++)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture_background, NULL, NULL);
        PutRect(rand_w1, 0, 0, rand_index1);
        PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
        if (rand1 == 0)
        {
            if (i <= power / 2)
                rect_fox.y -= 5;
            else
                rect_fox.y += 5;
            if (i == power)
                rect_fox.y = tmp_y;
            rect_fox.x += 5;
        }
        else
        {
            if (i <= power / 2)
                rect_fox.x -= 3;
            else
                rect_fox.x += 3;
            if (i == power)
                rect_fox.x = tmp_x;
            rect_fox.y -= 5;
        }
        SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
        SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
        SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
        SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
        SDL_RenderPresent(renderer);
        SDL_Delay(12);
    }

    // 检查是否落在下一个方块上或落在原来的方块上，如果是，继续，否则，结束
    if (rect_fox.x + rect_fox.w / 2 >= rect_land[rand_index2].x &&
        rect_fox.x + rect_fox.w / 2 <= rect_land[rand_index2].x + rect_land[rand_index2].w &&
        rect_fox.y + rect_fox.h >= rect_land[rand_index2].y &&
        rect_fox.y + rect_fox.h <= rect_land[rand_index2].y + rect_land[rand_index2].h)
    {
        // 平移并生成下一个方块，删除上一个方块
        if ((abs((rect_fox.x + rect_fox.w / 2) - (rect_land[rand_index2].x + rect_land[rand_index2].w / 2)) <= 15) &&
            (abs((rect_fox.y + rect_fox.h) - (rect_land[rand_index2].y + rect_land[rand_index2].h / 2)) <= 15))
            score += 2;
        else
            score++;
        sprintf(text_score, "Score: %d", score);
        surface_score = TTF_RenderText_Blended(font_title, text_score, color_title);
        texture_score = SDL_CreateTextureFromSurface(renderer, surface_score);
        SDL_QueryTexture(texture_score, NULL, NULL, &rect_score.w, &rect_score.h);
        SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
        if (score > maxscore)
        {
            maxscore = score;
            sprintf(text_maxscore, "MaxScore: %d", maxscore);
            surface_maxscore = TTF_RenderText_Blended(font_title, text_maxscore, color_title);
            texture_maxscore = SDL_CreateTextureFromSurface(renderer, surface_maxscore);
            SDL_QueryTexture(texture_maxscore, NULL, NULL, &rect_maxscore.w, &rect_maxscore.h);
            SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
        }

        Change();
        SDL_RenderCopy(renderer, texture_background, NULL, NULL);
        PutRect(rand_w1, rand_x1, rand_y1, rand_index1);
        SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
        SDL_RenderPresent(renderer);

        Move(rand_index1);
        rand1 = Rand(0, 1);
        if (rand1 == 0)
        {
            rand_x2 = Rand(150, 300);
            rand_y2 = 0;
        }
        else
        {
            rand_y2 = Rand(150, 300);
            rand_x2 = 0;
        }
        if (score < 20) // 分数越高难度越大
            rand_w2 = Rand(100, 150);
        else if (score < 40)
            rand_w2 = Rand(90, 130);
        else if (score < 60)
            rand_w2 = Rand(80, 100);
        else if (score < 80)
            rand_w2 = Rand(60, 90);
        else
            rand_w2 = Rand(50, 80);
        do
            rand_index2 = Rand(0, NUM);
        while (rand_index2 == rand_index1);
        PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
        SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
        SDL_RenderPresent(renderer);

        if (rand_index1 >= 4 && rand_index1 <= 7)
            flag_wait = Wait();
        if (flag_wait == -1)
            return -1;
        return 2;
    }
    else if (rect_fox.x + rect_fox.w / 2 >= rect_land[rand_index1].x &&
             rect_fox.x + rect_fox.w / 2 <= rect_land[rand_index1].x + rect_land[rand_index1].w &&
             rect_fox.y + rect_fox.h >= rect_land[rand_index1].y &&
             rect_fox.y + rect_fox.h <= rect_land[rand_index1].y + rect_land[rand_index1].h)
    {
        // 继续
        SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
        SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
        SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
        SDL_RenderPresent(renderer);
        return 2;
    }
    else
    {
        // 结束 or life-1
        life--;
        sprintf(text_life, "Life: %d", life);
        surface_life = TTF_RenderText_Blended(font_title, text_life, color_title);
        texture_life = SDL_CreateTextureFromSurface(renderer, surface_life);
        SDL_QueryTexture(texture_life, NULL, NULL, &rect_life.w, &rect_life.h);
        SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
        if (life == 0)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture_background, NULL, NULL);
            PutRect(rand_w1, 0, 0, rand_index1);
            PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
            SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
            SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
            SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
            SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
            SDL_QueryTexture(texture_gameover, NULL, NULL, &rect_gameover.w, &rect_gameover.h);
            SDL_RenderCopy(renderer, texture_gameover, NULL, &rect_gameover);
            SDL_RenderPresent(renderer);
            while (1)
            {
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        return -1;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_SPACE)
                            return 1;
                        break;
                    default:
                        break;
                    }
                }
                fflush(stdout);
            }
            SDL_Delay(5);
        }
        else
        {
            SDL_RenderCopy(renderer, texture_badfeeling, NULL, &rect_badfeeling);
            SDL_RenderCopy(renderer, texture_life_minus, NULL, &rect_life_minus);
            SDL_RenderPresent(renderer);
            SDL_Delay(700);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture_background, NULL, NULL);
            SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
            SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
            SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
            Move(rand_index2);
            rect_fox.x = 480;
            rect_fox.y = 280;
            SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
            Change();
            rand1 = Rand(0, 1);
            if (rand1 == 0)
            {
                rand_x2 = Rand(150, 300);
                rand_y2 = 0;
            }
            else
            {
                rand_y2 = Rand(150, 300);
                rand_x2 = 0;
            }
            rand_w2 = Rand(100, 150);
            do
                rand_index2 = Rand(0, NUM);
            while (rand_index2 == rand_index1);
            PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
            SDL_RenderPresent(renderer);
        }
    }
}
int Rand(int min, int max)
{
    srand((unsigned)time(NULL));
    return rand() % (max - min + 1) + min;
}
void Change()
{
    rect_land[rand_index1] = rect_land[rand_index2];
    rand_x1 = rand_x2;
    rand_y1 = rand_y2;
    rand_w1 = rand_w2;
    rand_index1 = rand_index2;
}
int Wait()
{
    clock_t begin = clock();
    while (1)
    {
        clock_t end = clock();
        timer = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
        if (timer >= 3000)
        {
            if (rand_index1 == 7)
            {
                SDL_RenderCopy(renderer, texture_goodfeeling, NULL, &rect_goodfeeling);
                sprintf(text_score_plus, "Score + %d", 30);
                surface_score_plus = TTF_RenderText_Blended(font_title, text_score_plus, color_title);
                texture_score_plus = SDL_CreateTextureFromSurface(renderer, surface_score_plus);
                SDL_QueryTexture(texture_score_plus, NULL, NULL, &rect_score_plus.w, &rect_score_plus.h);
                SDL_RenderCopy(renderer, texture_score_plus, NULL, &rect_score_plus);
                SDL_RenderPresent(renderer);
                SDL_Delay(700);
                score += 30;
            }
            if (rand_index1 == 5)
            {
                SDL_RenderCopy(renderer, texture_goodfeeling, NULL, &rect_goodfeeling);
                sprintf(text_score_plus, "Score + %d", 10);
                surface_score_plus = TTF_RenderText_Blended(font_title, text_score_plus, color_title);
                texture_score_plus = SDL_CreateTextureFromSurface(renderer, surface_score_plus);
                SDL_QueryTexture(texture_score_plus, NULL, NULL, &rect_score_plus.w, &rect_score_plus.h);
                SDL_RenderCopy(renderer, texture_score_plus, NULL, &rect_score_plus);
                SDL_RenderPresent(renderer);
                SDL_Delay(700);
                score += 10;
            }
            else if (rand_index1 == 6)
            {
                SDL_RenderCopy(renderer, texture_badfeeling, NULL, &rect_badfeeling);
                sprintf(text_score_minus, "Score - %d", 10);
                surface_score_minus = TTF_RenderText_Blended(font_title, text_score_minus, color_title);
                texture_score_minus = SDL_CreateTextureFromSurface(renderer, surface_score_minus);
                SDL_QueryTexture(texture_score_minus, NULL, NULL, &rect_score_minus.w, &rect_score_minus.h);
                SDL_RenderCopy(renderer, texture_score_minus, NULL, &rect_score_minus);
                SDL_RenderPresent(renderer);
                SDL_Delay(700);
                score -= 10;
            }
            else if (rand_index1 == 4)
            {
                SDL_RenderCopy(renderer, texture_goodfeeling, NULL, &rect_goodfeeling);
                SDL_RenderCopy(renderer, texture_life_plus, NULL, &rect_life_plus);
                SDL_RenderPresent(renderer);
                SDL_Delay(700);
                life++;
                sprintf(text_life, "Life: %d", life);
                surface_life = TTF_RenderText_Blended(font_title, text_life, color_title);
                texture_life = SDL_CreateTextureFromSurface(renderer, surface_life);
                SDL_QueryTexture(texture_life, NULL, NULL, &rect_life.w, &rect_life.h);
                SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
            }

            timer = 0;
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture_background, NULL, NULL);
            PutRect(rand_w1, 0, 0, rand_index1);
            PutRect(rand_w2, rand_x2, rand_y2, rand_index2);
            SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
            sprintf(text_score, "Score: %d", score);
            surface_score = TTF_RenderText_Blended(font_title, text_score, color_title);
            texture_score = SDL_CreateTextureFromSurface(renderer, surface_score);
            SDL_QueryTexture(texture_score, NULL, NULL, &rect_score.w, &rect_score.h);
            SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
            if (score > maxscore)
            {
                maxscore = score;
                sprintf(text_maxscore, "MaxScore: %d", maxscore);
                surface_maxscore = TTF_RenderText_Blended(font_title, text_maxscore, color_title);
                texture_maxscore = SDL_CreateTextureFromSurface(renderer, surface_maxscore);
                SDL_QueryTexture(texture_maxscore, NULL, NULL, &rect_maxscore.w, &rect_maxscore.h);
            }
            SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
            SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
            SDL_RenderPresent(renderer);
            return 1;
        }
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    Stop();
                    begin = clock();
                    break;
                }
                if (event.key.keysym.sym == SDLK_SPACE)
                    return 1;
                break;
            default:
                break;
            }
        }
        fflush(stdout);
    }
    SDL_Delay(5);
}
void Move(int rand_index)
{
    while (rect_land[rand_index].x > WINDOW_WIDTH / 2 - rect_land[rand_index].w / 2)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture_background, NULL, NULL);
        rect_land[rand_index].x -= 1;
        SDL_RenderCopy(renderer, texture_rect_land[rand_index], NULL, &rect_land[rand_index]);
        if (rand_index == rand_index1)
        {
            rect_fox.x -= 1;
            SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
        }
        SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
        SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
        SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
        SDL_RenderPresent(renderer);
        SDL_Delay(2);
    }
    while (rect_land[rand_index].y < WINDOW_HEIGHT / 2 - rect_land[rand_index].h / 2)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture_background, NULL, NULL);
        rect_land[rand_index].y += 1;
        SDL_RenderCopy(renderer, texture_rect_land[rand_index], NULL, &rect_land[rand_index]);
        if (rand_index == rand_index1)
        {
            rect_fox.y += 1;
            SDL_RenderCopy(renderer, texture_fox, NULL, &rect_fox);
        }
        SDL_RenderCopy(renderer, texture_score, NULL, &rect_score);
        SDL_RenderCopy(renderer, texture_maxscore, NULL, &rect_maxscore);
        SDL_RenderCopy(renderer, texture_life, NULL, &rect_life);
        SDL_RenderPresent(renderer);
        SDL_Delay(2);
    }
}