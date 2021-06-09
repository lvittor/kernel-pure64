#include <video.h>
#include <font.h>

vbe_mode_info * vbeInfo = 0x5C00;

Color PURPLE = {.r = 0x88, .g = 0x00, .b = 0xFF};
Color WHITE = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
Color BLACK = {.r = 0x00, .g = 0x00, .b = 0x00};

void init_screen(void){
    vbeInfo = (vbe_mode_info *) VBEModeInfoBlockAddress;
}

uint16_t getScreenWidth() {
    return vbeInfo->width;
}

uint16_t getScreenHeight() {
    return vbeInfo->height;
}

int getBaseXPixel(prompt_info * p){
    return p->x * font.Width;
}


int getBaseYPixel(prompt_info * p){
    return p->y * font.Height;
}

static void setPixel(int x, int y, Color * color) {
    uint8_t * screen = (uint8_t *) ((uint64_t) vbeInfo->framebuffer + x * vbeInfo->bpp / 8 + (int) y * vbeInfo->pitch);
    screen[0] = color->b;
    screen[1] = color->g;
    screen[2] = color->r;
}

void drawShellBorder(Color * color) {
    for (int j = 0; j < vbeInfo->height; j++)
        for (int i = -2; i < 3; i++)
            setPixel(vbeInfo->width / 2 + i, j, color);
}

void fillScreen(Color * color) {
    for (int i = 0; i < vbeInfo->width; i++)
        for (int j = 0; j < vbeInfo->height; j++)
            setPixel(i, j, color);
}

void scrollUp(prompt_info * p, Color * backgroundColor){
    for (int dy = 0; dy < p->windowHeight - font.Height; dy++) {
        for (int dx = 0; dx < p->windowWidth; dx++) {
            int x = p->baseX + dx;
            int yTo = p->baseY + dy;
            int yFrom = p->baseY + dy + font.Height;
            uint8_t * screenTo = (uint8_t *) ((uint64_t) vbeInfo->framebuffer + x * vbeInfo->bpp / 8 + (int) yTo * vbeInfo->pitch);
            uint8_t * screenFrom = (uint8_t *) ((uint64_t) vbeInfo->framebuffer + x * vbeInfo->bpp / 8 + (int) yFrom * vbeInfo->pitch);
            for (int i = 0; i < 3; i++)
                screenTo[i] = screenFrom[i];
        }
    }
}

void drawChar(prompt_info * p, char c, Color * fontColor, Color * backgroundColor) {
    if (getBaseXPixel(p) + font.Width - 1 >= p->windowWidth) {
        p->x = 0;
        p->y++;
    }

    if (getBaseYPixel(p) + font.Height - 1 >= p->windowHeight) {
        scrollUp(p, backgroundColor);
        // TODO: clear last line and decrease p->y;
    }

    
    uint8_t * bitMapCharacter = bitMap(c);
    int baseXPixel = p->x * font.Width + p->baseX;
    int baseYPixel = p->y * font.Height + p->baseY;
    for (int i = 0; i < font.Height; i++) {
        for (int j = 0; j < font.Width; j++) {
            if (bitMapCharacter[i] & 1 << (font.Width - j - 1)) // pregunta si el bit numero i esta prendido
                setPixel(baseXPixel + j, baseYPixel + i, fontColor);
            else
                setPixel(baseXPixel + j, baseYPixel + i, backgroundColor);
        }
    }
    p->x++;
}