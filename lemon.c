#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

struct {
	GLFWwindow * window;
	VkInstance instance;
	VkPhysicalDevice card;
} gfx;

#define ERRNDIE(X) { fputs(X, stderr); exit(1); }

void
loop()
{
	while (!glfwWindowShouldClose(gfx.window)) {
		glfwPollEvents();
	}
}



void
initVulkan()
{
	/*
	 * Create an instance.
	 * */
	unsigned glfw_extension_count;
	char const ** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	VkApplicationInfo applicationInfo = (VkApplicationInfo) {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Vulkan",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0
	};
	VkInstanceCreateInfo instanceInfo = (VkInstanceCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &applicationInfo,
		.enabledExtensionCount = glfw_extension_count,
		.ppEnabledExtensionNames = glfw_extensions,
#ifdef DEBUG
		.enabledLayerCount = 1,
		.ppEnabledLayerNames = (char const *[]) { "VK_LAYER_KHRONOS_validation" }
#else
		.enabledLayerCount = 0
#endif
	};
	if (vkCreateInstance(&instanceInfo, NULL, &gfx.instance) != VK_SUCCESS)
		ERRNDIE("Failed to create a vulkan instance\n");


	/*
	 * Choose a physical device.
	 * */
	unsigned card_count;
	vkEnumeratePhysicalDevices(gfx.instance, &card_count, NULL);
	VkPhysicalDevice * cards = malloc(card_count * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(gfx.instance, &card_count, cards);

	unsigned top_score = 0;
	while (card_count--) {
		/* Rate devices. */
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(cards[card_count], &properties);
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(cards[card_count], &features);
		unsigned score =
			2 * properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		if (score > top_score) {
			top_score = score;
			gfx.card = cards[card_count];
		}
	}

	free(cards);
	if (!gfx.card)
		ERRNDIE("No suitable graphics cards.\n");

	/*
	 * Create a logical device.
	 * */

	
}

void
initWindow()
{
	if (!glfwInit())
		ERRNDIE("Failed to initialize GLFW.\n");

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	gfx.window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);
}

void
deinit()
{
	vkDestroyInstance(gfx.instance, NULL);
	glfwDestroyWindow(gfx.window);
	glfwTerminate();
}

int
main()
{
	initWindow();
	initVulkan();
	loop();
	deinit();
}
