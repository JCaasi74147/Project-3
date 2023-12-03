#include <SFML/Graphics.hpp>
#include <iostream>
#include "graph.cpp"
#include "dijkstra.cpp"
#include "bfs.cpp"
#include <unordered_map>
using namespace std;

int main() {
    //import movie and actor data
    Graph movieGraph;
    movieGraph.buildGraphFromCSV("actor-movie-data.csv");
    unordered_map<int, string> actorNames = movieGraph.getActorNames();
    unordered_map<int, string> movieNames = movieGraph.getMovieNames();
    vector<int> path;

    int windowNum = 1;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "BACON", sf::Style::Close);
    sf::Font font;
    sf::Text welcomeText;
    sf::Text text2;
    font.loadFromFile("/Users/aily0/Downloads/Minesweeper_v4/Minesweeper_v4/files/font.ttf");
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
    bool b1select = false;
    bool b2select = false;
    bool dj = false;
    bool bf = false;
    string name1;
    string name2;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            int x,y;
            sf::Vector2i position = sf::Mouse::getPosition(window);
            x = position.x;
            y = position.y;
            cout << x << "    " << y << endl;
            if (windowNum == 1){
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    if(x> 330 and x < 650){
                        if (y> 780 and y < 950){
                            windowNum += 1;
                        }
                    }
                }
                sf::Texture tKB;
                tKB.loadFromFile("/Users/aily0/CLionProjects/proj3/images/KEVINBACON.png");
                sf::Sprite sKB;
                sKB.setTexture(tKB);
                sKB.setPosition(250, 250);

                sf::Texture tStart;
                tStart.loadFromFile("/Users/aily0/CLionProjects/proj3/images/Start.png");
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
            if (windowNum == 2){
                sf::Text input1, input2, actor1, actor2, dselect, bfselect, prompt;
                //text formatting for input prompts
                input1.setFont(font);
                input1.setString("Actor 1:");
                input1.setCharacterSize(25);
                input1.setFillColor(sf::Color::Black);
                input1.setStyle(sf::Text::Bold);
                input1.setPosition(185, 170);
                input2.setFont(font);
                input2.setString("Actor 2:");
                input2.setCharacterSize(25);
                input2.setFillColor(sf::Color::Black);
                input2.setStyle(sf::Text::Bold);
                input2.setPosition(185, 350);
                //other text formatting
                prompt.setFont(font);
                prompt.setString("Please select an algorithm:");
                prompt.setCharacterSize(25);
                prompt.setFillColor(sf::Color::Black);
                prompt.setStyle(sf::Text::Bold);
                prompt.setPosition(185, 500);

                dselect.setFont(font);
                dselect.setString("Dijkstra");
                dselect.setCharacterSize(25);
                dselect.setFillColor(sf::Color::Black);
                dselect.setStyle(sf::Text::Bold);
                dselect.setPosition(250, 547);

                bfselect.setFont(font);
                bfselect.setString("Breadth first");
                bfselect.setCharacterSize(25);
                bfselect.setFillColor(sf::Color::Black);
                bfselect.setStyle(sf::Text::Bold);
                bfselect.setPosition(250, 617);
                //importing of images for input boxes
                sf::Texture box, square, sbox, checked, button;
                box.loadFromFile("/Users/aily0/CLionProjects/proj3/images/textbox.jpg");
                square.loadFromFile("/Users/aily0/CLionProjects/proj3/images/cbox.png");
                sbox.loadFromFile("/Users/aily0/CLionProjects/proj3/images/selectedbox.png");
                checked.loadFromFile("/Users/aily0/CLionProjects/proj3/images/checkedbox.png");
                button.loadFromFile("/Users/aily0/CLionProjects/proj3/images/end.png");
                sf::Sprite tBox1, tBox2, cbox1, cbox2, nextb;
                tBox1.setTexture(box);
                tBox2.setTexture(box);
                cbox1.setTexture(square);
                cbox2.setTexture(square);
                nextb.setTexture(button);
                //intake whether mouse is pressed and where
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    //conditions for box 1 to be selected
                    if(x> 185 and x < 800){
                        b1select = false;
                        if (y> 200 and y < 300){
                            b1select = true;
                        }
                    }
                    else{
                        b1select = false;
                    }
                    //conditions for box 1 to be selected
                    if(x> 185 and x < 800){
                        b2select = false;
                        if (y> 380 and y < 480){
                            b2select = true;
                        }
                    }
                    else{
                        b2select = false;
                    }

                    if(x> 185 and x < 245){
                        dj = false;
                        if (y> 530 and y < 590){
                            dj = true;
                            bf = false;
                        }
                    }
                    if (y> 600 and y < 660){
                         bf = false;
                         if (x> 185 and x < 245){
                             dj = false;
                             bf = true;
                         }
                    }
                    if (x > 359 and x < 641){
                        if (y > 758 and y < 900){
                            if (dj or bf == true){
                                if (name1 != "" and name2 != "")
                                    windowNum += 1; 
                            }
                        }
                    }
                }

                if (b1select) {
                    tBox1.setTexture(sbox);
                    tBox1.setPosition(185, 200);
                    if (event.type == sf::Event::TextEntered) {
                        if (isalpha(event.text.unicode))
                            name1 += tolower(event.text.unicode);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                            name1 += " ";
                    }
                    if ((event.type == sf::Event::KeyPressed) and sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                        name1.pop_back();
                    }
                }
                else{
                    tBox1.setTexture(box);
                    tBox1.setPosition(185, 200);
                }
                actor1.setFont(font);
                actor1.setString(name1);
                actor1.setCharacterSize(35);
                actor1.setFillColor(sf::Color::Black);
                actor1.setStyle(sf::Text::Bold);
                actor1.setPosition(200, 225);

                if (b2select) {
                    tBox2.setTexture(sbox);
                    tBox2.setTexture(sbox);
                    tBox2.setPosition(185, 380);
                    if (event.type == sf::Event::TextEntered) {
                        if (isalpha(event.text.unicode))
                            name2 += tolower(event.text.unicode);
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                            name2 += " ";
                    }
                    if ((event.type == sf::Event::KeyPressed) and sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                        name2.pop_back();
                    }
                }
                else{
                    tBox2.setTexture(box);
                    tBox2.setTexture(box);
                    tBox2.setPosition(185, 380);
                }
                actor2.setFont(font);
                actor2.setString(name2);
                actor2.setCharacterSize(35);
                actor2.setFillColor(sf::Color::Black);
                actor2.setStyle(sf::Text::Bold);
                actor2.setPosition(200, 405);

                if (dj) {
                    cbox1.setTexture(checked);
                    cbox1.setPosition(185, 530);
                }
                else {
                    cbox1.setTexture(square);
                    cbox1.setPosition(185, 530);
                }
                if (bf) {
                    cbox2.setTexture(checked);
                    cbox2.setPosition(185, 600);
                }
                else {
                    cbox2.setTexture(square);
                    cbox2.setPosition(185, 600);
                }
                nextb.setPosition(359, 758);

                window.clear(sf::Color::Magenta);
                window.draw(tBox1);
                window.draw(tBox2);
                window.draw(prompt);
                window.draw(dselect);
                window.draw(bfselect);
                window.draw(input1);
                window.draw(input2);
                window.draw(cbox1);
                window.draw(cbox2);
                window.draw(actor1);
                window.draw(actor2);
                window.draw(nextb);
                window.display();
            }
        }
        if (windowNum == 3){
            //loading screen
            //plug actor names into code
            auto& actorNames = movieGraph.getActorNames();
            int startActorId = -1, targetActorId = -1;
            for (const auto& pair : actorNames) {
                if (pair.second == name1) {
                    startActorId = pair.first;
                }
                if (pair.second == name2) {
                    targetActorId = pair.first;
                }
            }
            if (startActorId == -1 || targetActorId == -1) {
                windowNum += 1;
            }

            if (dj) {
                path = dijkstra(movieGraph, startActorId, targetActorId);
            } else {
                path = bfs(movieGraph, startActorId, targetActorId);
            }
            window.clear(sf::Color::Black);
            window.display();
        }
        if (windowNum == 4){
            //error screen
            //ex. actor was not found in the database or name was incorectly entered
            //add button to retry ->windowNum = 2
        }
        if(windowNum == 5){
            // display output
            //add button to reset ->windowNum = 2
        }

    }
    return 0;
}