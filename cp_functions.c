#include <stdlib.h>
#include "cp_functions.h"

#define True  1
#define False 0

SDL_Surface *screen = NULL;
unsigned int message_texture;

Texture cpLoadTexture(const char *filename)
{  unsigned int tex_id;
   int type;
   Texture texture;

	SDL_Surface *image = IMG_Load(filename);
	if (image == NULL)
		return NULL;

   texture = (Texture)malloc(sizeof(Texture *));
   if (texture == NULL) {
      SDL_FreeSurface(image);
      return NULL;
   }

   if (image->format->BytesPerPixel == 3)
      type = GL_RGB;
   else
      type = GL_RGBA;

   glGenTextures(1, &tex_id);
   glBindTexture(GL_TEXTURE_2D, tex_id);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexImage2D(GL_TEXTURE_2D, 0, type, image->w, image->h, 0, type,
   	GL_UNSIGNED_BYTE, image->pixels);

   texture->tex_id = tex_id;
   texture->width = image->w;
   texture->height = image->h;
   SDL_FreeSurface(image);

   return texture;
}

Font cpLoadFont(const char *font_file, int font_size)
{
   return TTF_OpenFont(font_file, font_size);
}

Music cpLoadMusic(const char *music_file)
{
   return Mix_LoadMUS(music_file);
}

Sound cpLoadSound(const char *sound_file)
{
   return Mix_LoadWAV(sound_file);
}

void cpPlayMusic(Music music)
{
   Mix_PlayMusic(music, -1);
}

void cpPlaySound(Sound sound)
{
   Mix_PlayChannel(-1, sound, 0);
}

void cpFreeFont(Font font)
{
   TTF_CloseFont(font);
}

void cpFreeMusic(Music music)
{
   Mix_FreeMusic(music);
}

void cpFreeSound(Sound sound)
{
   Mix_FreeChunk(sound);
}

int cpInit(const char *title, int win_width, int win_height)
{
   freopen("CON", "w", stdout);
   freopen("CON", "w", stderr);

   if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
      return False;

   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   SDL_WM_SetCaption(title, NULL);

   screen = SDL_SetVideoMode(win_width, win_height, 32, SDL_OPENGL);
   if (screen == NULL)
      return False;

   if (TTF_Init() == -1)
      return False;

   if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
      return False;

   glClearColor(0, 0, 0, 1);
   glViewport(0, 0, win_width, win_height);
   glShadeModel(GL_SMOOTH);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, win_width, win_height, 0, 0, 1);
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glGenTextures(1, &message_texture);

   return True;
}

void cpCleanUp()
{
   SDL_Quit();
}

void cpSwapBuffers()
{
   SDL_GL_SwapBuffers();
}

void cpDelay(int millisecond)
{
   SDL_Delay(millisecond);
}

int cbPollEvent(Event *event)
{
   SDL_PollEvent(event);
}

void cpDrawTexture(int r, int g, int b,
   int x, int y, int width, int height, Texture texture)
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture->tex_id);
   glColor4ub(r, g, b, 255);
   glBegin(GL_QUADS);
      glTexCoord2d(0, 0); glVertex2f(x, y);
      glTexCoord2d(1, 0); glVertex2f(x + width, y);
      glTexCoord2d(1, 1); glVertex2f(x + width, y + height);
      glTexCoord2d(0, 1); glVertex2f(x, y + height);
   glEnd();
   glDisable(GL_TEXTURE_2D);
}

void cpDrawText(int r, int g, int b,
   int x, int y, const char *text, Font font, int center)
{  SDL_Surface *message;
   SDL_Color color = {r, g, b};
   SDL_Rect offset;
   int texture_format, xb, xe, yb, ye;

   message = TTF_RenderUTF8_Blended(font, text, color);
   if (message == NULL)
      return;

   if (message->format->BytesPerPixel == 3)
      texture_format = GL_RGB;
   else
      texture_format = GL_RGBA;

   if (center) {
      xb = -message->w/2;
      xe = message->w/2;
      yb = -message->h/2;
      ye = message->h/2;
   }
   else {
      xb = 0;
      xe = message->w;
      yb = 0;
      ye = message->h;
   }
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, message_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, texture_format, message->w, message->h, 0,
      texture_format, GL_UNSIGNED_BYTE, message->pixels);
   glColor4ub(r, g, b, 255);
   glBegin(GL_QUADS);
      glTexCoord2d(0, 0); glVertex2f(x + xb, y + yb);
      glTexCoord2d(1, 0); glVertex2f(x + xe, y + yb);
      glTexCoord2d(1, 1); glVertex2f(x + xe, y + ye);
      glTexCoord2d(0, 1); glVertex2f(x + xb, y + ye);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   SDL_FreeSurface(message);
}

void cpClearScreen()
{
   glClear(GL_COLOR_BUFFER_BIT);
}
