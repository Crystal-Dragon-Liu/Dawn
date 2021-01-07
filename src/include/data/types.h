#pragma once

#include "data/values.h"

namespace dawn {
class Type;

extern Type *singleton[2];

// TODO VARCHAR
enum class TypeId { INVALID = -1, BOOLEAN, INTEGER, DECIMAL, CHAR };

enum class CmpResult { TRUE = 0, FALSE };

class Value;

class Type {
public:
    explicit Type() = default;
    virtual ~Type() = default;

    // compare operation
    virtual CmpResult cmp_less(const Value &left, const Value &right) = 0;
    virtual CmpResult cmp_less_and_eq(const Value &left, const Value &right) = 0;
    virtual CmpResult cmp_eq(const Value &left, const Value &right) = 0;
    virtual CmpResult cmp_greater_and_eq(const Value &left, const Value &right) = 0;
    virtual CmpResult cmp_greater(const Value &left, const Value &right) = 0;

    virtual Value minus(const Value &left, const Value &right) = 0;
    virtual Value add(const Value &left, const Value &right) = 0;
    virtual Value multiply(const Value &left, const Value &right) = 0;
    virtual Value divide(const Value &left, const Value &right) = 0;
    virtual Value min(const Value &left, const Value &right) = 0;
    virtual Value max(const Value &left, const Value &right) = 0;

    static Type* get_instance(TypeId type_id) { return singleton[(int)type_id]; }
};
    
} // namespace dawn
