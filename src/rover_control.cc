#include <cmath>
#include <iostream>

#include "game_controller.h"
#include "logger.h"

static const std::string kLogTag = "ROVER CONTROL";
static const double kJoystickSmoothingFactor = 0.9;
static bool running = true;

static void HandleControllerEvent(game_controller::Controller *const controller);
static void HandleAxisMotionEvent(game_controller::Controller *const controller, game_controller::Event &event);
static void HandleButtonDownEvent(game_controller::Controller *const controller, game_controller::Event &event);
static double Map(const double value, const double in_min, const double in_max, const double out_min, const double out_max);
static inline double MovingAverage(const double current, const double previous, const double smoothing_factor);

int main(void)
{
    if (!game_controller::Initialize())
    {
    }
    else
    {
        game_controller::Controller *controller = game_controller::GetController(); // TODO add option to select which controller to use if multiple are connected

        while ((nullptr != controller) && running)
        {
            HandleControllerEvent(controller);
        }

        if (nullptr != controller)
        {
            game_controller::CloseController(controller);
        }

        game_controller::Deinitialize();
    }

    return 0;
}

static void HandleControllerEvent(game_controller::Controller *const controller)
{
    game_controller::Event event;
    while (0 != game_controller::PollEvent(&event))
    {
        switch (static_cast<game_controller::EventCode>(event.type))
        {
        case game_controller::EventCode::CONTROLLER_ADDED:
            if (nullptr == controller)
            {
                controller = game_controller::GetController();

                LOGGER_LOG_DEBUG(std::cout, kLogTag, "Controller added");
            }
            break;
        case game_controller::EventCode::CONTROLLER_REMOVED:
            if (game_controller::IsControllerEvent(controller, event))
            {
                game_controller::CloseController(controller);

                LOGGER_LOG_DEBUG(std::cout, kLogTag, "Controller removed");
            }
            break;
        case game_controller::EventCode::BUTTON_DOWN:
            if (game_controller::IsControllerEvent(controller, event))
            {
                HandleButtonDownEvent(controller, event);
            }
            break;
        case game_controller::EventCode::AXIS_MOTION:
            if (game_controller::IsControllerEvent(controller, event))
            {
                HandleAxisMotionEvent(controller, event);
            }
            break;
        case game_controller::EventCode::BUTTON_UP:
        case game_controller::EventCode::CONTROLLER_REMAPPED:
        case game_controller::EventCode::TOUCHPAD_DOWN:
        case game_controller::EventCode::TOUCHPAD_MOTION:
        case game_controller::EventCode::TOUCHPAD_UP:
        case game_controller::EventCode::SENSOR_UPDATE:
        default:
            break;
        }
    }
}

static void HandleAxisMotionEvent(game_controller::Controller *const controller, game_controller::Event &event)
{
    switch (static_cast<game_controller::JoystickAxis>(event.jaxis.axis))
    {
    case game_controller::JoystickAxis::LEFT_X:
        LOGGER_LOG_VERBOSE(std::cout, kLogTag, "X %d", event.jaxis.value);
        break;
    case game_controller::JoystickAxis::LEFT_Y:
    case game_controller::JoystickAxis::RIGHT_X:
    case game_controller::JoystickAxis::RIGHT_Y:
    default:
        break;
    }
}

static void HandleButtonDownEvent(game_controller::Controller *const controller, game_controller::Event &event)
{
    switch (static_cast<game_controller::Button>(event.cbutton.button))
    {
    case game_controller::Button::BUTTON_X:
        running = false;
        break;
    case game_controller::Button::BUTTON_LEFTSHOULDER:
    case game_controller::Button::BUTTON_RIGHTSHOULDER:
    case game_controller::Button::BUTTON_INVALID:
    case game_controller::Button::BUTTON_A:
    case game_controller::Button::BUTTON_B:
    case game_controller::Button::BUTTON_Y:
    case game_controller::Button::BUTTON_BACK:
    case game_controller::Button::BUTTON_GUIDE:
    case game_controller::Button::BUTTON_START:
    case game_controller::Button::BUTTON_LEFTSTICK:
    case game_controller::Button::BUTTON_RIGHTSTICK:
    case game_controller::Button::BUTTON_DPAD_UP:
    case game_controller::Button::BUTTON_DPAD_DOWN:
    case game_controller::Button::BUTTON_DPAD_LEFT:
    case game_controller::Button::BUTTON_DPAD_RIGHT:
    default:
        break;
    }
}

static double Map(const double value, const double in_min, const double in_max, const double out_min, const double out_max)
{
    double capped_value = value;

    if (value < in_min)
    {
        capped_value = in_min;
    }
    if (value > in_max)
    {
        capped_value = in_max;
    }

    return (capped_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline double MovingAverage(const double current, const double previous, const double smoothing_factor)
{
    return previous + (smoothing_factor * (current - previous));
}