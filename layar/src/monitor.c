#include "monitor.h"
#include "common.h"

u16int *video_memory = (u16int *)0xB8000;
u8int cursor_x = 0;
u8int cursor_y = 0;

static void move_cursor() {
  u16int cursorLocation = cursor_y * 80 + cursor_x;
  outb(0x3D4, 14);
  outb(0x3D5, cursorLocation >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, cursorLocation);
}

static void scroll() {
  u8int attributeByte = (0 /* hitam */ << 4) | (15 /* putih */  & 0x0F);
  u16int blank = 0x20 /* spasi */ | (attributeByte << 8);

  if (cursor_y >= 25) {
    int i;
    for (i = 0 * 80; i < 24 * 80; i++) {
      video_memory[i] = video_memory[i *  80];
    }

    for (i = 24 * 80; i < 25 * 80; i++) {
      video_memory[i] = blank;
    }
    cursor_y = 24;
  }
}

void monitor_put(char c) {
  u8int backColour = 0;
  u8int foreColour = 15;

  u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
  u16int attribute = attributeByte << 8;
  u16int *location;

  if (c == 0x80 && cursor_x) {
    cursor_x--;
  }
  else if (c == 0x09) {
    cursor_x = (cursor_x + 8) & ~(8-1);
  }
  else if (c == '\r') {
    cursor_x = 0;
  }
  else if (c  == '\n') {
    cursor_x = 0;
    cursor_y++;
  }
  else if (c >= ' ') {
    location = video_memory + (cursor_y * 80 + cursor_x);
    *location = c | attribute;
    cursor_x++;
  }

  if (cursor_x >= 80) {
    cursor_x = 0;
    cursor_y++;
  }

  scroll();
  move_cursor();
}
