#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
 
#define SIZE    16
#define UP      0x69
#define UP_B    0x49
#define DOWN    0x6b
#define DOWN_B  0x4b
#define RIGHT   0x6c
#define RIGHT_B 0x4c
#define LEFT    0x6a
#define LEFT_B  0x4a
 
int arr[SIZE] = {0};
int is_move = 0;
int is_merge = 0;
int max;
int total;
 
static struct termios oldt;
 
void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);
}
 
void disable_terminal_return(void)
{
    struct termios newt;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
    atexit(restore_terminal_settings);
}
 
void init(void)
{
    int i, random1, random2;
    random1 = rand() % SIZE;
    for(i = 0; i < 1; i++)
    {
        random2 = rand() % SIZE;
        if(random1 == random2)
        {
            i--;
        }
    }
    arr[random1] = 2;
    arr[random2] = 2;
}
 
int is_dead()
{
    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        if(!arr[i])
        {
            return 0;
        }
    }
    for(i = 0; i < SIZE; i += 4)
    {
        for(j = i; j < i + 3; j++)
        {
            if(arr[j] == arr[j + 1])
            {
                return 0;
            }
        }
    }
    for(i = 0; i < 4; i++)
    {
        for(j = i; j < i + 12; j += 4)
        {
            if(arr[j] == arr[j + 4])
            {
                return 0;
            }
        }
    }
    return 1;
}
 
int max_num(void)
{
    int i;
    static int count = 0;
    for(i = 0; i < SIZE; i++)
    {
        if(arr[i] > max)
        {
            max = arr[i];
        }
    }
    if(!count && 2048 == max)
    {
        count++;
        printf("恭喜过关，请按任意键继续！");
        getchar();
    }
    return max;
}
 
void print_game(void)
{
    system("clear");
    int i;
    printf("按【I】向上移动\n按【K】向下移动\n按【J】向左移动\n按【L】向右移动\n按【Ctrl + Z】退出游戏\n最大的数是：%d\n总分是：%d\n", max_num(), total);
    printf("\n");
    printf("+-------------  2048  --------------+\n");
    printf("+-----------------------------------+\n");
    printf("|        |        |        |        |\n");
    for(i = 0; i < SIZE; i++)
    {
        if(0 == i || 4 == i || 8 == i || 12 == i)
        {
            printf("|");
        }
        printf(0 == arr[i] ? "%9c|" : "%8d|", arr[i]);
        if(3 == i || 7 == i || 11 == i)
        {
            printf("\n");
            printf("|--------+--------+--------+--------|\n");
            printf("|        |        |        |        |\n");
        }
    }
    printf("\n");
    printf("+-----------------------------------+\n");
    printf("+------------Jeremy study -----------+\n");
    printf("\n\n\n");
    if(is_dead())
    {
        int i;
        printf("game over!\n");
        printf("请按任意键重新开始！\n");
        getchar();
        for(i = 0; i < SIZE; i++)
        {
            arr[i] = 0;
        }
        init();
        print_game();
    }
}
 
void rand_num(void)
{
    while(is_move || is_merge)
    {
        int random = rand() % SIZE;
        if(!arr[random])
        {
            arr[random] = 2;
            break;
        }
    }
    is_move = 0;
    is_merge = 0;
}
 
void move_go(int loop_count, int current_i, int direction)
{
    int i;
    for(i = 0; i < loop_count; i++)
    {
        if(arr[current_i] && !arr[current_i + direction])
        {
            arr[current_i + direction] = arr[current_i];
            arr[current_i] = 0;
            current_i += direction;
            is_move = 1;
        }
    }
}
 
void move_up(void)
{
    int i, loop_count, direction;
    for(i = 0; i < SIZE; i++)
    {
        if(arr[i])
        {
            loop_count = i / 4;
            direction = -4;
            move_go(loop_count, i, direction);
        }
    }
}
 
void move_down(void)
{
    int i, loop_count, direction;
    for(i = SIZE - 1; i >= 0; i--)
    {
        if(arr[i])
        {
            loop_count = (4 - 1) - i / 4;
            direction = 4;
            move_go(loop_count, i, direction); 
        }
    }
}
 
void move_right(void)
{
    int i, loop_count, direction;
    for(i = SIZE - 1; i >= 0; i--)
    {
        if(arr[i])
        {
            loop_count = (4 - 1) - (i + 4) % 4;
            direction = 1;
            move_go(loop_count, i, direction);
        }
    }
}
 
void move_left(void)
{
    int i, loop_count, direction;
    for(i = 0; i < SIZE; i++)
    {
        if(arr[i])
        {
            loop_count = (i + 4) % 4;
            direction = -1;
            move_go(loop_count, i, direction);
        }
    }
}
 
void merge(int current_i, int direction)
{
    if(arr[current_i] && arr[current_i + direction] && arr[current_i] == arr[current_i + direction])
    {
        arr[current_i] = arr[current_i + direction] * 2;
        total += arr[current_i];
        arr[current_i + direction] = 0;
        //current_i += direction;
        is_merge = 1;
    }
}
 
void move_up_pre(void)
{
    move_up();
    int i, j, direction = 4;
    for(i = 0; i < 4; i++)
    {
        for(j = i; j < i + 12; j += 4)
        {
            merge(j, direction);
        }
    }
    move_up();
}
 
void move_down_pre(void)
{
    move_down();
    int i, j, direction = -4;
    for(i = 4 - 1; i >= 0; i--)
    {
        for(j = i + 12; j >= 4; j -= 4)
        {
            merge(j, direction);
        }
    }
    move_down();
}
 
void move_right_pre(void)
{
    move_right();
    int i, j, direction = -1;
    for(i = 4 - 1; i >= 0; i--)
    {
        for(j = 4 * i + 3; j > 4 * i; j--)
        {
            merge(j, direction);
        }
    }
    move_right();
}
 
void move_left_pre(void)
{
    move_left();
    int i, j, direction = 1;
    for(i = 0; i <= 3; i++)
    {
        for(j = 4 * i; j < 4 * i + 3; j++)
        {
            merge(j, direction);
        }
    }
    move_left();
}
 
int main(void)
{
    srand(time(NULL));
    init();
    print_game();
    disable_terminal_return();
    while(1)
    {
        switch(getchar())
        {
            case UP:
            case UP_B:
                move_up_pre();
                rand_num();
                print_game();
                break;
            case DOWN:
            case DOWN_B:
                move_down_pre();
                rand_num();
                print_game();
                break;
            case RIGHT:
            case RIGHT_B:
                move_right_pre();
                rand_num();
                print_game();
                break;
            case LEFT:
            case LEFT_B:
                move_left_pre();
                rand_num();
                print_game();
                break;
            default:
                break;
        }
    }
    return 0;
}
