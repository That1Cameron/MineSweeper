#include <SFML/Graphics.hpp>
#include <iostream>

struct bombWieght {
    float weight;
    int* tile;
};

void genBombs(int row, int col, int bombCount, std::vector<std::vector<int>>& grid, int x, int y);
void mergeSort(std::vector <bombWieght>& wieghts, int l, int r);
void merge(std::vector <bombWieght>& wieghts, int p, int q, int r);
void reveal(std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& displayGrid, int sourceX, int sourceY);


int main(){
    
    // create the window
    sf::RenderWindow window(sf::VideoMode(384, 384), "Mine Sweeper");
    sf::Image icon;
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    

    //game variables 
    srand((int)time(0));
    int collumns = 12;
    int rows = 12;
    const int bombCount = 14;                             // number of bombs
    std::vector<int> row(collumns, 0);                    // initialize row grid
    std::vector<std::vector<int>> grid(rows, row);        // initialize data grid
    std::vector<std::vector<int>> displayGrid(rows, row); // initialize displaed grid
    bool bombsGen = false;                                // flag for first click to generate bombs after

    sf::Texture textureSet;
    if (!textureSet.loadFromFile("tiles.jpg")){
        std::cout << "No Textures Found" << std::endl;
    }
    
    //under the hood sprites are pointers to pixel array
    int spriteSize = 32;//32x32 from the texture set i found
    sf::Sprite sprites(textureSet); 
    //load it all as 1 sprite and just use snippets later bc im lazy

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < collumns; j++) {
                displayGrid[i][j] = 10;
                grid[i][j] = 0;
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

            //button events
            if (event.type == sf::Event::MouseButtonReleased) {

                //left click (going to check tile)
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    //generate bombs on the first click
                    if (!bombsGen) genBombs(rows, collumns, bombCount, grid, x, y); bombsGen = true;

                    reveal(grid, displayGrid, x, y);

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

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < collumns; j++) {
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


void genBombs(int row, int col, int bombCount, std::vector<std::vector<int>>& grid, int x, int y) {
    std::vector <bombWieght> weightVector;
    
    // generate wieghts for each tile randomly for random bomb spread
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            bombWieght tempObj;
            tempObj.tile = &grid[i][j];
            tempObj.weight = (float)(rand()) / (float)(RAND_MAX);
            weightVector.push_back(tempObj);
        }
    }

    weightVector[x + (y * row)].weight = 0;
    
    /* TEST FOR MERGE SORT REMOVE L8R
    std::cout << std::setprecision(1);
    for (int i = 0; i < wieghtVectr.size() - 1; i++)
        std::cout << wieghtVectr[i].weight << " ";
    std::cout << std::endl << std::endl;*/

    mergeSort(weightVector, 0, weightVector.size() - 1);

    /* TEST FOR MERGE SORT REMOVE L8R
    for (int i = 0; i < wieghtVectr.size() - 1; i++)
        std::cout << wieghtVectr[i].weight << " " << std::endl;
    std::cout ;*/

    // the highest wieghted tiles become bombs
    for (int i = 0; i < bombCount; i++) {
        *weightVector[i].tile = 9;
    }

    int neighborBombs;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            // skip bombs
            if (grid[i][j] == 9) continue;

            // for each tile loop through nearby tiles
            neighborBombs = 0;
            for (int xoff = -1; xoff < 2; xoff++) {
                for (int yoff = -1; yoff < 2; yoff++) {
                    int x = i + xoff;
                    int y = j + yoff;
                    if ((x > -1 && x < row) && (y > -1 && y < row) && (grid[x][y] == 9)) neighborBombs++;
                }
            }
            grid[i][j] = neighborBombs;
        }
    }
}


void reveal(std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& displayGrid, int sourceX, int sourceY ) {
    int row = grid[0].size();
    int col = grid.size();
    std::cout << " about to check" << std::endl;
    for (int xoff = -1; xoff < 2; xoff++) {
        for (int yoff = -1; yoff < 2; yoff++) {
            int x = sourceX + xoff;
            int y = sourceY + yoff;
            std::cout << " checking x : y " << x << " : " << y << std::endl;
            if ((x > -1 && x < row) && (y > -1 && y < col) && (grid[sourceX][sourceY] == 0) && (displayGrid[x][y] == 10)) {
                displayGrid[x][y] = grid[x][y];
                reveal(grid, displayGrid, x, y);
            }
        }
    }

}


// Divide the array into two subarrays, sort them and merge them
void mergeSort(std::vector <bombWieght>& wieghts, int left, int right) {
    if (left < right) {
        // m is the mid point
        int middle = left + (right - left) / 2;

        mergeSort(wieghts, left, middle);
        mergeSort(wieghts, middle + 1, right);

        // Merge sorted subarrays
        merge(wieghts, left, middle, right);
    }
}


void merge(std::vector <bombWieght>& wieghts, int firstIndex, int q, int rightend){

    // Create my left half and right half 
    int n1 = q - firstIndex + 1;
    int n2 = rightend - q;
    bombWieght tempObj;
    std::vector <bombWieght> L(n1, tempObj), M(n2, tempObj);

    for (int i = 0; i < n1; i++)
        L[i] = wieghts[firstIndex + i];
    for (int j = 0; j < n2; j++)
        M[j] = wieghts[q + 1 + j];

    // sync index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = firstIndex;

    // Until either end of either left or mid, pick larger left and mid; place in correct position of wieghts
    while (i < n1 && j < n2) {
        if (L[i].weight <= M[j].weight) {
            wieghts[k] = L[i];
            i++;
        }
        else {
            wieghts[k] = M[j];
            j++;
        }
        k++;
    }

    // When run out of wieghts in left or right side, put remaining wieghts in total wieghts
    while (i < n1) {
        wieghts[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        wieghts[k] = M[j];
        j++;
        k++;
    }
}