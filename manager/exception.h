#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>
using namespace std;

// ============================================================
//  He phan cap ngoai le cua ung dung (Application Exception Hierarchy)
//
//  AppException                   <- Goc, bat moi loi ung dung
//    +-- NotFoundException        <- Khong tim thay thuc the
//    +-- DuplicateException       <- Du lieu bi trung lap
//    +-- ValidationException      <- Du lieu dau vao khong hop le
//    +-- BusinessRuleException    <- Vi pham rang buoc nghiep vu
//         +-- TimeConflictException   <- Trung lich hoc
//         +-- ClassFullException      <- Lop hoc phan da het cho
//         +-- WrongSemesterException  <- Sai hoc ky dang ky
//         +-- NotEligibleException    <- SV khong du dieu kien dang ky
// ============================================================

// ---- Base ----

class AppException : public runtime_error {
public:
    explicit AppException(const string& msg) : runtime_error(msg) {}
};

// ---- Loi truy xuat du lieu ----

class NotFoundException : public AppException {
public:
    explicit NotFoundException(const string& msg) : AppException(msg) {}
};

class DuplicateException : public AppException {
public:
    explicit DuplicateException(const string& msg) : AppException(msg) {}
};

// ---- Loi du lieu dau vao ----

class ValidationException : public AppException {
public:
    explicit ValidationException(const string& msg) : AppException(msg) {}
};

// ---- Loi rang buoc nghiep vu ----

class BusinessRuleException : public AppException {
public:
    explicit BusinessRuleException(const string& msg) : AppException(msg) {}
};

class TimeConflictException : public BusinessRuleException {
public:
    explicit TimeConflictException(const string& msg) : BusinessRuleException(msg) {}
};

class ClassFullException : public BusinessRuleException {
public:
    explicit ClassFullException(const string& msg) : BusinessRuleException(msg) {}
};

class WrongSemesterException : public BusinessRuleException {
public:
    explicit WrongSemesterException(const string& msg) : BusinessRuleException(msg) {}
};

class NotEligibleException : public BusinessRuleException {
public:
    explicit NotEligibleException(const string& msg) : BusinessRuleException(msg) {}
};

#endif // EXCEPTION_H
