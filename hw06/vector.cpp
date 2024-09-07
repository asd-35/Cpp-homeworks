#include "vector.h"
#include "math.h"
#include <iterator>


namespace linalg {
auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}

Vector::Vector(std::size_t n){
    data_.resize(n);
};

Vector::Vector(std::size_t n, float val){
    data_ = std::vector<float>(n,val);
};

Vector::Vector(std::initializer_list<float> list) : data_{list} {}

auto Vector::operator[](int idx) -> float &{
    if(idx<0){idx = data_.size() + idx;}
    return this->data_.at(idx);
};


auto Vector::operator=(float val) -> Vector &{
    for(float &i:this->data_){i = val;}
    return *this;
};

auto Vector::assign(float val) -> void{
    data_ = std::vector<float>(data_.size(),val);
};

auto Vector::assign(Vector v) -> void{
    this->data_ = v.data_;
};

auto Vector::coeff(int idx) -> float &{
    if(idx > this->data_.size() - 1 || 0 > idx) throw std::out_of_range("out of range");
    return this->data_[idx];
};

auto Vector::coeff(int idx) const -> const float &{
    if(idx > this->data_.size() - 1 || 0 > idx) throw std::out_of_range("out of range");
    return this->data_[idx];
};

auto Vector::size() const -> std::size_t{
    return this->data_.size();
};

auto Vector::end() -> iterator {
    return data_.end();
};

auto Vector::begin() -> iterator{
    return data_.begin();
};

auto Vector::begin() const -> const_iterator{
    return data_.begin();
};

auto Vector::end() const -> const_iterator{
    return data_.end();
};

auto Vector::operator[](int idx) const -> const float &{
    if(idx<0){idx = data_.size() + idx;}
  
    return this->data_.at(idx);
};

auto Vector::operator-=(float val) -> Vector &{
    /*
    for (int i = 0; i < this->data_.size(); i++)
    {
        this->data_[i] -= val;
    }*/
    for(float &i:this->data_){i -= val;}

    return *this;
};

auto Vector::operator-=(const Vector &y) -> Vector &{
    //wondering if i can pull this with foreach loop. can i work arund y vector withput idx's ??
    for (int i = 0; i < this->data_.size(); i++)
    {
        this->data_[i] = this->data_[i] - y[i];
    }
   
    return *this;
};

auto Vector::operator/=(float val) -> Vector &{
    for(float &i:this->data_){i /= val;}
    return *this;
};

auto Vector::operator*=(float val) -> Vector &{
    for(float &i:this->data_){i *= val;}
    return *this;
};

auto Vector::operator+=(float val) -> Vector &{
    for(float &i:this->data_){i += val;}
    return *this;
};

auto Vector::operator+=(const Vector &y) -> Vector &{
    if(this->data_.size() != y.size()) throw std::invalid_argument("invalid arg");
    for (int i = 0; i < this->data_.size(); i++)
    {
        this->data_[i] = this->data_[i] + y[i];
    }
    return *this;
};

auto operator+(const Vector &x, const Vector &y) -> Vector{
    Vector vec(x);
    vec += y;
    return vec;
};

auto operator+(const Vector &x, float val) -> Vector{
    Vector vec(x);
    vec += val;
    return vec;
};

auto operator+(float val, const Vector &x) -> Vector{
    Vector vec(x);
    vec += val;
    return vec;
};



auto operator-(const Vector &x, float val) -> Vector{
    Vector vec(x);
    vec -= val;
    return vec;
};

auto operator-(const Vector &x, const Vector &y) -> Vector{
    Vector vec(x);
    vec -= y;
    return vec;
};

auto operator/(const Vector &x, float val) -> Vector{
    Vector vec(x);
    vec /= val;
    return vec;
};

auto operator*(float val, const Vector &x) -> Vector{
    Vector vec(x);
    vec *= val;
    return vec;
};

auto operator*(const Vector &x, float val) -> Vector{
    Vector vec(x);
    vec *= val;
    return vec;
};

auto norm(const Vector &x) -> float{
    float result = 0.0f;
    for(float i:x){result += i*i;}
    return std::sqrt(result);
};

auto min(const Vector &x) -> float{
    if(x.size() == 0) throw std::invalid_argument("invalid arg");
    
    float min_float = x[0];

    for(float i:x){
        min_float = std::min(min_float,i);
    }
    return min_float;
};

auto max(const Vector &x) -> float{
    if(x.size() == 0) throw std::invalid_argument("invalid arg");
    float max_float = x[0];

    for(float i:x){
        max_float = std::max(max_float,i);
    }
    return max_float;
};

auto floor(const Vector &x) -> Vector{
    Vector vec(x);
    for (size_t i = 0; i < vec.size(); i++)
    {
        vec[i] = std::floor(x[i]);
    }
    return vec;
};

auto ceil(const Vector &x) -> Vector{
    Vector vec(x);
    for (size_t i = 0; i < vec.size(); i++)
    {
        vec[i] = std::ceil(x[i]);
    }
    return vec;
};

auto normalize(Vector &x) -> void{
    float numNorm = norm(x);
    for (auto &val : x) {val /= numNorm;}
};
auto normalized(const Vector &x) -> Vector{
    Vector vec(x);
    normalize(vec);
    return vec;
};

auto dot(const Vector &x, const Vector &y) -> float{
    if(x.size() != y.size()) throw std::invalid_argument("invalid arg");
    float result = 0.0f;
    for (size_t i = 0; i < x.size(); i++)
    {
        result += x[i] * y[i];
    }
    return result;
};

auto argmin(const Vector &x) -> std::size_t{
    if(x.size() == 0) throw std::invalid_argument("invalid arg");
    size_t idx = 0;
    for (size_t i = 0; i < x.size(); i++)
    {
        if(x[idx] > x[i]){idx=i;}
    }
    return idx;
};

auto argmax(const Vector &x) -> std::size_t{
    if(x.size() == 0) throw std::invalid_argument("invalid arg");
    size_t idx = 0;
    for (size_t i = 0; i < x.size(); i++)
    {
        if(x[idx] < x[i]){idx=i;}
    }
    return idx;
};

auto non_zeros(const Vector &x) -> std::size_t{
    size_t count = 0;
    for(const float &i:x){
        if(i != 0){count += 1;}
    }
    return count;
};

auto sum(const Vector &x) -> float{
    float result = 0.0f;
    for(const float &i:x){result+=i;}
    return result;
};

auto prod(const Vector &x) -> float{
    float result = 1.0f;
    for(const float &i:x){result*=i;}
    return result;
};

auto operator+(const Vector &x) -> Vector{
    Vector vec(x);
    return vec;
};

auto operator-(const Vector &x) -> Vector{
    Vector vec(x);
    vec *= -1.0f;
    return vec;
};

auto operator-(float val, const Vector &x) -> Vector{
    Vector vec(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        vec[i] = val - x[i];
    }
    
    return vec;
};

}

/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
