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

/* Read and print the properties color */
void print_properties() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_PROPERTIES, &vla)) {
      perror("ioctl(VGA_BALL_READ_PROPERTIES) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.properties.red, vla.properties.green, vla.properties.blue);
}

/* Set the properties color */
void set_properties(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.properties = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_PROPERTIES, &vla)) {
      perror("ioctl(VGA_BALL_SET_PROPERTIES) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";

  static const vga_ball_color_t colors[] = {
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

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  printf("initial state: ");
  print_properties();

  for (i = 0 ; i < 24 ; i++) {
    set_properties(&colors[i % COLORS ]);
    print_properties();
    usleep(400000);
  }
  
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}
