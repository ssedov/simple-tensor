//
// Created by Sergey Sedov on 14.10.2018.
//

#ifndef SIMPLETENSOR_TENSOR_H
#define SIMPLETENSOR_TENSOR_H

#include <vector>
#include <memory>

template<class T>
class Tensor {

private:
    std::vector<int> dimensions;
    std::vector<int> dimension_sizes;
    std::vector<std::shared_ptr<T>> values;

    /*
     * Populates new dimension sizes (precalculated counts for each dimension)
     * Returns total size
     */
    int set_dimensions(std::vector<int> dims);

public :
    /*
     * Empty constructor initialized empty tensor
     */
    Tensor() = default;

    /*
     * Constructor with dimensions sizes
     * data is initialized by default T ()
     */
    Tensor(std::vector<int> const &dimSizes);

    /*
     * Copy - constructor , should copy entire tensor data
     */
    Tensor(const Tensor &rhs);

    /*
     * Move - constructor
     */
    Tensor(Tensor &&rhs);

    /*
     * Copy - assignment , should copy entire tensor data
     */
    Tensor &operator=(const Tensor &rhs);

    /*
     * Move - assignment
     */
    Tensor &operator=(Tensor &&rhs);

    /*
     * Returns dimensions sizes of tensor
     */
    std::vector<int> dim() const;

    /*
     * Returns sub - tensor with indices in first dimension [ low , high )
     * !!! All changes applied further to sub - tensor would be applied to this tensor as well and vice versa as well,
     * in other words they would share their content !!!
     */
    Tensor operator()(int low, int high);

    /*
     * Returns sub - tensor with index in first dimension pos
     * !!! All changes applied further to sub - tensor would be
     * applied to this tensor as well and vice versa as well,
     * in other words they would share their content !!!
     */
    Tensor operator()(int pos);

    /*
     * Returns value indexed by { pos [0] , pos [1] , pos [2]... pos [dim - 1]}
     * pos.size () should be equal to dim
     */
    T &operator[](std::vector<int> const &pos);

    /*
     *
     */
    Tensor<T> reshape(std::vector<int> new_dims);
};

template<class T>
Tensor<T>::Tensor(std::vector<int> const &dimSizes) {
    int size = set_dimensions(dimSizes);
    values.reserve(size);
    for (int i = 0; i < size; ++i) {
        values.push_back(std::make_shared<T>());
    }
}

template<class T>
T &Tensor<T>::operator[](std::vector<int> const &pos) {
    if (dimensions.empty())
        throw std::logic_error("Cannot query an empty tensor");
    int offset = 0;
    if (pos.size() != dimensions.size())
        throw std::range_error("Incorrect dimensions");

    for (int i = 0; i < dimensions.size(); ++i) {
        if (pos[i] >= dimensions[i] || pos[i] < 0)
            throw std::range_error("Dimension index out of bounds");
        offset += pos[i] * dimension_sizes[i];
    }
    if (offset >= values.size() || offset < 0)
        throw std::range_error("Index out of bounds");
    return *(values[offset]);
}

template<class T>
std::vector<int> Tensor<T>::dim() const {
    return dimensions;
}

template<class T>
Tensor<T>::Tensor(const Tensor &rhs) : dimensions(rhs.dimensions), dimension_sizes(rhs.dimension_sizes) {
    values.reserve(rhs.values.size());
    for (auto x:rhs.values)
        values.push_back(std::make_shared<T>(*x));
}

template<class T>
Tensor<T>::Tensor(Tensor &&rhs) :
        values(std::move(rhs.values)),
        dimensions(std::move(rhs.dimensions)),
        dimension_sizes(std::move(rhs.dimension_sizes)) {
}

template<class T>
Tensor<T> &Tensor<T>::operator=(Tensor const &rhs) {
    dimensions = std::move(rhs.dimensions);
    dimension_sizes = std::move(rhs.dimension_sizes);
    values = std::move(rhs.values);
    return *this;
}

template<class T>
Tensor<T> &Tensor<T>::operator=(Tensor &&rhs) {
    dimensions = std::move(rhs.dimensions);
    dimension_sizes = std::move(rhs.dimension_sizes);
    values = std::move(rhs.values);
    return *this;
}

template<class T>
Tensor<T> Tensor<T>::operator()(int pos) {
    if (dimensions.empty())
        throw std::logic_error("Cannot query an empty tensor");
    if (pos >= dimensions[0] || pos < 0)
        throw std::range_error("Index out of bounds");
    if (dimensions.size() == 1)
        return Tensor<T>(); // new one will be empty

    std::vector<int> new_dim = dimensions;
    new_dim.erase(new_dim.begin());

    Tensor<T> x;
    int n = x.set_dimensions(new_dim);
    x.values.reserve(n);
    for (int i = 0; i < n; i++)
        x.values.push_back(values[i + pos * dimension_sizes[0]]);
    return x;
}

template<class T>
Tensor<T> Tensor<T>::operator()(int low, int high) {
    if (dimensions.empty())
        throw std::logic_error("Cannot query an empty tensor");
    if (low >= high || low < 0 || high >= dimensions[0])
        throw std::range_error("Index out of bounds");


    std::vector<int> new_dim = dimensions;
    new_dim[0] = high - low;
    Tensor<T> x;
    int n = x.set_dimensions(new_dim);
    for (int i = 0; i < n; i++)
        x.values.push_back(values[i + low * dimension_sizes[0]]);
    return x;
}

template<class T>
Tensor<T> Tensor<T>::reshape(std::vector<int> new_dims) {
    if (new_dims.empty()) {
        if (dimensions.empty())
            return Tensor<T>();
        else
            throw std::range_error("Cannot reshape a non-empty tensor to empty");
    }
    int new_size = 1;
    for (auto it:new_dims)
        new_size *= it;
    if (new_size != values.size())
        throw std::range_error("New dimensions are invalid");

    Tensor<T> x;
    x.set_dimensions(new_dims);
    x.values = values;
    return x;
}

template<class T>
int Tensor<T>::set_dimensions(std::vector<int> dims) {
    dimensions = dims;
    dimension_sizes.clear();
    int size = 1;
    for (auto it = dimensions.crbegin(); it != dimensions.crend(); it++) {
        dimension_sizes.insert(dimension_sizes.begin(), size);
        size *= *it;
    }
    return size;
}

#endif //SIMPLETENSOR_TENSOR_H
