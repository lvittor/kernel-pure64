#include <font.h>
#include <lib.h>
#include <video.h>

static vbe_mode_info *vbeInfo;
static prompt_info *prompt;

Color PURPLE = {.r = 0x88, .g = 0x00, .b = 0xFF};
Color WHITE = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
Color BLACK = {.r = 0x00, .g = 0x00, .b = 0x00};
Color RED = {.r = 0xFF, .g = 0x00, .b = 0x00};

void declarePrompt(prompt_info *p) { prompt = p; }

prompt_info *getCurrentPrompt() { return prompt; }

void init_screen(void) { vbeInfo = (vbe_mode_info *)VBEModeInfoBlockAddress; }

uint16_t getScreenWidth() { return vbeInfo->width; }

uint16_t getScreenHeight() { return vbeInfo->height; }

uint64_t getXPixel(prompt_info *p) { return p->x * font.Width; }

uint64_t getYPixel(prompt_info *p) { return p->y * font.Height; }

static void setPixel(uint64_t x, uint64_t y, Color *color) {
  uint8_t *screen =
      (uint8_t *)((uint64_t)vbeInfo->framebuffer + x * vbeInfo->bpp / 8 +
                  (uint64_t)y * vbeInfo->pitch);
  screen[0] = color->b;
  screen[1] = color->g;
  screen[2] = color->r;
}

void drawShellBorder(Color *color) {
  for (uint64_t j = 0; j < vbeInfo->height; j++)
    for (uint64_t i = -2; i < 3; i++)
      setPixel(vbeInfo->width / 2 + i, j, color);
}

void fillScreen(Color *color) {
  for (uint64_t i = 0; i < vbeInfo->width; i++)
    for (uint64_t j = 0; j < vbeInfo->height; j++)
      setPixel(i, j, color);
}

static void scrollUp(prompt_info *p, Color *backgroundColor) {
  for (uint64_t dy = 0; dy < p->windowHeight - font.Height; dy++) {
    uint64_t x = (uint64_t)p->baseX;
    uint64_t yTo = (uint64_t)p->baseY + dy;
    uint64_t yFrom = p->baseY + dy + font.Height;
    uint8_t *to =
        (uint8_t *)((uint64_t)vbeInfo->framebuffer + x * vbeInfo->bpp / 8 +
                    (uint64_t)yTo * vbeInfo->pitch);
    uint8_t *from =
        (uint8_t *)((uint64_t)vbeInfo->framebuffer + x * vbeInfo->bpp / 8 +
                    (uint64_t)yFrom * vbeInfo->pitch);
    memcpy(to, from, p->windowWidth * vbeInfo->bpp / 8);
  }

  for (uint64_t y = p->windowHeight - font.Height; y < p->windowHeight; y++)
    for (uint64_t x = (uint64_t)p->baseX;
         x < (uint64_t)p->baseX + p->windowWidth; x++)
      setPixel(x, y, backgroundColor);
}

void clearWindow(prompt_info *p, Color *backgroundColor) {
  for (uint64_t x = 0; x < p->windowWidth; x++)
    for (uint64_t y = 0; y < p->y * font.Height; y++)
      setPixel((uint64_t)p->baseX + x, (uint64_t)p->baseY + y, backgroundColor);

  p->y = 0;
  p->x = 0;
}

static char isPromptOutOfWindowWidth(prompt_info *p) {
  return getXPixel(p) + font.Width - 1 >= p->windowWidth;
}

static char isPromptOutOfWindowHeight(prompt_info *p) {
  return getYPixel(p) + font.Height - 1 >= p->windowHeight;
}

void drawChar(prompt_info *p, char c, Color *fontColor,
              Color *backgroundColor) {
  if (isPromptOutOfWindowWidth(p)) {
    p->x = 0;
    p->y++;
  }

  if (isPromptOutOfWindowHeight(p)) {
    scrollUp(p, backgroundColor);
    p->y--;
  }

  uint8_t *bitMapCharacter = bitMap(c);
  uint64_t baseXPixel = p->x * font.Width + (uint64_t)p->baseX;
  uint64_t baseYPixel = p->y * font.Height + (uint64_t)p->baseY;
  for (uint64_t i = 0; i < font.Height; i++) {
    for (uint64_t j = 0; j < font.Width; j++) {
      if (bitMapCharacter[i] &
          1 << (font.Width - j -
                1)) // pregunta si el bit numero i esta prendido
        setPixel(baseXPixel + j, baseYPixel + i, fontColor);
      else
        setPixel(baseXPixel + j, baseYPixel + i, backgroundColor);
    }
  }
  p->x++;
}

void newLine(prompt_info *p, Color *backgroundColor) {
  p->x = 0;
  p->y++;
  if (isPromptOutOfWindowHeight(p)) {
    scrollUp(p, backgroundColor);
    p->y--;
  }
}

void eraseChar(prompt_info *p, Color *backgroundColor) {
  if (p->x == 0 && p->y == 0)
    return;

  if (p->x == 0) {
    p->x = p->windowWidth / font.Width - 1;
    p->y--;
  } else
    p->x--;

  uint64_t baseXPixel =
      (uint64_t)p->baseX + getXPixel(p); // Coordenadas absolutas
  uint64_t baseYPixel = (uint64_t)p->baseY + getYPixel(p);
  for (uint64_t y = baseYPixel; y < baseYPixel + font.Height; y++)
    for (uint64_t x = baseXPixel; x < baseXPixel + font.Width; x++)
      setPixel(x, y, backgroundColor);
}