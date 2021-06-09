#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stdint.h>

#define WIDHT   1920
#define HEIGHT  1080

#define VBEModeInfoBlockAddress 0x0000000000005C00	

// https://wiki.osdev.org/VESA_Video_Modes
typedef struct vbe_mode_info {
    uint16_t attributes;         
    uint8_t window_a, window_b;            
    uint16_t granularity;        
    uint16_t window_size;
    uint16_t segment_a, segment_b;
    uint32_t win_func_ptr;       
    uint16_t pitch;

    uint16_t width, height;             
    uint8_t w_char, y_char;

    uint8_t planes;
    uint8_t bpp;                 
    uint8_t banks; 

    uint8_t memory_model;
    uint8_t bank_size;           
    uint8_t image_pages;
    uint8_t reserved0;
 
    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t reserved_mask, reserved_position;
    uint8_t direct_color_attributes;
 
    uint32_t framebuffer;         // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed)) vbe_mode_info;


typedef struct prompt_info{
    unsigned char x, y;
} prompt_info;

typedef struct Color{
    uint8_t r, g, b;
} Color;

// ESCRIBIR EN PANTALLA
void pintarNashe(void);

// FUNCIONES AUXILIARES
//void init_video(void);

#endif