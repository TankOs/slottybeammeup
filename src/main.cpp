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
const sf::Time SIM_STEP = sf::milliseconds(1000 / 100); // 100 FPS.

int main() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  sf::RenderWindow window(VIDEO_MODE, "SFML", sf::Style::Close);
  window.setFramerateLimit(120);

  // Load textures.
  sf::Texture vegas_texture;
  sf::Texture frame_texture;
  sf::Texture apple_texture;
  sf::Texture coin_texture;
  sf::Texture bar_texture;
  sf::Texture cherries_texture;

  vegas_texture.loadFromFile(ASSETS_PATH + "/lasvegas.jpg");
  frame_texture.loadFromFile(ASSETS_PATH + "/frame.png");
  apple_texture.loadFromFile(ASSETS_PATH + "/apple.png");
  coin_texture.loadFromFile(ASSETS_PATH + "/coin.png");
  bar_texture.loadFromFile(ASSETS_PATH + "/bar.png");
  cherries_texture.loadFromFile(ASSETS_PATH + "/cherries.png");

  // Sprites.
  sf::Sprite vegas_sprite(vegas_texture);
  sf::Sprite frame_sprite(frame_texture);
  frame_sprite.setOrigin({
      static_cast<float>(frame_texture.getSize().x) / 2.0f,
      static_cast<float>(frame_texture.getSize().y) / 2.0f
  });
  frame_sprite.setPosition({
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
      static_cast<float>(frame_texture.getSize().x / drums.size())
    ;
    sf::Vector2f drum_position(0.0f, 0.0f);

    for(std::size_t drum_idx = 0; drum_idx < drums.size(); ++drum_idx) {
      Drum& drum = drums[drum_idx];
      drum.setPosition(drum_position);
      drum_position.x += drum_width;
    }
  }

  // We'll render the drums' pictures to a render texture in order to properly
  // apply a scissor to it. SFML only has limited support for such things.
  sf::RenderTexture drums_render_texture;
  drums_render_texture.create(
    frame_texture.getSize().x,
    frame_texture.getSize().y
  );

  sf::Sprite drums_sprite(drums_render_texture.getTexture());
  drums_sprite.setPosition(frame_sprite.getPosition());
  drums_sprite.setOrigin(frame_sprite.getOrigin());

  // Blur shader.
  sf::Shader blur_shader;
  blur_shader.loadFromFile(ASSETS_PATH + "/blur.glsl", sf::Shader::Fragment);
  blur_shader.setParameter("texture", sf::Shader::CurrentTexture);

  // Loop.
  sf::Event event;
  bool terminate = false;
  sf::Clock frame_clock;
  sf::Time frame_time;
  sf::Time sim_time_bucket;

  while(terminate == false) {
    while(window.pollEvent(event) == true) {
      if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Key::Escape) {
          terminate = true;
        }
        else if(event.key.code == sf::Keyboard::Key::S) {
          for(auto& drum : drums) {
            drum.set_running(!drum.get_running());
          }
        }
        else if(event.key.code == sf::Keyboard::Key::Num1) {
          drums[0].set_running(!drums[0].get_running());
        }
        else if(event.key.code == sf::Keyboard::Key::Num2) {
          drums[1].set_running(!drums[1].get_running());
        }
        else if(event.key.code == sf::Keyboard::Key::Num3) {
          drums[2].set_running(!drums[2].get_running());
        }
      }
    }

    // Update simulation.
    frame_time = frame_clock.restart();
    sim_time_bucket += frame_time;

    while(sim_time_bucket >= SIM_STEP) {
      for(auto& drum : drums) {
        drum.update(SIM_STEP);
      }

      sim_time_bucket -= SIM_STEP;
    }

    // Render.
    drums_render_texture.clear({255, 255, 255, 200});

    sf::RenderStates states;
    states.shader = &blur_shader;

    for(const auto& drum : drums) {
      blur_shader.setParameter(
        "blur_radius",
        (drum.get_velocity() / drum.get_max_velocity()) * 0.09f
      );

      drums_render_texture.draw(drum, states);
    }

    drums_render_texture.display();

    window.clear();
    window.draw(vegas_sprite);
    window.draw(drums_sprite);
    window.draw(frame_sprite);

    window.display();
  }

  return 0;
}
