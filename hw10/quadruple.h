#pragma once

#include <array>
#include <tuple> 
// TODO: Implement the Quadruple struct, with the required specialisations
// - The general case should store types and have getters for each called `get_first`, `get_second`,
// etc
// - Implement once specialization if all types are ints. Store them in a publicly accessible
// `std::array`, again provide the getters.
// - Implement once specialization if all types are the same. Store them in a publicly accessible
// `std::array`, again provide the getters.
template <typename T, typename U, typename V, typename W> struct Quadruple {
public:
    std::tuple<T,U,V,W> members;

    Quadruple (T t,U u,V v,W w){
        first = t;
        second = u;
        third = v;
        fourth = w;
        members = std::make_tuple(t,u,v,w);
    }

    auto get_first() { return first; }

    auto get_second() { return second; }

    auto get_third() { return third; }

    auto get_fourth() { return fourth; }

private:
    T first;
    U second;
    V third;
    W fourth;
};

template<>
struct Quadruple<int,int,int,int> {
public:
    std::array<int,4> members;

    Quadruple(int t,int u,int v,int w){
        first = t;
        second = u;
        third = v;
        fourth = w;
        members = {t,u,v,w};
    }

    auto get_first() { return first; }

    auto get_second() { return second; }

    auto get_third() { return third; }

    auto get_fourth() { return fourth; }
private:
    int first;
    int second;
    int third;
    int fourth;
};

template<typename T>
struct Quadruple<T,T,T,T> {
public:
    std::array<T,4> members;

    Quadruple(T t,T u,T v,T w){
        first = t;
        second = u;
        third = v;
        fourth = w;
        members = {t,u,v,w};
    }


    auto get_first() { return first; }

    auto get_second() { return second; }

    auto get_third() { return third; }

    auto get_fourth() { return fourth; }
private:
    T first;
    T second;
    T third;
    T fourth;
};