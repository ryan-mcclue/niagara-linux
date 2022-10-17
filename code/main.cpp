// SPDX-License-Identifier: zlib-acknowledgement

// Intel HD 620?
// ignore error handling, synchronisation

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdarg.h>

#define INTERNAL static

INTERNAL void
FPRINTF(FILE *stream, const char *format, ...)
{
  va_list args;
  va_start(args, format);

  vfprintf(stderr, format, args);

  va_end(args);
}

void 
glfw_error_callback(int error, const char *description)
{
  FPRINTF(stderr, "(GLFW) Error: %s\n", description);
}

int
main(int argc, char *argv[])
{
  // TODO(Ryan): dpkg -L vulkan-sdk not giving header files?
  printf("hello, niagara!\n");

  // instance is vulkan runtime/loader
  //
  // vulkan validation layers dlls that can be added to vulkan instance at runtime
  
  // to draw on screen: surface (tied to window) interfaces to swap chain (size of window)

  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == GLFW_TRUE)
  {
    // Distinction between physical device and logical device, e.g. SLI between 2 GPUs to 1 logical device
    GLFWwindow *window = glfwCreateWindow(640, 480, "window", NULL, NULL);
    if (window != NULL)
    {
      while (!glfwWindowShouldClose(window))
      {

      }
    }

    glfwTerminate();
  }

  return 0;
}
