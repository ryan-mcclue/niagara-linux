// SPDX-License-Identifier: zlib-acknowledgement

// Intel HD 620?
// ignore error handling, synchronisation

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11 1
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

#define INTERNAL static

typedef uint32_t u32;

#define ARRAY_COUNT(arr) \
  (sizeof(arr) / sizeof(arr[0]))

#define VKX(call) \
  do { \
    VkResult result = call; \
    (result != VK_SUCCESS) ? FPRINTF(stderr, "(Vulkan) Error: API %d\n", result), exit(1) : (void)0; \
  } while (0);

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
  app_info.apiVersion = VK_API_VERSION_1_1;

  VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  create_info.pApplicationInfo = &app_info;

#if defined(DEBUG)
  const char *debug_layers[] = {
    "VK_LAYER_KHRONOS_validation",
  };

  create_info.ppEnabledLayerNames = debug_layers;
  create_info.enabledLayerCount = sizeof(debug_layers) / sizeof(debug_layers[0]);
#endif

  // KHR (khronos extensions) are as good as part of Vulkan; ratified in 1.1
  const char *extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME, 
#endif
  };

  create_info.ppEnabledExtensionNames = extensions;
  create_info.enabledExtensionCount = sizeof(extensions) / sizeof(extensions[0]);

  VKX(vkCreateInstance(&create_info, NULL, &instance));
  // From this point on, talking to graphics drivers and validation layers

  VkPhysicalDevice physical_devices[16] = {};
  u32 physical_device_count = ARRAY_COUNT(physical_devices);
  VKX(vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices));

  VkPhysicalDevice physical_device = 0;
  if (physical_device_count > 0)
  {
    physical_device = physical_devices[0];
    for (u32 i = 0; i < physical_device_count; ++i)
    {
      VkPhysicalDeviceProperties props = {};
      vkGetPhysicalDeviceProperties(physical_devices[i], &props);
      if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
        physical_device = physical_devices[i];
        break;
      }
    }
  }
  else
  {
    FPRINTF(stderr, "(Vulkan) Error: No physical devices found\n");
  }
  
  // vkGetPhysicalDeviceQueueFamilyProperties(physical_device, );

  // devices can have multiple queues. each queue can be used to submit particular commands 
  // e.g. a queue for compute commands, a queue for presentation etc.
  float queue_properties = 1.0f;

  VkDeviceQueueCreateInfo queue_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
  queue_info.queueFamilyIndex = 0; // TODO: this needs to be computed from queue properties
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = &queue_properties;

  VkDeviceCreateInfo device_create_info = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  device_create_info.queueCreateInfoCount = 1;
  device_create_info.pQueueCreateInfos = &queue_info;

  const char *device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };
  device_create_info.ppEnabledExtensionNames = device_extensions;
  device_create_info.enabledExtensionCount = ARRAY_COUNT(device_extensions);

  VkDevice logical_device = 0;
  VKX(vkCreateDevice(physical_device, &device_create_info, NULL, &logical_device));

  // the exception to the mostly cross-platform vulkan is surface creation extension
  
  // most GPU specs will have extensions, e.g. RTX extensions
  
  // instance is vulkan runtime/loader
  //
  // vulkan validation layers dlls that can be added to vulkan instance at runtime
  // validation layers help us verify that the code we write is actually correct
  // I guess similar to clang sanitisers?
  // (DIFFERENT TO EXTENSIONS!)
  
  // to draw on screen: surface (tied to window) manages swap chain (size of window)

  // Distinction between physical device and logical device, e.g. SLI between 2 GPUs to 1 logical device

  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == GLFW_TRUE)
  {
    GLFWwindow *window = glfwCreateWindow(640, 480, "window", NULL, NULL);
    if (window != NULL)
    {
      VkSurfaceKHR surface = 0;
#if defined(VK_USE_PLATFORM_XLIB_KHR)
      VkXlibSurfaceCreateInfoKHR surface_create_info = { VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR };
      surface_create_info.dpy = glfwGetX11Display();
      surface_create_info.window = glfwGetX11Window(window);

      VKX(vkCreateXlibSurfaceKHR(instance, &surface_create_info, NULL, &surface));
#else
      // Here we would support say, MoltenVK, nintendo switch, etc.
#error "Unsupported Platform"
#endif
      int window_width = 0, window_height = 0;
      glfwGetWindowSize(window, &window_width, &window_height);

      // Will have to recreate swap chain when window size changes
      VkSwapchainCreateInfoKHR swap_chain_create_info = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
      swap_chain_create_info.surface = surface;
      swap_chain_create_info.minImageCount = 2; // double-buffering
      swap_chain_create_info.imageFormat = VK_FORMAT_R8G8B8A8_UNORM; // TODO: some devices only support BGRA so detect what is supported dynamically
      swap_chain_create_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; 
      swap_chain_create_info.imageExtent.width = window_width;
      swap_chain_create_info.imageExtent.height = window_height;
      swap_chain_create_info.imageExtent.height = window_height;
      swap_chain_create_info.imageArrayLayers = 1; // we could make image a texture array ...
      swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // we are going to render to swap chain image. we could copy things to swap chain image
      swap_chain_create_info.queueFamilyIndexCount = ;



      VkSwapchainKHR swapchain = 0;
      VKX(vkCreateSwapchainKHR(logical_device, &swap_chain_create_info, NULL, &swapchain));

      while (!glfwWindowShouldClose(window))
      {
        glfwPollEvents();
      }
    }

    glfwTerminate();
  }

  vkDestroyInstance(instance, NULL);

  return 0;
}
