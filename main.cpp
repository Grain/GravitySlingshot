//Gravity assist program
//Dec 2012
//By Paul He

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>

const float PI = std::atan(1.0f) * 4.0f;
const float GRAV = 1000;
const int FPS = 30;

//window
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFMLSling");
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "SFMLSling", sf::Style::Fullscreen);

double distance(sf::Vector2f a, sf::Vector2f b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

class planet
{
private:
    float x, y;
    int orbitRadius;
    int orbitSpeed;

public:
    sf::CircleShape me; //was lazy
    planet(int offset = 0, int radius = 20, sf::Color color = sf::Color::Green)
    {
        me.setRadius(radius);
        me.setFillColor(color);
        me.setOrigin(radius, radius);

        x = offset;
        y = offset;
        orbitRadius = 200;
        orbitSpeed = 40;   //smaller = faster

        me.setPosition(orbitRadius*sin(x/(PI * orbitSpeed)) + window.getSize().x/2, orbitRadius*cos(y/(PI * orbitSpeed)) + window.getSize().y/2);
    }

    void setOffset(int offset)
    {
        x = offset;
        y = offset;
    }

    void setColor(sf::Color color)
    {
        me.setFillColor(color);
    }

    void setRadius(float radius)
    {
        me.setRadius(radius);
        me.setOrigin(radius, radius);
    }

    void setOrbitRadius(int radius)
    {
        orbitRadius = radius;
    }

    void setOrbitSpeed(int speed)
    {
        orbitSpeed = speed;
    }

    void update()
    {
        me.setPosition(orbitRadius*sin(x++/(PI * orbitSpeed)) + window.getSize().x/2, orbitRadius*cos(y++/(PI * orbitSpeed)) + window.getSize().y/2);
        window.draw(me);
    }
};

int main()
{
    //window
    window.setFramerateLimit(FPS);

    //textures
    sf::Texture sol;
    sf::Texture planetImages[10];

    int a;
    char b[] = "0.png";
    for(a = 0; a < 10; ++a) //planet images from http://jangodb.deviantart.com/art/Planet-Renders-170504314
    {
        b[0] = a + 48;
        planetImages[a].loadFromFile(b);
    }

    sol.loadFromFile("sun.png");

    //speedometer
    sf::Text text("");
    text.setPosition(15, 10);
    text.setColor(sf::Color::White);

    //sun
    sf::CircleShape sun(50.f, 50);
    sun.setOrigin(sun.getRadius(), sun.getRadius());
    sun.setFillColor(sf::Color::White);
    sun.setPosition(window.getSize().x/2, window.getSize().y/2);
    sun.setTexture(&sol, true);

    //planets
    int planets = 1;
    planet array[5];
    array[0].setOffset(0);
    array[0].setColor(sf::Color::White);
    array[1].setColor(sf::Color::White);

    //spaceship
    sf::CircleShape ship(5.f);
    ship.setOrigin(ship.getRadius(), ship.getRadius());
    ship.setFillColor(sf::Color::White);
    ship.setPosition(window.getSize().x/2 - 100, window.getSize().y/2 + 150);

    double xVel, yVel;
    xVel = 4.5;
    yVel = -0.90;

    //viewport
    sf::View view;
    view.setSize(400,300);
    view.setCenter(array[0].me.getPosition());
    bool viewPort = false;

    //vars and stuff
    bool reset = true;
    bool drawSun = true;
    int mode = 1;
    srand(time(NULL));

    //MAIN LOOP

    while (window.isOpen())
    {
        int i;

        sf::Event event;
        while (window.pollEvent(event)) //check for exit program
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for(i = 0; i < planets; ++i)        //check for collision with planet
        {
            if(distance(ship.getPosition(), array[i].me.getPosition()) < ship.getRadius() + array[i].me.getRadius())
            {
                reset = true;
                sf::sleep(sf::milliseconds(750));
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))  //check for keyboard keys
        {
            mode = 1;   //randoms
            reset = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            mode = 2;   //180 degree speed up, frame of reference of planet
            reset = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            mode = 3;   //180 degree speed up
            reset = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            mode = 4;   //more realistic leaving solar system speed up
            reset = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        {
            mode = 5;   //180 degree slow down
            reset = true;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
        {
            mode = 6;   //more realistic going towards sun slow down
            reset = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        if(reset)   //reset simulation according to current mode
        {
            if(mode == 1)
            {
                array[0].me.setTexture(&planetImages[rand() % 10], true);
                array[1].me.setTexture(&planetImages[rand() % 10], true);

                viewPort = false;
                window.setView(window.getDefaultView());

                planets = rand() % 2 + 1;
                for(i = 0; i < planets; ++i)
                {
                    array[i].setRadius(rand() % 20 + 10);
                    array[i].setOrbitSpeed(rand() % 30 + 20);
                    array[i].setOffset(rand() % 10000);
                }
                int temp = rand() % 100 + 150;
                array[0].setOrbitRadius(temp);
                array[0].setOrbitRadius(temp + 150);

                ship.setPosition(rand() % (window.getSize().x - 20) + 10, rand() % (window.getSize().y - 20) + 10);
                xVel = rand() % 4 - 2;
                yVel = rand() % 4 - 2;
                drawSun = true;
            }
            else if(mode == 2)
            {
                array[0].me.setTexture(&planetImages[6], true);

                viewPort = true;
                planets = 1;
                array[0].setRadius(20);
                array[0].setOrbitSpeed(40);
                array[0].setOrbitRadius(200);
                ship.setPosition(window.getSize().x/2 +200, window.getSize().y/2 + 200);
                xVel = -2;
                yVel = 0.90;
                array[0].setOffset(0);
                drawSun = false;
            }
            else if(mode == 3)
            {
                array[0].me.setTexture(&planetImages[6], true);

                viewPort = false;
                planets = 1;
                array[0].setRadius(20);
                array[0].setOrbitSpeed(40);
                array[0].setOrbitRadius(200);
                window.setView(window.getDefaultView());
                ship.setPosition(window.getSize().x/2 +200, window.getSize().y/2 + 200);
                xVel = -2;
                yVel = 0.90;
                array[0].setOffset(0);
                drawSun = true;
            }
            else if(mode == 4)
            {
                array[0].me.setTexture(&planetImages[1], true);

                viewPort = false;
                planets = 1;
                array[0].setRadius(20);
                array[0].setOrbitSpeed(40);
                array[0].setOrbitRadius(300);
                window.setView(window.getDefaultView());
                ship.setPosition(window.getSize().x/2 + 150, window.getSize().y/2 - 25);
                xVel = 4;
                yVel = 0;
                array[0].setOffset(1000);
                drawSun = true;
            }
            else if(mode == 5)
            {
                array[0].me.setTexture(&planetImages[5], true);

                viewPort = false;
                planets = 1;
                array[0].setRadius(20);
                array[0].setOrbitSpeed(40);
                array[0].setOrbitRadius(200);
                window.setView(window.getDefaultView());
                ship.setPosition(window.getSize().x/2 - 100, window.getSize().y/2 + 150);
                xVel = 4.5;
                yVel = -0.90;
                array[0].setOffset(0);
                drawSun = true;
            }
            else if(mode == 6)
            {
                array[0].me.setTexture(&planetImages[8], true);
                array[1].me.setTexture(&planetImages[0], true);

                viewPort = false;
                planets = 2;
                array[0].setRadius(20);
                array[0].setOrbitSpeed(40);
                array[0].setOrbitRadius(300);
                array[0].setOffset(3000);

                array[1].setRadius(17);
                array[1].setOrbitSpeed(45);
                array[1].setOrbitRadius(200);
                array[1].setOffset(5010);

                window.setView(window.getDefaultView());
                ship.setPosition(window.getSize().x/2 - 400, window.getSize().y/2 + 250);
                xVel = 4.9;
                yVel = 0;
                drawSun = true;
            }

            reset = false;
        }

        if (ship.getPosition().x - ship.getRadius() < 0)    //check for collision with screen borders
        {
            ship.setPosition(ship.getRadius() + 1, ship.getPosition().y);
            xVel = -xVel;
        }
        else if (ship.getPosition().x + ship.getRadius() > window.getSize().x)
        {
            ship.setPosition(window.getSize().x - ship.getRadius() - 1, ship.getPosition().y);
            xVel = -xVel;
        }

        if (ship.getPosition().y - ship.getRadius() < 0)
        {
            ship.setPosition(ship.getPosition().x, ship.getRadius() + 1);
            yVel = -yVel;
        }
        else if (ship.getPosition().y + ship.getRadius() > window.getSize().y)
        {
            ship.setPosition(ship.getPosition().x, window.getSize().y - ship.getRadius() - 1);
            yVel = -yVel;
        }

        if (viewPort)
        {
            view.setCenter(array[0].me.getPosition());
            window.setView(view);
        }

        //start drawing
        window.clear();
        if (drawSun)
        {
            window.draw(sun);
        }

        for(i = 0; i < planets; ++i)        //calculate force for each active planet, then draw/update planet
        {
            double angle;

            if (array[i].me.getPosition().x - ship.getPosition().x != 0)
            {
                angle = atan((array[i].me.getPosition().y - ship.getPosition().y)/(array[i].me.getPosition().x - ship.getPosition().x));
            }
            else
            {
                angle = atan((array[i].me.getPosition().y - ship.getPosition().y)/(array[i].me.getPosition().x - ship.getPosition().x + 0.01));
            }

            double force = GRAV/pow(distance(array[i].me.getPosition(), ship.getPosition()), 2);

            if(ship.getPosition().x > array[i].me.getPosition().x)
            {
                xVel -= cos(angle) * force;
                yVel -= sin(angle) * force;
            }
            else
            {
                xVel += cos(angle) * force;
                yVel += sin(angle) * force;
            }

            array[i].update();
        }

        //draw the rest

        char temp[20];
        sprintf(temp, "%.0f Pixels/s", sqrt(xVel * xVel + yVel * yVel) * FPS);
        text.setString(temp);

        ship.move(xVel, yVel);
        window.draw(ship);
        window.draw(text);
        window.display();
    }

    return 0;
}
