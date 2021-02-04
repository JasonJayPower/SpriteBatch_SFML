# SpriteBatch_SFML

How to use

```
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    SpriteBatch spriteBatch(window);

    sf::Texture tex;
    tex.loadFromFile("file.png");

    sf::Sprite sprite;
    sprite.setTexture(tex);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        spriteBatch.begin(SpriteSortMode::None);
        for (int i = 0; i < 10000; i++) {
            spriteBatch.draw(sprite);
            sprite.setPosition(sf::Vector2f(rand() % 800, rand() % 600));
        }
        spriteBatch.end();
        window.display();
    }

    return 0;
}
```

Sortmodes
```
SpriteSortMode::None           (No sorting is used)
SpriteSortMode::FrontToBack    (Sorts and Renders sprites from the highest Y position to the lowest Y position))
SpriteSortMode::BackToFront    (Sorts and Renders sprites from the lowest Y position to the highest Y position))
SpriteSortMode::Texture        (Sorts textures to minimise texture switching)
```
