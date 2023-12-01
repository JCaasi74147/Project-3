#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "BACON", sf::Style::Close);
    sf::Font font;
    sf::Text welcomeText;
    sf::Text text2;
    font.loadFromFile("files/font.ttf");
    welcomeText.setFont(font);
    welcomeText.setString("Bacon Bridge: A Movie Network Adventure");
    welcomeText.setCharacterSize(35);
    welcomeText.setFillColor(sf::Color::Black);
    welcomeText.setStyle(sf::Text::Bold | sf::Text:: Underlined);
    welcomeText.setPosition(75, 125);

    text2.setFont(font);
    text2.setString("By: The BaconPath Pioneers");
    text2.setCharacterSize(25);
    text2.setFillColor(sf::Color::Black);
    text2.setStyle(sf::Text::Bold);
    text2.setPosition(275, 200);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            int x,y;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                sf::Vector2i position = sf::Mouse::getPosition(window);
                x = position.x;
                y = position.y;
                cout << x << "    " << y << endl;
            }
        }


        sf::Texture tKB;
        tKB.loadFromFile("files/images/KEVINBACON.png");
        sf::Sprite sKB;
        sKB.setTexture(tKB);
        sKB.setPosition(250, 250);

        sf::Texture tStart;
        tStart.loadFromFile("files/images/Start.png");
        sf::Sprite sStart;
        sStart.setTexture(tStart);
        sStart.setPosition(325, 700);


        window.clear(sf::Color(128,0,0,255));
        window.draw(sKB);
        window.draw(sStart);
        window.draw(welcomeText);
        window.draw(text2);

        // Here you can draw other SFML objects
        window.display();
    }
    return 0;
}
