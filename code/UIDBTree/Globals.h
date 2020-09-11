#pragma once

#include <cstddef>
#include <memory>
#include <experimental/memory>
#include <vector>
#include <map>
#include <optional>
#include <string>
#include <sstream>

#define observer_ptr std::experimental::observer_ptr
#define make_observer std::experimental::make_observer

#define ByteVector std::vector<unsigned char>