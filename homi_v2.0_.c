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
#define MAX_ITEM_AMMOUNT 5
#define MAX_NPC_AMMOUNT 5
//thing's IDs
#define TYPE_PROJ 11 //projectiles have ids that are dividable by 11
#define TYPE_SOLID 13 //solids have ids that are dividable by 13
#define TYPE_ENEMY 17 //enemys have ids that are dividable by 17
#define TYPE_NPC 19 //npcs have ids that are dividable by 19
#define FRIENDLY_PROJ 3 //all friendly projectiles are dividable by 3
#define EVERYTHING 0 //theoretical...dont plan on using it :p
#define VOID 1
#define PLAYER 2
#define KATANA 4
#define BOW 8
#define AXE 16
#define HEALTH 32
#define CITY 64
#define PERSON 128
//
#define ARROW TYPE_PROJ * FRIENDLY_PROJ * 2
#define FIREBALL TYPE_PROJ * 4
//
#define WALL TYPE_SOLID * 2
#define WATER TYPE_SOLID * 4
#define TREE TYPE_SOLID * 8
#define HOUSE TYPE_SOLID * 16
//
#define ENEMY TYPE_ENEMY * 2
#define ENEMY2 TYPE_ENEMY * 4

////
//initializing variables
int step = 0;
int mapa[W][H]; //has walls, phys, etc... positions

//int solids[] = {WALL,WATER,TREE};

int enemies[MAX_ENEMY_AMMOUNT][4]; //x,y,Hp,Type
int projectiles[MAX_PROJ_AMMOUNT][5]; //x,y,xgo,ygo,type
int items[MAX_ITEM_AMMOUNT][3]; //type,x,y
int npcs[MAX_NPC_AMMOUNT][4]; //Hp,x,y,Type

int px = 0;
int py = 0;
int pHp = 3;
int pWeapon = KATANA;
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

int enemy_data (int enemy_type,int what_do_you_want)
{
    if (what_do_you_want == 0) //range (ao quadrado pq int e double smoafnwapo)
    {
        if (enemy_type == ENEMY)
        {
            return 1;
        }
        if (enemy_type == ENEMY2)
        {
            return 18; //3*sqrt(2)
        }
    }
    if (what_do_you_want == 1) //is ranged?
    {
        if (enemy_type == ENEMY)
        {
            return false;
        }
        if (enemy_type == ENEMY2)
        {
            return true;
        }
    }
    if (what_do_you_want == 2) //attack frequency x -> attacks each x steps
    {
        if (enemy_type == ENEMY)
        {
            return 1;
        }
        if (enemy_type == ENEMY2)
        {
            return 3;
        }
    }
    if (what_do_you_want == 3) //projectile type
    {
        if (enemy_type == ENEMY)
        {
            return false2;
        }
        if (enemy_type == ENEMY2)
        {
            return FIREBALL;
        }
    }
    if (what_do_you_want == 4) //Hp
    {
        if (enemy_type == ENEMY)
        {
            return 2;
        }
        if (enemy_type == ENEMY2)
        {
            return 1;
        }
    }


    return false2;
}

void load_mapa(int map_type)
{
    for (int j = 0;j < H;j ++)
    {
        for (int i = 0;i < W;i ++)
        {
            mapa[i][j] = VOID;
        }
    }
    if (map_type == 0)
    {
        for (int i = 0;i < 3;i ++)
        {
            mapa[4][i + 1] = WALL;
        }
        mapa[5][3] = WALL;
        mapa[W - 5][H - 3] = WALL;
        for (int i = 0;i < 4;i ++)
        {
            mapa[0][2 + i] = WATER;
        }
        mapa[13][2] = TREE;
        mapa[13][3] = TREE;
        mapa[14][3] = TREE;
        mapa[W - 3][H - 3] = CITY;

        enemies[0][0] = W - 1;//x
        enemies[0][1] = H - 1;//y
        enemies[0][2] = enemy_data(ENEMY2,4);//hp
        enemies[0][3] = ENEMY;//Type
        
        enemies[1][0] = W - 4;//x
        enemies[1][1] = 2;//y
        enemies[1][2] = enemy_data(ENEMY2,4);//hp
        enemies[1][3] = ENEMY2;//Type
    }
    if (map_type == 1)
    {
        for (int i = 0;i < W;i ++)
        {
            mapa[i][0] = WALL;
        }
        for (int i = 0;i < H;i ++)
        {
            mapa[W - 1][i] = WALL;
        }
        for (int i = 0;i < W;i ++)
        {
            mapa[i][H - 1] = WALL;
        }
        mapa[4][2] = HOUSE;mapa[4][H - 3] = HOUSE;
        mapa[7][2] = HOUSE;mapa[7][H - 3] = HOUSE;
        mapa[10][2] = HOUSE;mapa[10][H - 3] = HOUSE;

        npcs[0][0] = 1;
        npcs[0][1] = W - 4;
        npcs[0][2] = 3;
        npcs[0][3] = PERSON;
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

void set_items()
{
    for (int i = 0;i < MAX_ITEM_AMMOUNT;i ++)
    {
        for (int j = 0;j < 3;j ++)
        {
            items[i][j] = -1;
        }
    }
}

void set_npcs()
{
    for (int i = 0;i < MAX_NPC_AMMOUNT;i ++)
    {
        for (int j = 0;j < 3;j ++)
        {
            npcs[i][j] = -1;
        }
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

int is_item(int x,int y)
{ //is_x type functions return false2 or id of object
    for (int i = 0;i < MAX_ITEM_AMMOUNT;i ++)
    {
        if (items[i][0] != -1)
        {
            int x_ = items[i][1];
            int y_ = items[i][2];
            if (x == x_ && y == y_)
            {
                return i;
            }
        }
    }
    return false2;
}

int is_npc(int x,int y)
{ //is_x type functions return false2 or id of object
    for (int n = 0;n < MAX_NPC_AMMOUNT;n ++)
    {
        if (npcs[n][0] != -1)
        {
            int x_ = npcs[n][1];
            int y_ = npcs[n][2];
            if (x == x_ && y == y_)
            {
                return n;
            }
        }
    }
    return false2;
}

const char * char_converter(int v)
{
    if (v == VOID)      {return "ロ";}
    if (v == PLAYER)    {return "人";}
    if (v == WALL)      {return "壁";}
    if (v == ENEMY)     {return "か";}
    if (v == WATER)     {return "水";}
    if (v == TREE)      {return "木";}
    if (v == KATANA)    {return "刀";}
    if (v == BOW)       {return "弓";}
    if (v == AXE)       {return "斧";}
    if (v == ARROW)     {return "矢";}
    if (v == ENEMY2)    {return "げ";}
    if (v == FIREBALL)  {return "炎";}
    if (v == HEALTH)    {return "果";}
    if (v == CITY)      {return "街";}
    if (v == HOUSE)     {return "家";}
    if (v == PERSON)    {return "民";}
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
                int u;

                u = is_item(i,j);
                if (u != false2)
                {
                    int_son = items[u][0];
                }

                u = is_projectile(i,j);
                if (u != false2)
                {
                    int_son = projectiles[u][0];
                }

                u = is_npc(i,j);
                if (u != false2)
                {
                    int_son = npcs[u][3];
                }

                u = is_enemy(i,j);
                if (u != false2)
                {
                    int_son = enemies[u][3];
                }

                printf(char_converter(int_son));
            }
        }
        printf("\n");
    }
}

int drop_item(int x,int y,int item_type)
{
    if (is_item(x,y) == false2)
    {
        for (int i = 0;i < MAX_ITEM_AMMOUNT;i ++)
        {
            if (items[i][0] == -1)
            {
                items[i][0] = item_type;
                items[i][1] = x;
                items[i][2] = y;
                return i;
            }
        }
    }
    return false2;
}

int create_proj(int x1,int y1,int x2,int y2,int proj_type)
{
    for (int p = 0;p < MAX_PROJ_AMMOUNT;p ++)
    {
        if (projectiles[p][0] == -1) //Creating Projectile
        {
            projectiles[p][0] = proj_type; //technically player's ammo type
            projectiles[p][1] = x1;
            projectiles[p][2] = y1;
            projectiles[p][3] = x2;
            projectiles[p][4] = y2;
            return true;
        }
    }
    return false;
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

void enemy_killed(int enemy_id)
{
    if (enemies[enemy_id][2] <= 0)
    {
       if (step % 4 == 0)
        {
            int u = drop_item(enemies[enemy_id][0],enemies[enemy_id][1],HEALTH);
        }
    }
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
                        enemy_killed(e);
                        break;
                    }
                }
            }
        }
        else { //Ranged weapon

            if (pAmmo > 0)
            {
                if (create_proj(px,py,px + dx,py + dy,ARROW) == true)
                {
                    pAmmo --;
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

    set_enemies();
    set_projectiles();
    set_items();
    set_npcs();
    load_mapa(0);

    //alterations

    //initializing variables
    int u; //"useless" variable. receives output from action and "is_" functions

    //game loop
    while (1 == 1)
    {
        char cmd_,cmd_a;

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
        //if (cmd_ == 'q') {drop_item()}
        if (cmd_ == 'x')
        { //Attacking
            int dx,dy;

            printf("prepare to attack!!\n");
            scanf("%d %d0x0A",&dx,&dy);
            scanf("%c",&cmd_a);

            attack(dx,dy);
        }
        if (cmd_ == 'e')
        { //Interacting with stuff
            u = is_item(px,py);
            if (u != false2)
            {
                if (items[u][0] == HEALTH)
                {
                    pHp ++;
                    printf("FRUIT EATEN +1 HP");
                }
                items[u][0] = -1;
            }
            if (mapa[px][py] == CITY)
            {
                printf("Entering City...");
                set_enemies();
                set_projectiles();
                set_items();
                set_npcs();
                load_mapa(1);
                px = 0;
                py = (int) H/2;
            }
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
                    if (distance(enemies[e][0],enemies[e][1],px,py) > sqrt(enemy_data(enemies[e][3],0)))
                    {
                        int dx = sign(px - enemies[e][0]);
                        int dy = sign(py - enemies[e][1]);
                        enemies[e][0] += move_enemy(enemies[e][0],enemies[e][1],dx,0);
                        enemies[e][1] += move_enemy(enemies[e][0],enemies[e][1],0,dy);
                    }
                    else {
                        if (enemy_data(enemies[e][3],1) == 0) //ranged or not?
                        {
                            pHp --;
                            printf("|Ouch! -1 Hp|\n");
                        }
                        else {
                            //ranged attack
                            if (step % enemy_data(enemies[e][3],2) == 0)
                            {
                                u = create_proj(enemies[e][0],enemies[e][1],px,py,enemy_data(enemies[e][3],3));
                            }
                        }
                    }
                }
            }
        }

        //Projectiles
        for (int p = 0;p < MAX_PROJ_AMMOUNT;p ++)
        {
            if (projectiles[p][0] != -1)
            {

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

                if (projectiles[p][0] != -1)
                {
                    //attacking
                    if (projectiles[p][0] % FRIENDLY_PROJ == 0)
                    {
                        int is_en = is_enemy(projectiles[p][1],projectiles[p][2]);
                        if (is_en != false2)
                        {
                            enemies[is_en][2] -= weapon_data(projectiles[p][0],0);
                            projectiles[p][0] = -1; //destroyed
                            printf("ENEMY WAS SHOT!!! ENEMY HP: %d\n",enemies[is_en][2]);
                        }
                    }
                    else {
                        if (projectiles[p][1] == px && projectiles[p][2] == py)
                        {
                            pHp -= 1;
                            projectiles[p][0] = -1; //destroyed
                            printf("PLAYER WAS SHOT!!! -1 Hp");
                        }
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
