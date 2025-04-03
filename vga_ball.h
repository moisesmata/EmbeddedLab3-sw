#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>

typedef struct {
  unsigned short x, y, rg_color;
} vga_ball_props_t;
  

typedef struct {
  vga_ball_props_t properties;
} vga_ball_arg_t;

#define VGA_BALL_MAGIC 'q'

/* ioctls and their arguments */
#define VGA_BALL_WRITE_PROPERTIES _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t)
#define VGA_BALL_READ_PROPERTIES  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t)

#endif
