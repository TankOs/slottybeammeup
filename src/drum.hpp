#pragma once

#include <SFML/Graphics.hpp>

class Drum : public sf::Drawable, public sf::Transformable {
  public:
    Drum(
      const std::vector<const sf::Texture*>& textures,
      std::size_t picture_count,
      float stepping = 17.0f,
      float acceleration = 55.0f
    );

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time time);
    void setRunning(bool running);
    bool getRunning() const;

  private:
    const std::vector<const sf::Texture*>& _textures;
    std::size_t _picture_count;
    std::size_t _active_picture;
    float _stepping;
    float _offset;
    float _speed;
    float _acceleration;
    bool _running;
};
