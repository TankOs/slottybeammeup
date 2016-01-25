#pragma once

#include <SFML/Graphics.hpp>

class Drum : public sf::Drawable, public sf::Transformable {
  public:
    Drum(
      const std::vector<const sf::Texture*>& textures,
      std::size_t picture_count,
      float max_velocity = 1500.0f,
      float acceleration = 4000.0f
    );

    void start();
    bool is_running() const;

    void stop();
    bool is_stopping() const;

    void toggle();

    float get_max_velocity() const;
    float get_velocity() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time time);

  private:
    const std::vector<const sf::Texture*>& _textures;
    std::size_t _picture_count;
    std::size_t _active_picture;
    float _max_velocity;
    float _offset;
    float _velocity;
    float _max_acceleration;
    float _stop_shift;
    bool _running;
    bool _stop;
};
