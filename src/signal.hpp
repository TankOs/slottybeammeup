#pragma once

#include <vector>
#include <functional>

template <class Slot>
class Signal {
  public:
    void add_slot(std::function<Slot> slot);

    template <class ...Args>
    void call(Args... args);

  private:
    std::vector<std::function<Slot>> _slots;
};

#include "signal.inl"
