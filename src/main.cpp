#include "config.hpp"
#include "vector.hpp"
#include "drum.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <cstdint>
#include <cassert>
#include <iostream>

const sf::VideoMode VIDEO_MODE(1280, 720);
const std::size_t DRUM_COUNT = 5;
const std::size_t PICTURE_COUNT = 3;
const sf::Time SIM_STEP = sf::milliseconds(1000 / 100); // 100 FPS.

int main() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  sf::RenderWindow window(VIDEO_MODE, "SFML", sf::Style::Close);
  window.setFramerateLimit(120);

  // Load sounds.
  sf::SoundBuffer stop_buffer;
  sf::SoundBuffer start_buffer;

  stop_buffer.loadFromFile(ASSETS_PATH + "/stop.wav");
  start_buffer.loadFromFile(ASSETS_PATH + "/start.wav");

  sf::Sound stop_sound(stop_buffer);
  sf::Sound start_sound(start_buffer);

  // Load textures.
  sf::Texture vegas_texture;
  sf::Texture frame_texture;
  sf::Texture apple_texture;
  sf::Texture coin_texture;
  sf::Texture bar_texture;
  sf::Texture cherries_texture;
  sf::Texture sfml_texture;

  vegas_texture.loadFromFile(ASSETS_PATH + "/lasvegas.jpg");
  frame_texture.loadFromFile(ASSETS_PATH + "/frame.png");
  apple_texture.loadFromFile(ASSETS_PATH + "/apple.png");
  coin_texture.loadFromFile(ASSETS_PATH + "/coin.png");
  bar_texture.loadFromFile(ASSETS_PATH + "/bar.png");
  cherries_texture.loadFromFile(ASSETS_PATH + "/cherries.png");
  sfml_texture.loadFromFile(ASSETS_PATH + "/sfml.png");

  frame_texture.setRepeated(true);

  // Sprites.
  sf::Sprite vegas_sprite(vegas_texture);
  sf::Sprite frame_sprite(frame_texture);

  // Pictures.
  std::vector<const sf::Texture*> picture_textures;
  picture_textures.push_back(&apple_texture);
  picture_textures.push_back(&coin_texture);
  picture_textures.push_back(&bar_texture);
  picture_textures.push_back(&cherries_texture);
  picture_textures.push_back(&sfml_texture);

  // Drums.
  std::vector<Drum> drums;
  sf::Vector2f drum_size(
    static_cast<float>(frame_texture.getSize().x),
    static_cast<float>(frame_texture.getSize().y)
  );
  sf::Vector2f frame_size(drum_size.x * DRUM_COUNT, drum_size.y);

  {
    // Absolutely positioned, because rendered in render texture.
    sf::Vector2f drum_position(0.0f, 0.0f);

    for(std::size_t drum_idx = 0; drum_idx < DRUM_COUNT; ++drum_idx) {
      drums.emplace_back(picture_textures, PICTURE_COUNT);

      auto& drum = drums.back();
      drum.setPosition(drum_position);
      drum_position.x += drum_size.x;

      // Setup callbacks.
      drum.on_stop.add_slot([&stop_sound]() {
          stop_sound.play();
      });

      drum.on_start.add_slot([&start_sound]() {
          start_sound.play();
      });
    }
  }

  // We'll render the drums' pictures to a render texture in order to properly
  // apply a scissor to it. SFML only has limited support for such things.
  sf::RenderTexture drums_render_texture;
  drums_render_texture.create(
    static_cast<unsigned int>(frame_size.x),
    static_cast<unsigned int>(frame_size.y)
  );

  sf::Sprite drums_sprite(drums_render_texture.getTexture());
  drums_sprite.setPosition(
    static_cast<float>(window.getSize().x) / 2.0f - frame_size.x / 2.0f,
    static_cast<float>(window.getSize().y) / 2.0f - frame_size.y / 2.0f
  );

  // Frame visual.
  sf::VertexArray frame_visual(sf::PrimitiveType::Quads, 4);

  frame_visual[0] = sf::Vertex({0.0f, 0.0f}, {0.0f, 0.0f});
  frame_visual[1] = sf::Vertex({0.0f, frame_size.y}, {0.0f, frame_size.y});
  frame_visual[2] = sf::Vertex(
    {frame_size.x, frame_size.y}, {frame_size.x, frame_size.y}
  );
  frame_visual[3] = sf::Vertex({frame_size.x, 0.0f}, {frame_size.x, 0.0f});

  sf::RenderStates frame_states;
  frame_states.texture = &frame_texture;
  frame_states.transform.translate(drums_sprite.getPosition());

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
            drum.toggle();
          }
        }
        else if(
          event.key.code >= sf::Keyboard::Key::Num1 &&
          event.key.code <= sf::Keyboard::Key::Num9
        ) {
          std::size_t index = static_cast<std::size_t>(
            event.key.code - sf::Keyboard::Key::Num1
          );

          if(index < drums.size()) {
            drums[index].toggle();
          }
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
    window.draw(frame_visual, frame_states);

    window.display();
  }

  return 0;
}
