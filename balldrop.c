/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.background = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";
  double pos_x, pos_y, vx, vy;

  static const vga_ball_color_t colors[] = {
    //1280 x 480
    { 0x000, 0x000, 0x000 }, /* Red */
    { 1280, 480, 0 }, /* Red */
    { 1280, 0, 0 }, /* Red */
    { 0, 480, 0 }, /* Red */
    { 0x000, 0x000, 0x100 }, /* Red */
    { 1280, 0, 480 }, /* Red */
    { 1280, 480, 567 }, /* Red */
    { 0, 480, 480 }, /* Red */
    { 400, 200, 200 }, /* Red */
  };
  vga_ball_color_t init = { 640, 0, 0x000 };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  printf("initial state: ");
  print_background_color();
  pos_x = 640;
  pos_y = 0;
  vx = 1;
  vy = 0;

    
while(1){
    set_background_color(&init);
    vy += 0.2;

    pos_x = pos_x + vx;
    pos_y = pos_y + vy;

    if (pos_y > 480) {
      pos_y = 480;
      vy = 0.8*-vy;
    }

    if (pos_y < 0) {
      pos_y = 0;
      vy = 0.8*-vy;
    }
    if (pos_x > 1280) {
      pos_x = 1280;
      vx = 0.8*-vx;
    }
    if (pos_x < 0) {
      pos_x = 0;
      vx = 0.8*-vx;
    }
    init.red = (short)(pos_x);
    init.green = (short)(pos_y);
    //set_background_color(&colors[i % COLORS ]);
    //print_background_color();
    usleep(4000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
