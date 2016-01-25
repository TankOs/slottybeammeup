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
  _stop_shift(0.0f),
  _running(true),
  _stop(false)
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
        _offset -
        _stop_shift
      )
  });

  std::size_t texture_idx =
    (_active_picture + (_picture_count - 2)) % _textures.size()
  ;

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
  float sim_seconds = time.asSeconds();
  float picture_height = _textures[0]->getSize().y;

  if(_running == true) {
    _velocity += _max_acceleration * sim_seconds;
    _velocity = std::max(0.0f, std::min(_max_velocity, _velocity));

    _offset -= sim_seconds * _velocity;

    if(_offset <= -picture_height) {
      while(_offset <= -picture_height) {
        _active_picture = (_active_picture + 1) % _textures.size();
        _offset += picture_height;
      }

      if(_stop == true) {
        _offset = 0.0f;
        _velocity = 0.0f;
        _running = false;
        _stop = false;
        _stop_shift = picture_height * 0.08f;
      }
    }
  }
  else {
    if(_stop_shift > 0.0f) {
      _stop_shift = std::max(
        0.0f,
        _stop_shift - (sim_seconds * picture_height)
      );
    }
  }
}

void Drum::start() {
  _running = true;
  _stop = false;
  _stop_shift = 0.0f;
}

void Drum::stop() {
  _stop = true;
}

void Drum::toggle() {
  if(_running == true) {
    stop();
  }
  else {
    start();
  }
}

bool Drum::is_running() const {
  return _running;
}

bool Drum::is_stopping() const {
  return _stop;
}

float Drum::get_velocity() const {
  return _velocity;
}

float Drum::get_max_velocity() const {
  return _max_velocity;
}
