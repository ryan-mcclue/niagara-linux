// SPDX-License-Identifier: zlib-acknowledgement

// Intel HD 620?
// ignore error handling, synchronisation

#include <stdio.h>
#include <stdarg.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>


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
  VkInstance instance = 0;

  // vulkan API uses structs with type field
  // TODO: should verify that supports version 1.1
  VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  app_info.apiVersion = VK_VERSION_1_1;

  VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  create_info.pApplicationInfo = &app_info;

  vkCreateInstance(&create_info, 0, &instance);

  // most GPU specs will have extensions, e.g. RTX extensions
  
  // instance is vulkan runtime/loader
  //
  // vulkan validation layers dlls that can be added to vulkan instance at runtime
  // validation layers help us verify that the code we write is actually correct
  // I guess similar to clang sanitisers?
  // (DIFFERENT TO EXTENSIONS!)
  
  // to draw on screen: surface (tied to window) interfaces to swap chain (size of window)

  // Distinction between physical device and logical device, e.g. SLI between 2 GPUs to 1 logical device

  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == GLFW_TRUE)
  {
    GLFWwindow *window = glfwCreateWindow(640, 480, "window", NULL, NULL);
    if (window != NULL)
    {
      while (!glfwWindowShouldClose(window))
      {
        glfwPollEvents();
      }
    }

    glfwTerminate();
  }

  vkDestroyInstance(instance, 0);

  return 0;
}
