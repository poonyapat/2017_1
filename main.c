#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "cp_functions.h"


#define True  1
#define False 0
#define WindowTitle  "The Juke Ball"
#define WindowWidth  1280
#define WindowHeight 800


Sound hit_paddle_sound, hit_brick_sound;
Sound hit_top_sound, end_sound;
Texture paddle_texture, ball_texture;
Texture brick_texture, bg_menu_texture, bg_pausing_texture;
Texture bg_stage1_texture,bg_stage2_texture, bg_stage3_texture;
Texture item_lifeup_texture, item_longerpad_texture, item_point_texture, item_revive_texture, item_speed_texture;
Font minimal_font, big_brad_font, small_brad_font;
//setting ball & paddle
float defaultBallX = WindowWidth/2-12, defaultBallY = 500,defaultBallsize = 18, defaultBallVel_x = 0, defaultBallVel_y = 10, defaultBallVel_r = 10;
float defaultPadX = WindowWidth/2-62, defaultPadY = WindowHeight-50, defaultPadWidth = 124, defaultPadVel_x = 20,defaultPadVel_r = 20;
int mode,maxSpd=10;
char pcName[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

// Structure for storing info for objects, i.e. Paddle, Brick, Ball.
typedef struct
{
   float x, y;
   float width, height;
   int destroyed;
   float vel_x, vel_y, vel_r;
} Object;

Object drop = {0,0,50,50,1,0,0,0};

typedef struct
{
   float brickxpos, brickypos, brickspos;
   int n_bricks, item_drop_rate, levelnum;
   float ballsize, paddlesize, morespd;

} Level;

//Level zero = {1,1,0,80,0,0,1,1,0,bg_stage1_texture};
Level one = {2.5,1.5,1.25,47,20,1,1,1,0};
Level two = {1,1.5,2,97,12,2,0.8,0.8,5,};
Level three = {1.5,1.5,0.75,110,5,3,0.5,0.5,10};

// Initial routine to load sounds, textures, and fonts.
int game_init()
{
   hit_paddle_sound = cpLoadSound("./sound/hitDown.wav");
   hit_brick_sound = cpLoadSound("./sound/hitUp.wav");
   hit_top_sound = cpLoadSound("./sound/hitTop.wav");
   end_sound = cpLoadSound("./sound/theEnd.wav");

   paddle_texture = cpLoadTexture("./texture/object/paddle.png");
   ball_texture = cpLoadTexture("./texture/object/ball.png");
   brick_texture = cpLoadTexture("./texture/object/brick.png");
   bg_stage1_texture = cpLoadTexture("./texture/background/bg_stage1.png");
   bg_stage2_texture = cpLoadTexture("./texture/background/bg_stage2.png");
   bg_stage3_texture = cpLoadTexture("./texture/background/bg_stage3.png");
   bg_menu_texture = cpLoadTexture("./texture/background/bg_menu.png");
   bg_pausing_texture = cpLoadTexture("./texture/background/bg_pausing.png");

   item_lifeup_texture = cpLoadTexture("./texture/object/item_lifeup.png");
   item_longerpad_texture = cpLoadTexture("./texture/object/item_longerpad.png");
   item_point_texture = cpLoadTexture("./texture/object/item_point.png");
   item_revive_texture = cpLoadTexture("./texture/object/item_revive.png");
   item_speed_texture = cpLoadTexture("./texture/object/item_speed.png");

   minimal_font = cpLoadFont("./font/bangna-new.ttf",30);
   big_brad_font = cpLoadFont("./font/BRADHITC.ttf",60);
   small_brad_font = cpLoadFont("./font/BRADHITC.ttf",30);

   if (hit_paddle_sound == NULL || hit_brick_sound == NULL ||
      hit_top_sound == NULL || end_sound == NULL ||
      paddle_texture == NULL || ball_texture == NULL ||
      brick_texture == NULL || bg_stage1_texture == NULL ||
      bg_menu_texture == NULL)
      return False;
   return True;
}

int menu(Event event)
{
   int on_menu, choice = 1, cnt_pName = 0;
   char showName[100];
   on_menu = True;
   mode = 2;
   while (on_menu){
      cpClearScreen();
      sprintf(showName,"Your Name is %s",pcName);
      //Draw All
      cpDrawTexture(255, 255, 255, 0, 0,
       WindowWidth, WindowHeight, bg_menu_texture);
      cpDrawText(255, 255, 255, WindowWidth/2, 20,
       showName, minimal_font, 1);
      cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight/5,
       "New Game", choice==1?big_brad_font:small_brad_font, 1);
      cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight*0.4,
       "Score Board", choice==2?big_brad_font:small_brad_font, 1);
      cpDrawText(255, mode == 1? 0 : 255, 255, WindowWidth/2-200, WindowHeight*0.6,
       "Easy", choice==3 && mode == 1?big_brad_font:small_brad_font, 1);
      cpDrawText(255, mode == 2? 0 : 255, 255, WindowWidth/2, WindowHeight*0.6,
       "Normal", choice==3 && mode == 2?big_brad_font:small_brad_font, 1);
      cpDrawText(255, mode == 3? 0 :255, 255, WindowWidth/2+200, WindowHeight*0.6,
       "Hard", choice==3 && mode == 3?big_brad_font:small_brad_font, 1);
      cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight*0.8,
       "Exit", choice==4?big_brad_font:small_brad_font, 1);

      while(cbPollEvent(&event)){
         if (event.type == KEYUP){
            //choose choice
            if (event.key.keysym.sym == K_DOWN && choice<4)
               choice++;
            else if (event.key.keysym.sym == K_UP && choice>1)
               choice--;
            else if (event.key.keysym.sym == K_LEFT && choice==3 && mode >1)
               mode--;
            else if (event.key.keysym.sym == K_RIGHT && choice==3 && mode <3) 
               mode++;
            else if (event.key.keysym.sym == K_RETURN){
               if (choice == 1)
                  return 1;
               else if (choice ==2)
                  return 2;
               else if (choice ==4){
                  cpCleanUp();
                  return 0;
               }
            }
            else if (event.key.keysym.sym == K_ESCAPE){
               cpCleanUp();
               return 0;
            }
            else if (event.key.keysym.sym == K_BCKSPC && cnt_pName != 0)
               pcName[--cnt_pName] = 0;
            //key name
            else if (event.key.keysym.sym >= 32 &&
             event.key.keysym.sym <= 126 && cnt_pName < 12 && event.key.keysym.sym != ' ')
               pcName[cnt_pName++] = event.key.keysym.sym;
         }
      }
      cpSwapBuffers();
      
   }
}

int newHghScr(int nscore, float nspd, Event event)
{
   FILE *board;

   int numOfHScr = 11;

   char name[11][13],nameSpare[13];
   int score[11],scoreSpare;
   float spd[11],spdSpare;

   board = fopen("./data/scrBoard","r");

   for (int i=0;i<11;i++){
      fscanf(board,"%s\t%d\t%f\n",
         &name[i],&score[i],&spd[i]);
   }
   strcpy(name[10],pcName[0]!=0?pcName:"Anonymous");
   score[10] = nscore;
   spd[10] = nspd;

   for (int i=10;i>0;i--){
      if (score[i]>score[i-1] ||
       score[i]==score[i-1] && spd[i]>spd[i-1]){
         strcpy(nameSpare,name[i-1]);
         scoreSpare = score[i-1];
         spdSpare = spd[i-1];
         strcpy(name[i-1],name[i]);
         score[i-1] = score[i];
         spd[i-1] = spd[i];
         strcpy(name[i],nameSpare);
         score[i] = scoreSpare;
         spd[i] = spdSpare;
      }
      else
         break;
   }
   fclose(board);

   board = fopen("./data/scrBoard","w");
   for (int i=0;i<10;i++){
      fprintf(board,"%s\t%d\t%.1f\n",
         name[i],score[i],spd[i]);
   }
   fclose(board);

   return 0;
}

int scrBrd(Event event)
{
   cpClearScreen();
   cpDrawTexture(255, 255, 255, 0, 0,
    WindowWidth, WindowHeight, bg_menu_texture);
   cpDrawText(255, 255, 255, WindowWidth/2, 150,
    "High Score", big_brad_font, 1);

   FILE *board;
   char name[13],scoreStr[20],spdStr[20];
   int score;
   float spd;
   board = fopen("./data/scrBoard","r");
   for (int i=0;i<11;i++){
      fscanf(board,"%s\t%d\t%f",&name,&score,&spd);
      sprintf(scoreStr,"%d",score);
      sprintf(spdStr,"%.1f",spd);
      cpDrawText(255, 255, 255, WindowWidth/4, WindowHeight*(5+i)/20,
       name, minimal_font, 0);
      cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight*(5+i)/20,
       scoreStr, minimal_font, 0);
      cpDrawText(255, 255, 255, WindowWidth*0.75, WindowHeight*(5+i)/20,
       spdStr, minimal_font, 0);
   }
   cpSwapBuffers();
   while(1){
      cbPollEvent(&event);
      if (event.type == KEYUP && (event.key.keysym.sym == K_RETURN || event.key.keysym.sym == K_BCKSPC))
         return 1;
      else if (event.key.keysym.sym == K_ESCAPE)
         return 0;
   }

}

// Collision Detection between two objects a and b
int collide(Object a, Object b)
{
   if (a.x + a.width  < b.x || b.x + b.width  < a.x ||
       a.y + a.height < b.y || b.y + b.height < a.y)
      return False;
   else
      return True;
}

int stage(int life,int score,Level level,Object ball,Object paddle,Event event)
{
   int showlv,running, n_hits = 0, revivable = False;
   float paddle_vel_x = 0, maxSpeed = 10;
   char msg[80],pcName[13];
   float pointMulti = 1;

   paddle.x = defaultPadX;
   paddle.y = defaultPadY;
   paddle.width = defaultPadWidth*level.paddlesize*(4-mode)/2;
   paddle.vel_x = defaultPadVel_x;
   paddle.vel_r = defaultPadVel_r;

   ball.x = defaultBallX;
   ball.y = defaultBallY;
   ball.width = defaultBallsize*level.ballsize;
   ball.height = defaultBallsize*level.ballsize;
   ball.vel_x = defaultBallVel_x;
   ball.vel_y = defaultBallVel_y*mode/2;
   ball.vel_r = defaultBallVel_r*mode/2.;
   ball.vel_r += level.morespd;

   Object bricks[level.n_bricks];

   drop.destroyed = 1;

   for (int n = 0, x = 35, y = 80, z = 0, flip = 0; n < level.n_bricks; n++) {
      bricks[n].width = 65;
      bricks[n].height = 25;
      if (z&1)
         x += bricks[n].width*level.brickspos;
      if (x > WindowWidth-bricks[n].width) {
         x  = 35;
         y += bricks[n].height*level.brickypos;
         z++;
      }
      bricks[n].x = x;
      bricks[n].y = y;
      bricks[n].destroyed = False;
      x += bricks[n].width*level.brickxpos;
   }
   showlv = True;
   running = True;
   int c=0,h=0,e=0,a=0,t=0,cheat2=0,cheat3=0; // for cheat
   int option; //for item
   while (running) {
      cpClearScreen();
      // Draw background
      if (level.levelnum==1)
         cpDrawTexture(255, 255, 255,
          0, 0, WindowWidth, WindowHeight, bg_stage1_texture);
      else if (level.levelnum == 2)
         cpDrawTexture(255, 255, 255,
          0, 0, WindowWidth, WindowHeight, bg_stage2_texture);
      else if (level.levelnum == 3)
         cpDrawTexture(255, 255, 255,
          0, 0, WindowWidth, WindowHeight, bg_stage3_texture);
      // Draw paddle
      cpDrawTexture(255, 255, 255,
         paddle.x, paddle.y, paddle.width, paddle.height, paddle_texture);
      // Draw ball
      cpDrawTexture(255, 255, 255,
         ball.x, ball.y, ball.width, ball.height, ball_texture);
      // Display score
      sprintf(msg, "Point: %d  Life: %d  BallSpd: %.1f", score, life,ball.vel_r);
      cpDrawText(255, 255, 255, 3, 3, msg, minimal_font, 0);

      // Draw bricks
      for (int n = 0; n < level.n_bricks; n++) {
         if (!bricks[n].destroyed)
            cpDrawTexture(255, 255, 255,
               bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
               brick_texture);
      }

      if (!drop.destroyed){
         if (option == 0)
            cpDrawTexture(255, 255, 255,
             drop.x, drop.y, drop.width, drop.height, item_lifeup_texture);
         else if (option == 1)
            cpDrawTexture(255, 255, 255,
             drop.x, drop.y, drop.width, drop.height, item_longerpad_texture);
         else if (option == 2)
            cpDrawTexture(255, 255, 255,
             drop.x, drop.y, drop.width, drop.height, item_point_texture);
         else if (option == 3)
            cpDrawTexture(255, 255, 255,
             drop.x, drop.y, drop.width, drop.height, item_revive_texture);
         else if (option == 4)
            cpDrawTexture(255, 255, 255,
             drop.x, drop.y, drop.width, drop.height, item_speed_texture);
         drop.y += drop.vel_y;

         if (collide(paddle,drop)){
            drop.destroyed =1;
            if (option == 0)
               life++;
            else if (option == 1)
               paddle.width*=1.2;
            else if (option == 2)
               pointMulti+=0.5;
            else if (option == 3)
               revivable = True;
            else if (option == 4)
               ball.vel_r+=5;
         }else if (drop.y >= WindowHeight)
            drop.destroyed =1;
      }


      if (n_hits == level.n_bricks && level.levelnum!=3){
         cpClearScreen();
         life++;
         if (level.levelnum == 1)
            return stage(life,score,two,ball,paddle,event);
         else if (level.levelnum ==2)
            return stage(life,score,three,ball,paddle,event);
      }
      // If the ball has left the screen or all bricks were destroyed,
      // die
      if (ball.y + ball.width > WindowHeight) {
         cpPlaySound(end_sound);
         paddle.x = defaultPadX;
         paddle.y = defaultPadY;
         paddle.width = defaultPadWidth*level.paddlesize*(5-mode)/3;
         paddle.vel_x = defaultPadVel_x;
         paddle.vel_r = defaultPadVel_r;

         ball.x = defaultBallX;
         ball.y = defaultBallY;
         ball.width = defaultBallsize*level.ballsize;
         ball.height = defaultBallsize*level.ballsize;
         ball.vel_x = defaultBallVel_x;
         ball.vel_y = defaultBallVel_y*mode/2;
         ball.vel_r = defaultBallVel_r*mode/2;
         ball.vel_r += level.morespd;
         if (!revivable)
            life--;
         drop.destroyed = 1;
         cpDelay(1000);
      }
      //game over
      if (life == -1 || n_hits == level.n_bricks && level.levelnum==3){

         cpDrawText(255, 255, 0, WindowWidth/2, WindowHeight*2/3, "END GAME", big_brad_font, 1);
         cpSwapBuffers();
         newHghScr(score,maxSpeed,event);
         while (1) {
            cbPollEvent(&event);
            if (event.type == QUIT || event.type == KEYUP && event.key.keysym.sym == K_ESCAPE) {
               return 0;
            }else if (event.type == KEYUP && event.key.keysym.sym == K_RETURN)
               return 1;
         }
      }
      // All drawn objects are now displayed.
      if (showlv){
         char lv[10];
         sprintf(lv,"Level %d",level.levelnum);
         cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight*2/3, lv, minimal_font, 1);
         showlv--;
         cpSwapBuffers();
         cpDelay(2000);
      }
      else
         cpSwapBuffers();

      // Get an event from event queue.
      while (cbPollEvent(&event)) {
         // If the window is close or Esc key is just released,
         // then the program is about to exit main loop.
         if (event.type == QUIT)
            return 0;

         // Paddle left, right control routine
         if (event.type == KEYDOWN) {
            if (event.key.keysym.sym == K_LEFT)
               paddle_vel_x = -paddle.vel_x;
            if (event.key.keysym.sym == K_RIGHT)
               paddle_vel_x = paddle.vel_x;
         }
         else
         if (event.type == KEYUP) {
            if (event.key.keysym.sym == K_LEFT && paddle_vel_x < 0)
               paddle_vel_x = 0;
            else if (event.key.keysym.sym == K_RIGHT && paddle_vel_x > 0)
               paddle_vel_x = 0;
            else if (event.key.keysym.sym == K_ESCAPE){
               int pausing = 1,choice=1;
               while (pausing){
                  cpDrawTexture(255, 255, 255,
                   400, WindowHeight/3, 500, WindowHeight/3, bg_pausing_texture);
                  cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight*0.4,
                   "Return to Game", choice==1?big_brad_font:small_brad_font, 1);
                  cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight/2,
                   "Back to MainMenu", choice==2?big_brad_font:small_brad_font, 1);
                  cpDrawText(255, 255, 255, WindowWidth/2, WindowHeight*0.6,
                   "Exit Game", choice==3?big_brad_font:small_brad_font, 1);
                  cpSwapBuffers();
                  while (cbPollEvent(&event)){
                     if (event.type == KEYUP && event.key.keysym.sym == K_UP && choice >1)
                        choice--;
                     else if (event.type == KEYUP && event.key.keysym.sym == K_DOWN && choice <3)
                        choice++;
                     else if (event.type == KEYUP && event.key.keysym.sym == K_RETURN){
                        if (choice == 1){
                           pausing=0;
                           break;
                        }
                        else if (choice == 2)
                           return 1;
                        else if (choice == 3){
                           cpCleanUp();
                           return 0;
                        }
                     }
                  }
               }
            }
            else if (event.key.keysym.sym == 'p' || event.key.keysym.sym == 'P')
               cpDelay(3000);
            //else if (event.key.keysym.sym == ' ')
              // ball.vel_r+=5;
            else if (event.key.keysym.sym == 'c')
               c=1;
            else if (event.key.keysym.sym == 'h' && c)
               h=1;
            else if (event.key.keysym.sym == 'e' && c && h)
               e=1;
            else if (event.key.keysym.sym == 'a' && c && h && e)
               a=1;
            else if (event.key.keysym.sym == 't' && c && h &&e && a)
               t=1;
            else if (event.key.keysym.sym == SDLK_2 && c && h && e && a && t)
               c=0,h=0,e=0,a=0,t=0,cheat2=1;
            else if (event.key.keysym.sym == SDLK_3 && c && h && e && a && t)
               c=0,h=0,e=0,a=0,t=0,cheat3=1,cheat2=0;
            else if (event.key.keysym.sym == 'i' && c && h && e && a && t)
               level.item_drop_rate *= 50;
            else if (c || h || e || a || t || cheat2 || cheat3){
               c=0,h=0,e=0,a=0,t=0,cheat2=0,cheat3=0,paddle.width = defaultPadWidth*level.paddlesize;
               if (level.item_drop_rate >= 80)
                  level.item_drop_rate /=50;
            }
         }
      }
      // Move paddle according to its velocity and CHEAT

      if (c && h && e && a && t)
         paddle.x = ball.x-20;
      else if (cheat2)
         paddle.x = ball.x-20-paddle.width/2;
      else if (cheat3){
         paddle.x = 0;
         paddle.width = WindowWidth;
      }
      else
         paddle.x += paddle_vel_x;
      //paddle.x = ball.x-20;

      // Prevent paddle to not penetrate walls
      if (paddle.x < 0)
         paddle.x = 0;
      if (paddle.x + paddle.width > WindowWidth)
         paddle.x = WindowWidth - paddle.width;

      // Move ball according to its velocity
      ball.x += ball.vel_x;
      ball.y += ball.vel_y;

      // Prevent ball to not penetrate walls
      if (ball.x < 0 || ball.x + ball.width > WindowWidth){
         ball.vel_x = abs(ball.vel_x);
         if (ball.x + ball.width > WindowWidth)
            ball.vel_x = -ball.vel_x; 
      }

      // If ball hits ceiling, then it is bounced back.
      if (ball.y < 0) {
         cpPlaySound(hit_top_sound);
         ball.vel_y = -ball.vel_y;
      }

      // Bricks-Ball collision check
      for (int n = level.n_bricks-1; n >= 0; n--) {
         if (!bricks[n].destroyed &&
             collide(ball, bricks[n]) == True) {
            cpPlaySound(hit_brick_sound);
            // if (ball.x + ball.width > bricks[n].x + bricks[n].width || ball.x < bricks[n].x)
            //    ball.vel_x = ball.vel_r*2*(ball.x + (ball.x<bricks[n].x?ball.width:0) - bricks[n].x - bricks[n].width/2)/bricks[n].width;
            // ball.vel_y = -ball.vel_y/abs(ball.vel_y)*sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
            ball.vel_x = ball.vel_r*2*(ball.x + (ball.x<bricks[n].x?ball.width:0) - bricks[n].x - bricks[n].width/2)/bricks[n].width;
            if (ball.x + ball.width/2 < bricks[n].x + bricks[n].width/2 && ball.y + ball.height/2 < bricks[n].y + bricks[n].height/2){
               if (ball.y + ball.height/2 - bricks[n].y > ball.x + ball.width/2 - bricks[n].x)
                  ball.vel_y = sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
               else
                  ball.vel_y = -ball.vel_y/abs(ball.vel_y)*sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
            }else if (ball.x + ball.width/2 < bricks[n].x + bricks[n].width/2 && ball.y + ball.height/2 > bricks[n].y + bricks[n].height/2){
               if (bricks[n].y + bricks[n].height - ball.y - ball.height/2 > ball.x + ball.width/2 -bricks[n].x)
                  ball.vel_y = sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
               else
                  ball.vel_y = -ball.vel_y/abs(ball.vel_y)*sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
            }else if (ball.x + ball.width/2 > bricks[n].x + bricks[n].width/2 && ball.y + ball.height/2 < bricks[n].y + bricks[n].height/2){
               if (ball.y + ball.height/2 - bricks[n].y > bricks[n].x +bricks[n].width - ball.x - ball.width/2)
                  ball.vel_y = sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
               else
                  ball.vel_y = -ball.vel_y/abs(ball.vel_y)*sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
            }else if (ball.x + ball.width/2 > bricks[n].x + bricks[n].width/2 && ball.y + ball.height/2 > bricks[n].y + bricks[n].height/2){
               if (bricks[n].y + bricks[n].height - ball.y - ball.height/2 > bricks[n].x +bricks[n].width - ball.x - ball.width/2)
                  ball.vel_y = sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
               else
                  ball.vel_y = -ball.vel_y/abs(ball.vel_y)*sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
            }
            bricks[n].destroyed = True;
            srand((int)(clock()/CLOCKS_PER_SEC));
            if (((rand()%100) <= level.item_drop_rate*(4-mode)*0.5) && drop.destroyed){
               int rand_int = rand()%20;
               if (rand_int <2)
                  option = 0; //life up
               else if (rand_int < 8)
                  option = 1; //longpad
               else if (rand_int < 14)
                  option = 2; //x2point
               else if (rand_int < 17)
                  option = 3; //save life
               else if (rand_int < 20)
                  option = 4; //more speed
               drop.x = bricks[n].x;
               drop.y = bricks[n].y;
               drop.vel_y = 7;
               drop.destroyed = 0;
            }
            n_hits++;
            score += (int)ball.vel_r*pointMulti;
            if (ball.vel_r < maxSpd){
               ball.vel_r  +=0.5*mode/2;
               if (ball.vel_r > maxSpeed)
                  maxSpeed = ball.vel_r;
               paddle.vel_x  +=0.5*mode/2;
            }
            
            break;
         }
      }

      // Paddle-Ball collision check
      if (collide(ball, paddle) == True && (ball.y + ball.height/2 <= paddle.y + ball.vel_y)) {
         cpPlaySound(hit_paddle_sound);
         ball.vel_x = ball.vel_r*2*(ball.x + (ball.x<paddle.x?ball.width:0) - paddle.x - paddle.width/2)/paddle.width;
         ball.vel_y = -1*sqrt(pow(ball.vel_r,2) - pow(ball.vel_x,2));
      }
      cpDelay(10);
   }
   cpCleanUp();
}

/* The main() function is called when the program starts. */
int main(int argc, char *args[])
{
   // Window Initialization
   if (cpInit(WindowTitle, WindowWidth, WindowHeight) == False) {
     printf("Window Initialization Failed!\n");
     exit(1);
   }
   // Game Initialization
   if (game_init() == False) {
     printf("Game Initialization Failed!\n");
     exit(1);
   }
   // set life and score
   int score,life; 
   // default ball
   Object ball = {defaultBallX, defaultBallY, defaultBallsize, defaultBallsize, False,
    0, defaultBallVel_y, defaultBallVel_r};
    // default paddle
   Object paddle = {defaultPadX, defaultPadY, defaultPadWidth, 30, False,
    defaultPadVel_x, 0, defaultPadVel_r};
    //event
   Event event;
   int choice;
   while(choice = menu(event)){
      if (choice == 1){
         life=2;
         score=0;
         maxSpd*=mode;
         life = life+1-mode;
         if(!stage(life,score,one,ball,paddle,event))
            break;
      }
      else if (choice == 2){
         if(!scrBrd(event))
            break;
      }
   }
   cpCleanUp();

   //stage(life,score,ball,paddle,event);
   // Brick setup
   

   return 0;
}
