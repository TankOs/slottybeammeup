
template <class Slot>
void Signal<Slot>::add_slot(std::function<Slot> slot) {
  _slots.push_back(slot);
}

template <class Slot>
template <class ...Args>
void Signal<Slot>::call(Args... args) {
  for(auto& slot : _slots) {
    slot(args...);
  }
}
