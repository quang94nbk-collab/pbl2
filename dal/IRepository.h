#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include "../DSA/DoublyLinkedList.h"
#include <cstdint>
#include <string>
using namespace std;

// Chuyen chuoi UTF-8 (bao gom tieng Viet co dau) ve chu thuong de tim kiem khong phan biet hoa/thuong
inline string utf8ToLower(const string& s) {
    string out;
    out.reserve(s.size());
    size_t i = 0;
    while (i < s.size()) {
        unsigned char b0 = static_cast<unsigned char>(s[i]);
        uint32_t cp = 0;
        size_t len = 1;

        if (b0 < 0x80) {
            cp = b0;
            len = 1;
        } else if ((b0 & 0xE0) == 0xC0 && i + 1 < s.size()) {
            cp = ((b0 & 0x1F) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3F);
            len = 2;
        } else if ((b0 & 0xF0) == 0xE0 && i + 2 < s.size()) {
            cp = ((b0 & 0x0F) << 12) |
                 ((static_cast<unsigned char>(s[i + 1]) & 0x3F) << 6) |
                 (static_cast<unsigned char>(s[i + 2]) & 0x3F);
            len = 3;
        } else if ((b0 & 0xF8) == 0xF0 && i + 3 < s.size()) {
            cp = ((b0 & 0x07) << 18) |
                 ((static_cast<unsigned char>(s[i + 1]) & 0x3F) << 12) |
                 ((static_cast<unsigned char>(s[i + 2]) & 0x3F) << 6) |
                 (static_cast<unsigned char>(s[i + 3]) & 0x3F);
            len = 4;
        } else {
            out.push_back(s[i]);
            ++i;
            continue;
        }

        // Map uppercase codepoints (ASCII + Vietnamese Latin) to lowercase
        if (cp >= 0x0041 && cp <= 0x005A) {
            cp += 0x20;
        } else if ((cp >= 0x00C0 && cp <= 0x00D6) || (cp >= 0x00D8 && cp <= 0x00DE)) {
            cp += 0x20;
        } else if (cp == 0x0102 || cp == 0x0110 || cp == 0x0128 || cp == 0x0168 ||
                   cp == 0x01A0 || cp == 0x01AF) {
            cp += 1;
        } else if (cp >= 0x1EA0 && cp <= 0x1EF8 && (cp % 2 == 0)) {
            cp += 1;
        }

        if (cp < 0x80) {
            out.push_back(static_cast<char>(cp));
        } else if (cp < 0x800) {
            out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else if (cp < 0x10000) {
            out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }

        i += len;
    }
    return out;
}

// ============================================================
//  Interface chung cho tat ca cac Repository (DAL base)
//  Dinh nghia cac phep CRUD co ban ma moi repository phai co
// ============================================================
template <typename T, typename KeyType = string>
class IRepository {
public:
    virtual ~IRepository();

    // Them moi phan tu, tra ve false neu da ton tai hoac du lieu khong hop le
    virtual bool add(const T& entity) = 0;

    // Xoa phan tu theo khoa chinh, tra ve false neu khong tim thay
    virtual bool remove(const KeyType& key) = 0;

    // Tim phan tu theo khoa chinh, tra ve nullptr neu khong co
    virtual T* findById(const KeyType& key) = 0;

    // Hien thi tat ca phan tu ra console
    // (da chuyen sang Presentation; DAL chi tra ve danh sach du lieu)
    virtual const DoubleLinkedList<T>& getList() const = 0;
};

template <typename T, typename KeyType>
IRepository<T, KeyType>::~IRepository() {}

#endif // IREPOSITORY_H
