#pragma once

#include <unordered_map>

#include "storage/page/page.h"
#include "util/util.h"
#include "util/config.h"
#include "util/rwlatch.h"
#include "storage/disk/disk_manager.h"
#include "buffer/buffer_pool_manager.h"

namespace dawn {

/**
 * catalog header layout:
 * ------------------------------------------------------------------------
 * |                          common header (64)                          |
 * ------------------------------------------------------------------------
 * | table_num (4) | tb_name ptr 1 (4) | tb_name size 1 (4) | page id (4) |
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * | tb_name ptr 2 (4) | tb_name size 2 (4) | page id (4) |      ....     |
 * ------------------------------------------------------------------------
 * | tb_name ptr n (4) | tb_name size n (4) | page id (4) |
 * ------------------------------------------------------------------------
 * |  ...free space... | tb_name (x) | tb_name (x) |  ....  | tb_name (x) |
 * ------------------------------------------------------------------------
 *                     ^
 *                     free space pointer
 * 
 * flush itself every time the data have been modified.
 * It's inefficient but can ensure the data to be stored on the disk.
 */
class CatalogPage {
public:
    CatalogPage();

    ~CatalogPage() {
        // return the page
        bpm_->flush_page(page_id_);
        bpm_->unpin_page(page_id_);
    }

    void init_catalog(page_id_t page_id, BufferPoolManager *bpm);

    table_id_t get_table_id(const string_t &tb_name) {
        latch_.r_lock();
        auto iter = tb_name_to_id_.find(tb_name);
        if (iter == tb_name_to_id_.end()) {
            latch_.r_unlock();
            return -1;
        }
        table_id_t table_id = iter->second;
        latch_.r_unlock();
        return table_id;
    }

    string_t get_table_name(table_id_t table_id) {
        latch_.r_lock();
        auto iter = tb_id_to_name_.find(table_id);
        if (iter == tb_id_to_name_.end()) {
            latch_.r_unlock();
            return string_t("");
        }
        string_t tb_name = iter->second;
        latch_.r_unlock();
        return tb_name;
    }
    
    inline int get_table_num() { return table_num_; }
    inline page_id_t get_page_id() const { return page_id_; }

    // ATTENTION no lock
    inline int get_free_space() const {
        return free_space_pointer_ - COM_PG_HEADER_SZ - 4 - tb_id_to_name_.size() * TABLE_RECORD_SZ;
    }

    void new_table(const string_t &table_name);
    bool delete_table(const string_t &table_name);
    bool delete_table(table_id_t table_id);
    bool change_table_name(table_id_t table_id, const string_t &new_name);
private:
    // ATTENTION no lock
    string_t get_table_name(offset_t tb_name_offset, int size);

    BufferPoolManager *bpm_;
    const page_id_t page_id_; // it's his own page id
    const int TABLE_RECORD_SZ = 12;
    const offset_t TABLE_NUM_OFFSET = COM_PG_HEADER_SZ;
    offset_t free_space_pointer_;
    std::unordered_map<table_id_t, string_t> tb_id_to_name_;
    std::unordered_map<string_t, table_id_t> tb_name_to_id_;
    std::atomic<int> table_num_;
    ReaderWriterLatch latch_;
    Page *page_;
    char *data_;
};

} // namespace dawn