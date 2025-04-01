#include "game_controller.h"

#include <iostream>

#include "logger.h"

namespace game_controller
{

static const std::string kLogTag = "GAME CONTROLLER";

bool Initialize(void)
{
    bool initialized = false;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        LOGGER_LOG_ERROR(std::cout, kLogTag, "SDL error code {}", SDL_GetError());
        LOGGER_LOG_ERROR(std::cout, kLogTag, "Failed to initialize");
    }
    else
    {
        initialized = true;
    }

    return initialized;
}

std::vector<int> GetControllerNumbers(void)
{
    std::vector<int> controller_numbers;
    int              controller_count = SDL_NumJoysticks();

    LOGGER_UNUSED(controller_numbers);

    if (controller_count < 0)
    {
        LOGGER_LOG_ERROR(std::cout, kLogTag, "SDL error code {}", SDL_GetError());
        LOGGER_LOG_ERROR(std::cout, kLogTag, "Failed to get controller numbers");
    }
    else
    {
        controller_numbers.reserve(static_cast<size_t>(controller_count));

        for (int i = 0; i < controller_count; i++)
        {
            if (SDL_IsGameController(i))
            {
                controller_numbers.emplace_back(i);
            }
        }
    }

    return controller_numbers;
}

Controller *OpenController(const int controller_number)
{
    Controller *controller = nullptr;

    if (controller_number < SDL_NumJoysticks())
    {
        controller = SDL_GameControllerOpen(controller_number);

        if (nullptr == controller)
        {
            LOGGER_LOG_ERROR(std::cout, kLogTag, "SDL error code {}", SDL_GetError());
            LOGGER_LOG_ERROR(std::cout, kLogTag, "Failed to open controller");
        }
    }

    return controller;
}

Controller *GetController(void)
{
    Controller *     controller         = nullptr;
    std::vector<int> controller_numbers = game_controller::GetControllerNumbers();

    if (!controller_numbers.empty())
    {
        controller = game_controller::OpenController(controller_numbers[0]);
    }

    return controller;
}

} // namespace controller