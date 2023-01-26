#include <SFML/Graphics.hpp>
#include <iostream>

void genBombs(std::vector<int>& grid);

int main(){
    
    // create the window
    sf::RenderWindow window(sf::VideoMode(384, 384), "Mine Sweeper");
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    

    //game variables 
    int Collumns = 12;  //const for static array rn  (want to make this variable at some point)
    int Rows = 12;      //const for static arrya rn  (want to make this variable at some point)
    const int bombCount = 10;
    std::vector<int> row(Collumns, 0);
    std::vector<std::vector<int>> grid(Rows, row);
    std::vector<std::vector<int>> displayGrid(Rows, row);
    bool bombsGen = false;

    sf::Texture textureSet;
    if (!textureSet.loadFromFile("tiles.jpg")){
        std::cout << "No Textures Found" << std::endl;
    }
    
    //under the hood sprites are pointers to pixel array
    int spriteSize = 32;//32x32 from the texture set i found
    sf::Sprite sprites(textureSet); 
    //load it all as 1 sprite and just use snippets later bc im lazy and 
    //i think it would be more effitient for small project

        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Collumns; j++) {
                displayGrid[i][j] = 10;
                grid[i][j] = 0;
            }
        }

        /*
                //if this won the bomb lottery and there is no bomb place one
                if ((1 == rand() % bombCount) && grid[i][j] != 9) {
                    grid[i][j] == 9;
                    currentBombs++;
                    std::cout << currentBombs << std::endl;
                    //make bounds for updater
                    int xbound = 0;
                    (i == 0) ? xbound = 0 : xbound--;
                    int ybound = 0;
                    (j == 0) ? ybound = 0 : ybound--;
                    //update nearby tiles
                    for (int k = xbound; k < xbound + 2; k++) {
                        for (int l = ybound; l < ybound + 2; l++) {
                            if (!(grid[k][l] == 9)) {
                                grid[k][l]++;
                            }
                        }
                    }
                }
                
        */

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

            //button events
            if (event.type == sf::Event::MouseButtonReleased) {

                //left click (going to check tile)
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    //if (!bombsGen) genBombs(grid[]);
                    //reveal
                    displayGrid[x][y] = grid[x][y];
                }

                //right click (going to flag tile)
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    //Flag is the 11th sprite on the spritesheet
                    displayGrid[x][y] = 11;
                }
            }
        }



        // clear the buffer
        window.clear(sf::Color::White);

        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Collumns; j++) {
                sprites.setTextureRect(sf::IntRect(displayGrid[i][j] * spriteSize, 0, spriteSize, spriteSize));
                sprites.setPosition(i * spriteSize, j * spriteSize);
                window.draw(sprites);
            }
        }
        //swap buffers
        window.display();
    }

    return 0;
}


void genBombs(std::vector<int>& grid) {

}