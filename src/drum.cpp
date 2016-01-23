#include "drum.hpp"

#include <iostream>
#include <cstdlib>
#include <cassert>

Drum::Drum(
  const std::vector<const sf::Texture*>& textures,
  std::size_t picture_count,
  float stepping,
  float acceleration
) :
  sf::Drawable(),
  sf::Transformable(),
  _textures(textures),
  _picture_count(picture_count),
  _active_picture(0),
  _stepping(stepping),
  _offset(0.0f),
  _speed(0.0f),
  _acceleration(acceleration),
  _running(true)
{
  assert(_textures.size() > 0);
}

void Drum::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  float picture_height = _textures[0]->getSize().y;

  sf::Sprite picture;
  picture.setPosition({
      0.0f,
      (
        -2.0f * static_cast<float>(picture_height) +
        (_offset * picture_height)
      )
  });

  std::size_t texture_idx =
    (_active_picture + (_picture_count - 2)) % _textures.size();

  for(
    std::size_t picture_idx = 0;
    picture_idx < _picture_count + 2;
    ++picture_idx
  ) {
    picture.setPosition(
      picture.getPosition() +
      sf::Vector2f(0.0f, picture_height)
    );
    picture.setTexture(*_textures[texture_idx], true);

    target.draw(picture, states);

    texture_idx = (texture_idx + 1) % _textures.size();
  }
}

void Drum::update(sf::Time time) {
  float target_speed = (_running == true ?  1.0f : 0.0f);
  float sim_seconds = time.asSeconds();

  if(target_speed > _speed) {
    _speed += _acceleration * sim_seconds;
  }
  else if(target_speed < _speed) {
    _speed -= _acceleration * sim_seconds;
  }

  _speed = std::max(0.0f, std::min(1.0f, _speed));
  _offset -= sim_seconds * (_stepping * _speed);

  while(_offset <= -1.0f) {
    _active_picture = (_active_picture + 1) % _textures.size();
    _offset += 1.0f;
  }
}

void Drum::set_running(bool running) {
  _running = running;
}

bool Drum::get_running() const {
  return _running;
}

float Drum::get_speed() const {
  return _speed;
}
