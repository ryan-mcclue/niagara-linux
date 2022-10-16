// SPDX-License-Identifier: zlib-acknowledgement

// Intel HD 620?
// ignore error handling, synchronisation

// use lunarG sdk

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <assert.h>

int
main(int argc, char *argv[])
{
  printf("hello, niagara!\n");

  if (glfwInit() == GLFW_TRUE)
  {

  }
  else
  {
    assert(0);
  }

  return 0;
}
