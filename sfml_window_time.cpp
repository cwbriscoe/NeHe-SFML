
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
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML Time");

    // Disable vertical synchronization to get maximum framerate
    App.UseVerticalSync(false);

    // Create a clock for measuring time
    sf::Clock Clock;

    // Some dummy variables to simulate an object that moves
    const float Speed = 50.f;
    float Left = 0.f;
    float Top  = 0.f;

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

        // Get elapsed time since last frame (we could as well use App.GetFrameTime())
        float ElapsedTime = Clock.GetElapsedTime();
        Clock.Reset();

        // Make our virtual object move
        if (App.GetInput().IsKeyDown(sf::Key::Left))  Left -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Right)) Left += Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Up))    Top  -= Speed * ElapsedTime;
        if (App.GetInput().IsKeyDown(sf::Key::Down))  Top  += Speed * ElapsedTime;

        // Display window on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}
