#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

char level[21][101] = {
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "O     OOO          OOOOOOOOO           OOOOOO          OOOOOOOOOOO                  OOOOOOOOOOOOOOOO",
    "O  P                    OOO                            OOOOOOOOOOO                             OOOOO",
    "O     OOO          OOOO OOO O          OOOOOO                                       OOOOOOOOOO OOOOO",
    "OOOOOOOOO          OOOO     O          OOOOOO          OOOOOOOOOOO                  OOOOOOOOOO OOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO          OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO OOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                 OOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   Femei&        OOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO    Bautura&     OOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO      utere      OOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
};


const int WIDTH = 100;
const int HEIGHT = 20;

class Player {
private:
    int health;
    int x, y;

public:
    Player(int x, int y, int health = 100) : x(x), y(y), health(health) {
        place();
    }

    void place() const {
        level[x][y] = 'P';
    }

    void clear() const {
        level[x][y] = ' ';
    }

    void move(int dx, int dy) {
        int nx = x + dx;
        int ny = y + dy;
        if (level[nx][ny] == ' ' || isdigit(level[nx][ny])) {
            clear();
            x = nx;
            y = ny;
            place();
        }
    }
    bool isAlive() const { return health > 0; }

    int getHealth() const { return health; }

    int getX() const { return x; }
    int getY() const { return y; }

    void damage(int amount) {
        health -= amount;
    }
    
    void heal(int amount) {
        health += amount;
    }    
};

class Entity {
protected:
    int x, y;
    char symbol;
    int roomID;

public:
    Entity(int x, int y, char symbol, int roomID) : x(x), y(y), symbol(symbol), roomID(roomID) {}

    virtual void draw() {
        level[x][y] = symbol;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
    virtual void update() {}
};

class Enemy : public Entity {
private:
    int moveCooldown;
    int currentCooldown;
    std::vector<std::pair<int, int>> validArea;

public:
    Enemy(int x, int y, int roomID, const std::vector<std::pair<int, int>>& area)
        : Entity(x, y, '1' + rand() % 4, roomID), validArea(area) {
        moveCooldown = 2 + rand() % 4;
        currentCooldown = 0;
    }

    void update() override {
        if (++currentCooldown >= moveCooldown) {
            currentCooldown = 0;
            int dir = rand() % 4;
            int dx = 0, dy = 0;
            if (dir == 0) dx = -1;
            else if (dir == 1) dx = 1;
            else if (dir == 2) dy = -1;
            else dy = 1;
            int nx = x + dx;
            int ny = y + dy;
            if (level[nx][ny] == ' ' && inRoom(nx, ny)) {
                level[x][y] = ' ';
                x = nx;
                y = ny;
            }
        }
        draw();
    }

    bool inRoom(int nx, int ny) const {
        for (const auto& tile : validArea) {
            if (tile.first == nx && tile.second == ny)
                return true;
        }
        return false;
    }
};

class Friend : public Entity {
public:
    Friend(int x, int y, int roomID) : Entity(x, y, '5' + rand() % 5, roomID) {}
};

class Room {
    private:
        int id;
        int mode; // 0: easy, 1: normal, 2: hard, 3: impossible (future use)
        std::vector<std::pair<int, int>> tiles;
        std::vector<Enemy> enemies;
        std::vector<Friend> friends;
    
    public:
        Room(int id, int mode, const std::vector<std::pair<int, int>>& tiles) 
            : id(id), mode(mode), tiles(tiles) 
        {
            int enemyCount = 0;
            int friendCount = 0;
    
            switch (mode) {
                case 0: // easy
                    enemyCount = 1 + rand() % 2;         // 1-2
                    friendCount = 1 + rand() % 3;        // 1-3
                    break;
                case 1: // normal
                    enemyCount = 2 + rand() % 2;         // 2-3
                    friendCount = rand() % 2;            // 0-1
                    break;
                case 2: // hard
                    enemyCount = 3 + rand() % 3;         // 3-5
                    friendCount = 0;                     // always 0
                    break;
                case 3: // impossible
                    enemyCount = 6 + rand() % 3;         // 6-8
                    friendCount = 0;                     // always 0
                    break;
                default:
                    break;
            }
    
            for (int i = 0; i < enemyCount && !tiles.empty(); ++i) {
                auto [x, y] = tiles[rand() % tiles.size()];
                enemies.emplace_back(x, y, id, tiles);
            }
    
            for (int i = 0; i < friendCount && !tiles.empty(); ++i) {
                auto [x, y] = tiles[rand() % tiles.size()];
                friends.emplace_back(x, y, id);
            }
        }
    
        void drawEntities() {
            for (auto& e : enemies) e.update();
            for (auto& f : friends) f.draw();
        }
        
        void handlePlayerCollision(Player& player) {
            for (size_t i = 0; i < enemies.size(); ++i) {
                if (enemies[i].getX() == player.getX() && enemies[i].getY() == player.getY()) {
                    int val = enemies[i].getSymbol() - '0';
                    int damage = (5 - val) * 20;
                    player.damage(damage);
                    level[player.getX()][player.getY()] = 'P';
                    enemies.erase(enemies.begin() + i);
                    return;
                }
            }
        
            for (size_t i = 0; i < friends.size(); ++i) {
                if (friends[i].getX() == player.getX() && friends[i].getY() == player.getY()) {
                    int val = friends[i].getSymbol() - '0';
                    player.heal(val * 10);
                    level[player.getX()][player.getY()] = 'P';
                    friends.erase(friends.begin() + i);
                    return;
                }
            }
        }
        
    
        int getMode() const { return mode; }

        friend std::ostream& operator<<(std::ostream& out, const Room& room){
            out << "Room ID: " << room.id << " | Mode: ";
            switch (room.mode) {
                case 0: out << "Easy"; break;
                case 1: out << "Normal"; break;
                case 2: out << "Hard"; break;
                case 3: out << "Impossible"; break;
                default: out << "Unknown"; break;
            }
            out << " | Enemies: " << room.enemies.size()
                << " | Friends: " << room.friends.size() << "\n";
            return out;
        };

        bool contains(int x, int y) const {
            for (const auto& tile : tiles)
                if (tile.first == x && tile.second == y)
                    return true;
            return false;
        }
        

    };
    

class Game {
private:
    Player* player;
    std::vector<Room> rooms;
    bool running;

public:
    Game() {
        srand(time(0));
        player = new Player(2, 3);
        running = true;
        initializeRooms();
    }

    ~Game() { delete player; }

    void initializeRooms() {
        rooms.clear();
        std::vector<std::pair<int, int>> room1;
        for (int i = 1; i < 5; ++i)
            for (int j = 9; j < 19; ++j)
                if (level[i][j] == ' ') room1.emplace_back(i, j);
        rooms.emplace_back(0, 0, room1);

        std::vector<std::pair<int, int>> room2;
        for (int i = 1; i < 5; ++i)
            for (int j = 28; j < 40; ++j)
                if (level[i][j] == ' ') room2.emplace_back(i, j);
        rooms.emplace_back(1, 1, room2);

        std::vector<std::pair<int, int>> room3;
        for (int i = 1; i < 6; ++i)
            for (int j = 45; j < 56; ++j)
                if (level[i][j] == ' ') room3.emplace_back(i, j);
        rooms.emplace_back(2, 2, room3);

        std::vector<std::pair<int, int>> room4;
        for (int i = 1; i < 5; ++i)
            for (int j = 66; j < 85; ++j)
                if (level[i][j] == ' ') room4.emplace_back(i, j);
        rooms.emplace_back(3, 3, room4);
    }

    Room* getCurrentRoom() {
        int px = player->getX();
        int py = player->getY();
        for (auto& r : rooms) {
            if (r.contains(px, py))
                return &r;
        }
        return nullptr;
    }

    void reset() {
        // Clear the level of all entities and player
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (isdigit(level[i][j]) || level[i][j] == 'P') {
                    level[i][j] = ' ';
                }
            }
        }

        delete player;
        player = new Player(2, 3);
        initializeRooms();
        running = true;
    }

    void showMenu(bool died) {
        system("cls");
        if (died) {
            std::cout << "Oops, ai picat fraiere!\n";
            std::cout << "Press ENTER to play again or Q to quit...\n";
        } else {
            std::cout << "Welcome to the Treci Sesiunea!\n";
            std::cout << "Press ENTER to start playing...\n";
        }

        while (true) {
            int key = _getch();
            if (key == 13) break; 
            if (died && (key == 'q' || key == 'Q')) {
                running = false;
                break;
            }
        }
    }

    void run() {
        while (true) {
            showMenu(false); 
            reset();

            while (running && player->isAlive()) {
                system("cls");
                Room* current = getCurrentRoom();
                std::cout << "                 Health: " << player->getHealth() << " =============   ";
                if (current) {
                    std::cout << *current << "\n";
                } else {
                    std::cout << "You are not in any SESIUNE.\n";
                }
                for (int i = 0; i < HEIGHT; ++i) std::cout << level[i] << "\n";

                if (_kbhit()) {
                    int key = _getch();
                    if (key == 224) {
                        int arrow = _getch();
                        switch (arrow) {
                            case 72: player->move(-1, 0); break;
                            case 80: player->move(1, 0); break;
                            case 75: player->move(0, -1); break;
                            case 77: player->move(0, 1); break;
                        }
                    } else if (key == 'q') {
                        running = false;
                    }
                }

                for (auto& r : rooms) {
                    r.handlePlayerCollision(*player);
                    r.drawEntities();
                }

                Sleep(50);
            }

            system("cls");
            if (!player->isAlive()) {
                showMenu(true);
                if (!running) break;
            } else {
                std::cout << "You quit the game.\n";
                break;
            }
        }
    }
};

int main() {
    Game g;
    g.run();
    return 0;
}