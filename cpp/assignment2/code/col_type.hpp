#ifndef ASSIGNMENT2_COLTYPE_HPP
#define ASSIGNMENT2_COLTYPE_HPP
#include <memory>
#include <iostream>

class GenericDataType {
public:
    virtual ~GenericDataType() = default;
    virtual std::unique_ptr<GenericDataType> clone() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const GenericDataType& v);
    virtual bool operator==(const GenericDataType& other) const = 0;
    virtual bool operator<(const GenericDataType& other) const = 0;
protected:
    virtual void print(std::ostream& os) const = 0;
};

inline std::ostream& operator<< (std::ostream& os, const GenericDataType& v) {
    v.print(os);
    return os;
}

template <typename T>
class DataType : public GenericDataType {
    T value;

public:
    explicit DataType(T const & val) : value(val) {}
    std::unique_ptr<GenericDataType> clone() const override; // TO IMPLEMENT
    const T& get_value() const { return value; }
    T& get_value() { return value; }
    explicit operator T() const { return value; }
    bool operator==(const GenericDataType& other) const override {
        const auto* dt = dynamic_cast<const DataType<typename std::remove_reference_t<T>>*>(&other);
        if(dt == nullptr) {
            return false;
        }
        return this->get_value() == dt->get_value();
    }
    bool operator<(const GenericDataType& other) const override {
        const auto* dt = dynamic_cast<const DataType<typename std::remove_reference_t<T>>*>(&other);
        if(dt == nullptr) {
            return false;
        }
        return this->get_value() < dt->get_value();
    }
protected:
    void print(std::ostream& os) const override {
        os << this->get_value();
    }
};


class ColType {
    std::unique_ptr<GenericDataType> container;

public:
    template <typename ValueType>
    explicit ColType(const ValueType& value); // TO IMPLEMENT
    ColType(const ColType& other); // TO IMPLEMENT
    ColType& operator=(const ColType& other); // TO IMPLEMENT
    GenericDataType& get() {
        return *container;
    }
    const GenericDataType& get() const {
        return *container;
    }
    bool operator==(const ColType& other) const; // TO IMPLEMENT
    bool operator<(const ColType& other) const; // TO IMPLEMENT
};

#endif //ASSIGNMENT2_COLTYPE_HPP
