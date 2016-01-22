#include "config.hpp"
#include "vector.hpp"
#include "drum.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <cstdint>
#include <cassert>
#include <iostream>


const sf::VideoMode VIDEO_MODE(1280, 720);
const std::size_t DRUM_SIZE = 3;

int main() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  sf::RenderWindow window(VIDEO_MODE, "SFML", sf::Style::Close);
  window.setFramerateLimit(120);

  // Load textures.
  sf::Texture drums_texture;
  sf::Texture apple_texture;
  sf::Texture coin_texture;
  sf::Texture bar_texture;
  sf::Texture cherries_texture;

  drums_texture.loadFromFile(ASSETS_PATH + "/drums.png");
  apple_texture.loadFromFile(ASSETS_PATH + "/apple.png");
  coin_texture.loadFromFile(ASSETS_PATH + "/coin.png");
  bar_texture.loadFromFile(ASSETS_PATH + "/bar.png");
  cherries_texture.loadFromFile(ASSETS_PATH + "/cherries.png");

  // Sprites.
  sf::Sprite drums_sprite(drums_texture);
  drums_sprite.setOrigin({
      static_cast<float>(drums_texture.getSize().x) / 2.0f,
      static_cast<float>(drums_texture.getSize().y) / 2.0f
  });
  drums_sprite.setPosition({
      static_cast<float>(window.getSize().x) / 2.0f,
      static_cast<float>(window.getSize().y) / 2.0f
  });

  // Pictures.
  std::vector<const sf::Texture*> picture_textures;
  picture_textures.push_back(&apple_texture);
  picture_textures.push_back(&coin_texture);
  picture_textures.push_back(&bar_texture);
  picture_textures.push_back(&cherries_texture);

  // Drums.
  std::vector<Drum> drums;
  drums.emplace_back(picture_textures, DRUM_SIZE);
  drums.emplace_back(picture_textures, DRUM_SIZE);
  drums.emplace_back(picture_textures, DRUM_SIZE);

  {
    float drum_width =
      static_cast<float>(drums_texture.getSize().x / drums.size());
    sf::Vector2f drum_center(
      drum_width / 2.0f, drums_texture.getSize().y / 2.0f
    );
    // lol just guessing right now.. Almost correct though! ;-)
    sf::Vector2f drum_position =
      drums_sprite.getPosition() -
      sf::Vector2f(
        drums_sprite.getOrigin().x / 2.0f + drum_center.x / 2.0f, 0.0f
      )
    ;

    for(std::size_t drum_idx = 0; drum_idx < drums.size(); ++drum_idx) {
      Drum& drum = drums[drum_idx];

      drum.setOrigin(drum_center);
      drum.setPosition(drum_position);

      drum_position.x += drum_width;
    }
  }

  // Loop.
  sf::Event event;
  bool terminate = false;
  sf::Clock frame_clock;
  sf::Time frame_time = sf::Time::Zero;

  while(terminate == false) {
    while(window.pollEvent(event) == true) {
      if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Escape) {
          terminate = true;
        }
      }
    }

    // Update simulation.
    frame_time = frame_clock.restart();

    for(auto& drum : drums) {
      drum.update(frame_time);
    }

    // Render.
    window.clear({255, 255, 255});

    for(const auto& drum : drums) {
      window.draw(drum);
    }

    window.draw(drums_sprite);

    window.display();
  }

  return 0;
}
