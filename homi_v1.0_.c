//THIS ALGORITHM IS NOT EFFICIENT :D
#include<stdio.h>
#include<math.h>
#include<string.h>

#define true 1
#define false 0
#define false2 -1
#define W 15
#define H 7
#define MAX_ENEMY_AMMOUNT 5
#define MAX_PROJ_AMMOUNT 5
//thing's IDs
#define TYPE_PROJ 11 //projectiles have ids that are dividable by 11
#define TYPE_SOLID 13 //solids have ids that are dividable by 13
#define EVERYTHING 0 //theoretical...dont plan on using it :p
#define VOID 1
#define PLAYER 2
#define ENEMY 3
#define KATANA 4
#define BOW 5
#define AXE 6
#define ARROW 11 * 2
#define WALL 13 * 2
#define WATER 13 * 3
#define TREE 13 * 4

int mapa[W][H]; //has walls, phys, etc... positions

//int solids[] = {WALL,WATER,TREE};

int enemies[MAX_ENEMY_AMMOUNT][3];
int projectiles[MAX_PROJ_AMMOUNT][5];

int px = 0;
int py = 0;
int pHp = 3;
int pWeapon = BOW;
int pAmmo = 4;

int sign(int v)
{
    if (v == 0)
    {
        return 0;
    }
    else {
        if (v < 0) {return -1;}
        if (v > 0) {return 1;} //if desnecessário mas beleza é bela
    }
}

double distance(int x1,int y1,int x2,int y2)
{
    double dis;
    dis = (double) sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    return dis;
}

int weapon_data (int weapon,int what_do_you_want)
{
    if (what_do_you_want == 0) //atk
    {
        if (weapon == KATANA)
        {
            return 1;
        }
        if (weapon == AXE)
        {
            return 2;
        }
        if (weapon == BOW)
        {
            return 0;
        }
        if (weapon == ARROW)
        {
            return 1;
        }
    }
    if (what_do_you_want == 1) //range (ao quadrado pq int e double smoafnwapo)
    {
        if (weapon == KATANA)
        {
            return 2; //sqrt(2)
        }
        if (weapon == AXE)
        {
            return 2; //sqrt(2)
        }
        if (weapon == BOW)
        {
            return 18; //3*sqrt(2)
        }
    }
    return -1;
}

void create_mapa()
{
    for (int j = 0;j < H;j ++)
    {
        for (int i = 0;i < W;i ++)
        {
            mapa[i][j] = VOID;
        }
    }
}

void set_enemies()
{
    for (int i = 0;i < MAX_ENEMY_AMMOUNT;i ++)
    {
        for (int j = 0;j < 3;j ++)
        {
            enemies[i][j] = 0;
        }
    }
}

void set_projectiles()
{
    for (int i = 0;i < MAX_PROJ_AMMOUNT;i ++)
    {
        projectiles[i][0] = -1;
        for (int j = 0;j < 4;j ++)
        {
            projectiles[i][j + 1] = 0;
        }
    }
}

const char * char_converter(int v)
{
    if (v == VOID)
    {
        return "ロ";
    }
    if (v == PLAYER)
    {
        return "人";
    }
    if (v == WALL)
    {
        return "壁";
    }
    if (v == ENEMY)
    {
        return "か";
    }
    if (v == WATER)
    {
        return "水";
    }
    if (v == TREE)
    {
        return "木";
    }
    if (v == KATANA)
    {
        return "刀";
    }
    if (v == BOW)
    {
        return "弓";
    }
    if (v == AXE)
    {
        return "斧";
    }
    if (v == ARROW)
    {
        return "矢";
    }
}

void print_mapa()
{
    printf("\n");
    for (int j = 0;j < H;j ++)
    {
        for (int i = 0;i < W;i ++)
        {   
            if (px == i && py == j)
            {
                printf(char_converter(PLAYER));
            }
            else {
                int int_son = mapa[i][j];

                for (int p = 0;p < MAX_PROJ_AMMOUNT;p ++)
                {
                    if (projectiles[p][0] != -1)
                    {
                        if (projectiles[p][1] == i && projectiles[p][2] == j)
                        {
                            int_son = projectiles[p][0];
                            break;
                        }
                    }
                }

                for (int e = 0;e < MAX_ENEMY_AMMOUNT;e ++)
                {
                    if (enemies[e][2] != 0)
                    {
                        if (enemies[e][0] == i && enemies[e][1] == j)
                        {
                            int_son = ENEMY;
                            break;
                        }
                    }
                }

                printf(char_converter(int_son));
            }
        }
        printf("\n");
    }
}

int is_wall(int x,int y)
{ //is_x type functions return false2 or id of object
    for (int i = 0;i < 3;i ++) //gotta figure out list size!!!!
    {
        if (mapa[x][y] % TYPE_SOLID == 0)
        {
            return mapa[x][y];
        }
    }

    return false2;
}

int is_enemy(int x,int y)
{ //is_x type functions return false2 or id of object
    for (int e = 0;e < MAX_ENEMY_AMMOUNT;e ++)
    {
        if (enemies[e][2] != 0)
        {
            int x_ = enemies[e][0];
            int y_ = enemies[e][1];
            if (x == x_ && y == y_)
            {
                return e;
            }
        }
    }
    return false2;
}

int is_projectile(int x,int y)
{ //is_x type functions return false2 or id of object
    for (int p = 0;p < MAX_PROJ_AMMOUNT;p ++)
    {
        if (projectiles[p][0] != -1)
        {
            int x_ = projectiles[p][1];
            int y_ = projectiles[p][2];
            if (x == x_ && y == y_)
            {
                return p;
            }
        }
    }
    return false2;
}

int move_player(int dx,int dy)
{
    if (px + dx <= W - 1 && px + dx >= 0 && py + dy <= H - 1 && py + dy >= 0)
    {
        if (is_wall(px + dx,py + dy) == false2)
        {
            return dx + dy; //um dos dois é 0... o_o
        }
    }
    return 0;
}

int move_enemy(int ex,int ey,int dx,int dy)
{
    if (ex + dx <= W - 1 && ex + dx >= 0 && ey + dy <= H - 1 && ey + dy >= 0)
    {
        if (is_wall(ex + dx,ey + dy) == false2 && ((ex + dx != px) || (ey + dy != py)))
        {
            if (is_enemy(ex + dx,ey + dy) == false2)
            {
                return dx + dy; //um dos dois é 0... o_o
            }
        }
    }
    return 0;
}

void attack(int dx,int dy)
{
    if ((dx*dx) + (dy*dy) <= weapon_data(pWeapon,1))
    {
        if (pWeapon != BOW)
        {
            for (int e = 0;e < MAX_ENEMY_AMMOUNT;e ++)
            {
                if (enemies[e][2] != 0)
                {
                    if (enemies[e][0] == px + dx && enemies[e][1] == py + dy)
                    {
                        enemies[e][2] --;
                        printf("ENEMY WAS ATTACKED!!! ENEMY HP: %d\n",enemies[e][2]);
                        break;
                    }
                }
            }
        }
        else { //Ranged weapon

            if (pAmmo > 0)
            {
                for (int p = 0;p < MAX_PROJ_AMMOUNT;p ++)
                {
                    if (projectiles[p][0] == -1) //Creating Projectile
                    {
                        projectiles[p][0] = ARROW; //technically player's ammo type
                        projectiles[p][1] = px; //x1
                        projectiles[p][2] = py; //y1
                        projectiles[p][3] = px + dx; //x2
                        projectiles[p][4] = py + dy; //y2
                        pAmmo --;
                        break;
                    }
                }
            }
            else {
                printf("No Ammo!!!");
            }
        }
    }
    else {
        printf("Out of Range!!!");
    }
}

int cll_path(int x1,int y1,int x2,int y2)
{

    int ix = x1;
    int iy = y1;

    while (ix != x2 || iy != y2)
    {
        int dx = (x2 - ix);
        int dy = (y2 - iy);
        
        ix += sign(dx);
        if (is_wall(ix,iy) != false2)
        {
            return false;
        }
        
        iy += sign(dy);
        if (is_wall(ix,iy) != false2)
        {
            return false;
        }

    }

    return true;

}

int main()
{
    printf("Starting...\n");

    create_mapa();
    set_enemies();
    set_projectiles();

    //alterations
    for (int i = 0;i < 3;i ++)
    {
        mapa[4][i + 1] = WALL;
    }
    mapa[5][3] = WALL;
    for (int i = 0;i < 4;i ++)
    {
        mapa[0][2 + i] = WATER;
    }
    mapa[13][2] = TREE;
    mapa[13][3] = TREE;
    mapa[14][3] = TREE;
    
    enemies[0][0] = W - 1;//x
    enemies[0][1] = H - 1;//y
    enemies[0][2] = 2;//hp
    
    enemies[1][0] = W - 3;//x
    enemies[1][1] = H - 1;//y
    enemies[1][2] = 2;//hp

    //initializing variables
    int step = 0;

    //game loop
    while (1 == 1)
    {
        char cmd_,cmd_a;

        //getting hit by projectiles
        //to-do

        if (pHp <= 0)
        {
            break;
        }

        printf("step: %d    ❤ : %d   Weapon: %s",step,pHp,char_converter(pWeapon));
        if (pWeapon == BOW) {printf(" Ammo: %d",pAmmo);}
        print_mapa();
        scanf("%c",&cmd_);//this guy is 0x0A (enter) dont exactly know how it works...
        scanf("%c",&cmd_a);

        if ((int) cmd_ == 10) {cmd_ = cmd_a;}

        if (cmd_ == 'w') {py += move_player(0,-1);}
        if (cmd_ == 's') {py += move_player(0,+1);}
        if (cmd_ == 'a') {px += move_player(-1,0);}
        if (cmd_ == 'd') {px += move_player(+1,0);}
        if (cmd_ == 'x')
        {
            int dx,dy;

            printf("prepare to attack!!\n");
            scanf("%d %d0x0A",&dx,&dy);
            scanf("%c",&cmd_a);

            attack(dx,dy);
        }

        //enemy AI
        for (int e = 0;e < MAX_ENEMY_AMMOUNT;e ++)
        {
            if (enemies[e][2] < 0) {enemies[e][2] = 0;}
            if (enemies[e][2] != 0)
            {
                //getting attacked
                int is_proj = is_projectile(enemies[e][0],enemies[e][1]);
                if (is_proj != false2)
                {
                    enemies[e][2] -= weapon_data(projectiles[is_proj][0],0);
                    projectiles[is_proj][0] = -1; //destroyed
                    printf("ENEMY WAS SHOT!!! ENEMY HP: %d\n",enemies[e][2]);
                }

                //moving and attacking
                if (cll_path(enemies[e][0],enemies[e][1],px,py) == true)
                {
                    if (distance(enemies[e][0],enemies[e][1],px,py) > 1)
                    {
                        int dx = sign(px - enemies[e][0]);
                        int dy = sign(py - enemies[e][1]);
                        enemies[e][0] += move_enemy(enemies[e][0],enemies[e][1],dx,0);
                        enemies[e][1] += move_enemy(enemies[e][0],enemies[e][1],0,dy);
                    }
                    else {
                        pHp --;
                        printf("|Ouch! -1 Hp|\n");
                    }
                }
            }
        }

        //Projectiles
        for (int p = 0;p < MAX_PROJ_AMMOUNT;p ++)
        {
            if (projectiles[p][0] != -1)
            {
                //attacking
                //getting attacked
                int is_en = is_enemy(projectiles[p][1],projectiles[p][2]);
                if (is_en != false2)
                {
                    enemies[is_en][2] -= weapon_data(projectiles[p][0],0);
                    projectiles[p][0] = -1; //destroyed
                    printf("ENEMY WAS SHOT!!! ENEMY HP: %d\n",enemies[is_en][2]);
                }

                int dx = projectiles[p][3] - projectiles[p][1];
                int dy = projectiles[p][4] - projectiles[p][2];

                //reached destination
                if (dx == 0 && dy == 0)
                {
                    projectiles[p][0] = -1; //destroyed
                }
                else {
                    if (is_wall(projectiles[p][1] + sign(dx),projectiles[p][2] + sign(dy)) == false2)
                    {
                        projectiles[p][1] += sign(dx); //x1
                        projectiles[p][2] += sign(dy); //y1
                    }
                    else { //hitting a wall
                        projectiles[p][0] = -1; //destroyed
                    }
                }
            }
        }
        step ++;
    }

    printf("\nGAME OVER");

    return 0;
}


//keep in mind
/*
typedef struct{
    int katana_atk = 1;
} weapon_data;
*/
