#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <string>
using namespace std;

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
    virtual void displayAll() const = 0;
};

template <typename T, typename KeyType>
IRepository<T, KeyType>::~IRepository() {}

#endif // IREPOSITORY_H
