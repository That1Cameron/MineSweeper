#include <SFML/Graphics.hpp>
#include <iostream>

int main(){
    
    // create the window
    sf::RenderWindow window(sf::VideoMode(384, 384), "Mine Sweeper");
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    

    //game variables 
    const int Collumns = 12; //const for static array rn  (want to make this variable at some point)
    const int Rows = 12;     //const for static arrya rn  (want to make this variable at some point)
    int bombCount = 10;
    int grid[Rows][Collumns]; //tracks bombs
    int displayGrid[Rows][Collumns]; //displays tile

    sf::Texture textureSet;
    if (!textureSet.loadFromFile("tiles.jpg")){
        std::cout << "No Textures Found" << std::endl;
    }
    
    //under the hood sprites are pointers to pixel array
    int spriteSize = 32;//32x32 from the texture set i found
    sf::Sprite sprites(textureSet); 
    //load it all as 1 sprite and just use snippets later bc im lazy and 
    //i think it would be more effitient for small project

    
    for (int i = 0; i < Rows; i++){
        for (int j = 0; j < Collumns; j++) {
            displayGrid[i][j] = 10;
            //place mines...

        }   
    }

    while (window.isOpen())
    {
        //getPosition returns an int vector :/
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        int x = pos.x / spriteSize;
        int y = pos.y / spriteSize;

        // check window events 
        sf::Event event;
        while (window.pollEvent(event))
        {
            //close event
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //left click (going to chekc tile)
            if (event.type == sf::Event::MouseButtonReleased) {
                //reveal

            }

            //right click (going to flag tile)
            if (event.type == sf::Event::MouseButtonReleased) {
                displayGrid[x][y] = 11;
                std::cout << "Flag: " << x << y << std::endl;
            }
        }

        // clear the window
        window.clear(sf::Color::White);


        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Collumns; j++) {
                sprites.setTextureRect(sf::IntRect(displayGrid[i][j] * spriteSize, 0, spriteSize, spriteSize));
                sprites.setPosition(i * spriteSize, j * spriteSize);
                window.draw(sprites);
            }
        }   
        window.display();
    }

    return 0;
}