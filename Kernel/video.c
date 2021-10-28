#include <video.h>
#include <font.h>
#include <lib.h>

vbe_mode_info * vbeInfo = (vbe_mode_info *)0x5C00;
prompt_info prompt;

Color PURPLE = {.r = 0x88, .g = 0x00, .b = 0xFF};
Color WHITE = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
Color BLACK = {.r = 0x00, .g = 0x00, .b = 0x00};
Color RED = {.r = 0xFF, .g = 0x00, .b = 0x00};

void init_screen(void){
    vbeInfo = (vbe_mode_info *) VBEModeInfoBlockAddress;
    prompt = (prompt_info) {	.x = 0,
								.y = 0,
							  	.baseX = 0,
							  	.baseY = 0,
							  	.windowWidth = getScreenWidth(),
							  	.windowHeight = getScreenHeight()
							};
}

uint16_t getScreenWidth() {
    return vbeInfo->width;
}

uint16_t getScreenHeight() {
    return vbeInfo->height;
}

int getXPixel(){
    return prompt.x * font.Width;
}


int getYPixel(){
    return prompt.y * font.Height;
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

static void scrollUp(Color * backgroundColor) {
    for (int dy = 0; dy < prompt.windowHeight - font.Height; dy++){
        int x = prompt.baseX;
        int yTo = prompt.baseY + dy;
        int yFrom = prompt.baseY + dy + font.Height;
        uint8_t * to = (uint8_t *) ((uint64_t) vbeInfo->framebuffer + x * vbeInfo->bpp / 8 + (int) yTo * vbeInfo->pitch);
        uint8_t * from = (uint8_t *) ((uint64_t) vbeInfo->framebuffer + x * vbeInfo->bpp / 8 + (int) yFrom * vbeInfo->pitch);
        memcpy(to, from, prompt.windowWidth * vbeInfo->bpp / 8);
    }
    
    for (int y = prompt.windowHeight - font.Height; y < prompt.windowHeight; y++)
        for (int x = prompt.baseX; x < prompt.baseX + prompt.windowWidth; x++) 
            setPixel(x, y, backgroundColor);
}

void clearWindow(Color * backgroundColor) {
    for (int x = 0; x < prompt.windowWidth; x++)
        for (int y = 0; y < prompt.y * font.Height; y++)
            setPixel(prompt.baseX + x, prompt.baseY + y, backgroundColor);

    prompt.y = 0;
    prompt.x = 0;
}

static char isPromptOutOfWindowWidth(void) {
    return getXPixel() + font.Width - 1 >= prompt.windowWidth;
}

static char isPromptOutOfWindowHeight(void) {
    return getYPixel() + font.Height - 1 >= prompt.windowHeight;
}

void drawChar(char c, Color * fontColor, Color * backgroundColor) {
    if (isPromptOutOfWindowWidth()) {
        prompt.x = 0;
        prompt.y++;
    }

    if (isPromptOutOfWindowHeight()) {
        scrollUp(backgroundColor);
        prompt.y--;
    }

    
    uint8_t * bitMapCharacter = bitMap(c);
    int baseXPixel = prompt.x * font.Width + prompt.baseX;
    int baseYPixel = prompt.y * font.Height + prompt.baseY;
    for (int i = 0; i < font.Height; i++) {
        for (int j = 0; j < font.Width; j++) {
            if (bitMapCharacter[i] & 1 << (font.Width - j - 1)) // pregunta si el bit numero i esta prendido
                setPixel(baseXPixel + j, baseYPixel + i, fontColor);
            else
                setPixel(baseXPixel + j, baseYPixel + i, backgroundColor);
        }
    }
    prompt.x++;
}

void newLine(Color * backgroundColor) {
    prompt.x = 0;
    prompt.y++;
    if (isPromptOutOfWindowHeight()) {
        scrollUp(backgroundColor);
        prompt.y--;
    }
}

void eraseChar(Color * backgroundColor) {
    if (prompt.x == 0 && prompt.y == 0)
        return;

    if (prompt.x == 0) {
        prompt.x = prompt.windowWidth / font.Width - 1;
        prompt.y--;
    } else 
        prompt.x--;
    
    int baseXPixel = prompt.baseX + getXPixel(); // Coordenadas absolutas
    int baseYPixel = prompt.baseY + getYPixel();
    for (int y = baseYPixel; y < baseYPixel + font.Height; y++)
        for (int x = baseXPixel; x < baseXPixel + font.Width; x++)
            setPixel(x, y, backgroundColor);
}