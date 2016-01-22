#pragma once

#include <SFML/Graphics.hpp>

class Drum : public sf::Drawable, public sf::Transformable {
  public:
    Drum(
      const std::vector<const sf::Texture*>& textures,
      std::size_t picture_count,
      float stepping = 1.0f
    );

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time time);

  private:
    const std::vector<const sf::Texture*>& _textures;
    std::size_t _picture_count;
    std::size_t _active_picture;
    float _stepping;
    float _offset;
};
