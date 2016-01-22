template <class T>
sf::Vector2<T> get_rect_position(const sf::Rect<T>& rect) {
  return sf::Vector2<T>(rect.left, rect.top);
}

template <class T>
sf::Vector2<T> get_rect_size(const sf::Rect<T>& rect) {
  return sf::Vector2<T>(rect.width, rect.height);
}
