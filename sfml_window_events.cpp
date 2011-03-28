
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML Events");

    // Get a reference to the input manager associated to our window, and store it for later use
    const sf::Input& Input = App.GetInput();

    // Start main loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();

            // Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                App.Close();
        }

        // Get some useless input states, just to illustrate the tutorial
        bool         LeftKeyDown     = Input.IsKeyDown(sf::Key::Left);
        bool         RightButtonDown = Input.IsMouseButtonDown(sf::Mouse::Right);
        bool         JoyButton1Down  = Input.IsJoystickButtonDown(0, 1);
        unsigned int MouseX          = Input.GetMouseX();
        unsigned int MouseY          = Input.GetMouseY();
        int          JoystickX       = Input.GetJoystickAxis(1, sf::Joy::AxisZ);
        int          JoystickY       = Input.GetJoystickAxis(1, sf::Joy::AxisY);
        int          JoystickPOV     = Input.GetJoystickAxis(1, sf::Joy::AxisPOV);

        // Display window on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}
