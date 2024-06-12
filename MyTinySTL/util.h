#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_
// 这个文件包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等 

#include <cstddef>

#include "type_trails.h"

namespace mastl
{

//move函数
//从T类型的对象中移除引用符号，并将其转换为T类型的左值引用。
//返回类型为T类型的左值引用
template <typename T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept//noexcept是ISO C++17新特性，表示不需要异常处理
{
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

//forward函数
//std::forward会将输入的参数原封不动地传递到下一个函数中，
//这个“原封不动”指的是，如果输入的参数是左值，那么传递给下一个函数的参数的也是左值；
//如果输入的参数是右值，那么传递给下一个函数的参数的也是右值。
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept//&&为万能引用，输入的变量既可以是左值，也可以是右值
{
    static_cast(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

//swap

// 定义一个模板函数swap，用于交换两个变量的值
template <class Tp>
void swap(Tp& lhs, Tp& rhs)
{
    // 创建一个临时变量，存储lhs的值
    auto tmp(mastl::move(lhs));
    // 将rhs的值赋给lhs
    lhs = mastl::move(rhs);
    // 将tmp的值赋给rhs
    rhs = mastl::move(tmp);
}

//交换两个范围[first1, last1)和[first2, first2 + (last1 - first1))中的元素。
template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    for(; first1 != last1; ++first1, ++first2)
        mastl::swap(*first1, *first2);
    return first2;
}

//它接受两个同类型数组a和b作为参数，以及一个大小N作为参数。这个函数的目的是交换这两个数组中的元素
template <class Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N])
{
    // 调用 mastl::swap_range 函数来交换数组 a 和 b 的内容
    // mastl::swap_range 函数接收三个参数：第一个数组的起始迭代器，第一个数组的末尾迭代器，第二个数组的起始迭代器
    // 通过这种方式，可以实现两个数组内容的交换
    mastl::swap_range(a, a + N, b);
}


// --------------------------------------------------------------------------------------
// pair

// 结构体模板 : pair
// 两个模板参数分别表示两个数据的类型
// 用 first 和 second 来分别取出第一个数据和第二个数据
template <class T1, class T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    first_type first;
    second_type second;

    // 构造函数
    // 默认构造函数
    // 构造一个pair对象，其两个数据成员的值都是默认值
    // 模板参数 Other1 和 Other2 分别表示两个数据成员的类型
    template <class Other1 = T1, class Other2 = T2,
    typename = typename std::enable_if<                         //std::enable_if<> 判断条件是否成立，如果成立则执行后面的语句
    std::is_default_constructible<Other1>::value &&             //std::is_default_constructible<> 判断变量是否是默认可构造
    std::is_default_constructible<Other2>::value,void>::type>
    constexpr pair()                                            //constexpr 修饰的函数，在编译阶段就执行
    :first, second
    {
    }

    //implicit constructiable for this type
    //隐式构造
    template <class U1 = T1, class U2 = T2,
    typename std::enable_if<
    std::is_copy_constructible<U1>::value &&
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, T1>::value &&                //std::is_convertible<> 判断变量是否可以转换为指定类型
    std::is_convertible<const U2&, T2>::value, int>::type = 0>
    constexpr pair(const T1& a, const T2& b)
    :first(a), second(b)
    {
    }
    
    // explicit constructible for this type
    //显式构造,要求函数初始化时必须使用括号赋值
    template <class U1 = T1, class U2 = T2>
    typename std::enable_if<
    std::is_copy_constructible<U1>::value &&
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, T1>::value &&
    std::is_convertible<const U2&, T2>::value, pair>::type = 0>
    // 显式构造函数，用于创建pair对象
    explicit constexpr pair(const Ty1& a, const Ty2& b)
    // 初始化first成员变量为a
    : first(a), 
    // 初始化second成员变量为b
    second(b)
    // 构造函数体结束
    {
    }

    //定义一个pair的拷贝构造函数
    pair(const pair& rhs) = default;
    //移动构造函数。它的目的是从一个即将被销毁（或不再需要）的pair对象（这里命名为rhs）创建一个新的pair对象
    pair(pair&& rhs) = default;


    // implicit constructiable for other type
    template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    (!std::is_convertible<Other1&&, Ty1>::value &&
    !std::is_convertible<Other2&&, Ty2>::value), int>::type = 0>
    explicit constexpr pair(Other1&& a, Other2&& b)
    :first(mastl::forward<Other1>(a)),
    second(mastl::forward<Other2>(b))
    {

    }



};


};


#endif