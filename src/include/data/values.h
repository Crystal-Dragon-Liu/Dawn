#pragma once

#include <string>
#include <string.h>

#include "data/types.h"
#include "util/config.h"

namespace dawn {
enum class TypeId;

class Value {
public:
    Value();
    Value(bool val);
    Value(__INT32_TYPE__ val);
    Value(double val);
    Value(char *val, int size);
    Value(const string_t &val);
    ~Value();

    void swap(Value &val) {
        std::swap(val.type_id_, type_id_);
        std::swap(val.value_, value_);
        std::swap(val.str_size_, str_size_);
    }

    // similar to shallow copy
    void load(const Value &val) {
        type_id_ = val.type_id_;
        value_ = val.value_;
        str_size_ = val.str_size_;
    }

    TypeId get_type_id() const { return type_id_; }

    template<typename T>
    T get_value() const {
        return *reinterpret_cast<const T*>(&value_);
    }

    int get_varchar_size() const { return str_size_; }

private:
    union values {
        bool boolean;
        __INT32_TYPE__ integer;
        double decimal;
        char *varchar;
    };
    
    values value_;
    TypeId type_id_;
    int str_size_;
};

} // namespace dawn
