#include "drum.hpp"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iomanip>

Drum::Drum(
  const std::vector<const sf::Texture*>& textures,
  std::size_t picture_count,
  float max_velocity,
  float acceleration
) :
  sf::Drawable(),
  sf::Transformable(),
  _textures(textures),
  _picture_count(picture_count),
  _active_picture(0),
  _max_velocity(max_velocity),
  _offset(0.0f),
  _velocity(0.0f),
  _max_acceleration(acceleration),
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
        _offset
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
  float picture_height = _textures[0]->getSize().y;
  float target_velocity = (_running == true ? _max_velocity : 0.0f);
  float acceleration = _max_acceleration;
  float sim_seconds = time.asSeconds();

  if(_running == false) {
    float velocity_diff = target_velocity - _velocity;
    float remaining = picture_height + _offset;
    float seconds_to_target = remaining / _velocity;

    acceleration = std::max(
      -_max_acceleration,
      std::min(
        _max_acceleration,
        velocity_diff / seconds_to_target
      )
    );
  }

  _velocity = std::max(
    0.0f,
    std::min(
      _max_velocity,
      _velocity + (acceleration * sim_seconds)
    )
  );
  _offset -= sim_seconds * _velocity;

  while(_offset <= -picture_height) {
    _active_picture = (_active_picture + 1) % _textures.size();
    _offset += picture_height;
  }
}

void Drum::set_running(bool running) {
  _running = running;
}

bool Drum::get_running() const {
  return _running;
}

float Drum::get_velocity() const {
  return _velocity;
}

float Drum::get_max_velocity() const {
  return _max_velocity;
}
