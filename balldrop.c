/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include <stdlib.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int vga_ball_fd;

/* Read and print the properties color */
void print_properties_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_PROPERTIES, &vla)) {
      perror("ioctl(VGA_BALL_READ_PROPERTIES) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.properties.x, vla.properties.y, vla.properties.rg_color);
}

/* Set the properties color */
void set_properties_color(const vga_ball_props_t *c)
{
  vga_ball_arg_t vla;
  vla.properties = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_PROPERTIES, &vla)) {
      perror("ioctl(VGA_BALL_SET_PROPERTIES) failed");
      return;
  }
}

int main(int argc, char *argv[])
{
  if(argc != 9){
    printf("Usage: %s <pos_x> <pos_y> <vx> <vy> <gravityx> <gravityy>  <dampening> <random_flag>\n", argv[0]);
    printf("  random_flag: 1 for random colors, 0 for no random colors\n");
    return -1;
  }
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";
  double pos_x, pos_y, vx, vy, gx,gy, damp;
  int random_flag;

  vga_ball_props_t init = { 640, 0, 0x000 };

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  pos_x = strtod(argv[1], NULL);
  pos_y = strtod(argv[2], NULL);
  vx = strtod(argv[3], NULL);
  vy = strtod(argv[4], NULL);
  gx = strtod(argv[5], NULL);
  gy = strtod(argv[6], NULL);
  damp = strtod(argv[7], NULL);
  random_flag = atoi(argv[8]);

srand(time(NULL));

printf("%lf, %lf, %lf, %lf, %lf, %lf, %lf\n", pos_x, pos_y, vx, vy, gx,gy, damp);
while(1){
    unsigned short random_number = random_flag ? (rand() % 65536) : 0;
    
    set_properties_color(&init);
    vy += gy;
    vx += gx;

    pos_x = pos_x + vx;
    pos_y = pos_y + vy;

    if (pos_y > 460) {
      pos_y = 460;
      vy = damp*-vy;
      init.rg_color = random_number;
    }

    if (pos_y < 20) {
      pos_y = 20;
      vy = damp*-vy;
      init.rg_color = random_number;
    }
    if (pos_x > 1240) {
      pos_x = 1240;
      vx = damp*-vx;
      init.rg_color = random_number;
    }
    if (pos_x < 40) {
      pos_x = 40;
      vx = damp*-vx;
      init.rg_color = random_number;
    }

    init.x = (short)(pos_x);
    init.y = (short)(pos_y);
    //set_properties_color(&colors[i % COLORS ]);
    //print_properties_color();
    usleep(4000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
