#pragma once

#include <vector>
   
#include "util/util.h"
#include "util/config.h"
#include "util/rwlatch.h"
#include "table/rid.h"
#include "data/values.h"
#include "table/table_schema.h"

namespace dawn {

/**
 * ATTENTION no lock!
 */
class Tuple {
friend class TablePage;
public:
    Tuple() = default;
    Tuple(std::vector<Value> *values, const TableSchema &schema) {
        init(values, schema);
    }

    Tuple(std::vector<Value> *values, const TableSchema &schema, const RID &rid) : rid_(rid) {
        init(values, schema);
    }

    // deep copy
    Tuple(const Tuple &tuple) {
        if (allocated_) {
            delete[] data_;
        }
        allocated_ = tuple.allocated_;
        size_ = tuple.size_;
        rid_ = tuple.rid_;
        if (allocated_) {
            data_ = new char[size_];
            memcpy(data_, tuple.data_, size_);
        }
    }

    ~Tuple() {
        if (allocated_) {
            delete[] data_;
        }
    }
    
    inline size_t_ get_size() const { return size_; }
    inline char* get_data() const { return data_; }

    inline RID get_rid() const { return rid_; }
    inline void set_rid(const RID &rid) { rid_ = rid; }

    Value get_value(const TableSchema &schema, int idx) const;
    void set_value(const TableSchema &schema, Value *value, int idx);

    inline bool is_allocated() const { return allocated_; }
    string_t to_string(const TableSchema &schema);

    inline void serialize_to(char *dst) const {
        if (allocated_) {
            memcpy(dst, data_, size_);
        }
    }

    inline void deserialize_from(char *src) {
        if (allocated_) {
            memcpy(data_, src, size_);
        }
    }

    bool operator==(const Tuple &tuple);

    // deep copy
    Tuple& operator=(const Tuple &tuple) {
        if (allocated_) {
            delete[] data_;
        }
        allocated_ = tuple.allocated_;
        size_ = tuple.size_;
        rid_ = tuple.rid_;
        if (allocated_) {
            data_ = new char[size_];
            memcpy(data_, tuple.data_, size_);
        }
        return *this;
    }

private:
    void init(std::vector<Value> *values, const TableSchema &schema);

    bool allocated_ = false;
    RID rid_;
    size_t_ size_ = -1;
    char *data_;
};

} // namespace dawn
