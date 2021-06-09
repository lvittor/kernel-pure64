#include <video.h>

prompt_info * prompt = {0, 0}; 

vbe_mode_info * vbeInfo = 0x5C00;

void pintarNashe(){
    char * curpos = vbeInfo->framebuffer;
    for (int i = 0; i < 6000; i++){
        *curpos = 255;
        curpos++;
        *curpos = 255;
        curpos++;
        *curpos = 255;
        curpos++;
    }
}

// void drawPixel(int x, int y, int r, int g, int b){
//     unsigned char * screen = (unsigned char *) ((uint64_t) vbeInfo->framebuffer + x * vbeInfo->bpp / 8 + (int) y * vbeInfo->pitch);
//     screen[0] = b;
//     screen[1] = g;
//     screen[2] = r;
// }

// void pintarNashe(void){
//     for (int i = 0; i < 100; i++){
//         for (int j = 0; j < 100; j++){
//             //Color color = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
//             drawPixel(i, j, 255, 255, 255);
//         }
//     }
// }

// void init_video(void){
//     vbeInfo = (vbe_mode_info *) VBEModeInfoBlockAddress;
//    // prompt.x = 0;
//     //prompt.y = vbeInfo-> height - font.
// }


