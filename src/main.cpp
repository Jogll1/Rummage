#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

constexpr auto WINDOW_WIDTH  = 800u;
constexpr auto WINDOW_HEIGHT = 600u;

constexpr auto WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Rummage", WINDOW_STYLE);
    window.setVerticalSyncEnabled(true); // VSync
    //window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

        // Draw here

        window.display();
    }
}