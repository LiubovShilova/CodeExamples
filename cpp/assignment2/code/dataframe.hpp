#ifndef ASSIGNMENT2_DATAFRAME_HPP
#define ASSIGNMENT2_DATAFRAME_HPP

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <list>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "col_type.hpp"
#include "utils.hpp"

// This is needed to be able to deduce the first type of a passed lambda object
template<typename Ret, typename Arg, typename... Rest>
Arg first_argument_helper(Ret(*) (Arg, Rest...));

template<typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_helper(Ret(F::*) (Arg, Rest...));

template<typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_helper(Ret(F::*) (Arg, Rest...) const);

template <typename F>
decltype(first_argument_helper(&F::operator())) first_argument_helper(F);

template <typename T>
using first_argument_t = decltype(first_argument_helper(std::declval<T>()));


class Dataframe {
    std::list<std::vector<ColType>> data;
    std::vector<std::string> colnames;
    std::vector<std::string> rownames;

    template<typename T>
    T& get_type_(ColType& ct){
        auto& val = dynamic_cast<DataType<typename std::remove_reference_t<T>>&>(ct.get());
        return val.get_value();
    }

    template<typename T>
    const T& get_type_(const ColType& ct) const {
        const auto& val = dynamic_cast<const DataType<typename std::remove_reference_t<T>>&>(ct.get());
        return val.get_value();
    }

public:
    class Invalid{};
    Dataframe();

    Dataframe(const Dataframe &other);
    Dataframe(Dataframe &&other);
    Dataframe &operator=(const Dataframe &other);
    Dataframe &operator=(Dataframe &&other);

    template<typename T>
    void add_column(const std::vector<T>& col_values);

    void remove_column(const size_t i);

    void remove_column(const std::string& c);
    
    void set_colnames(const std::vector<std::string>& cnames);

    void set_rownames(const std::vector<std::string>& rnames);

    bool has_rownames() const;

    bool has_colnames() const;

    std::size_t nrows() const;

    std::size_t ncols() const;

    const std::vector<std::string>& get_rownames() const;

    const std::vector<std::string>& get_colnames() const;

    template <typename T>
    T get(const size_t i, const size_t j) const;

    template <typename T>
    T get(const std::string& r, const std::string& c) const;
    
    template <typename T>
    T& get(const size_t i, const size_t j);

    template <typename T>
    T& get(const std::string& r, const std::string& c);

    friend std::ostream& operator<<(std::ostream& os, const Dataframe& df);

    void swap_columns(const size_t i, const size_t j);

    void swap_columns(const std::string& i, const std::string& j);

    void order_by(const std::string& c);

    template<typename Fun>
    void apply(const size_t i, Fun l);

    template<typename Fun>
    void apply(const std::string& c, Fun l);

    bool operator==(const Dataframe& other) const;

    bool operator!=(const Dataframe& other) const;

    void clear_rownames();

    void clear_colnames();

};

#endif //ASSIGNMENT2_DATAFRAME_HPP
