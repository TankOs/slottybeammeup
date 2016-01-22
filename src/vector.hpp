#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

template <class T>
sf::Vector2<T> get_rect_position(const sf::Rect<T>& rect);

template <class T>
sf::Vector2<T> get_rect_size(const sf::Rect<T>& rect);

#include "vector.inl"
