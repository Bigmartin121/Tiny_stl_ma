#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

// 这个头文件用于迭代器设计，包含了一些模板结构体与全局函数，

#include <cstddef>

#include "type_traits.h"

namespace mastl
{
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


    //iteerator 模板
    template <class Category, class T, class Distance = ptrdiff_t,
    class Pointer = T*, class Reference = T&>
    struct iterator
    {
        // 迭代器类型
        typedef Category                             iterator_category;
        // 元素的类型
        typedef T                                    value_type;
        // 指向元素的指针
        typedef Pointer                              pointer;
        // 指向元素的引用
        typedef Reference                            reference;
        // 迭代器与起始元素的距离
        typedef Distance                             difference_type;
    };

    // iterator traits
    //用于检查一个类型T是否具有iterator_category类型的成员
    template <class T>
    struct has_iterator_cat
    {
    private:    
        // 定义一个结构体two，用于判断U::iterator_category是否为0
        struct two{char a; char b;};
        // 定义一个模板函数test，用于判断U::iterator_category是否为0
        template <class U> static two test(...);
        template <class U> static char test(typename U::iterator_category* = 0);
        // 定义一个公共静态成员变量value，用于存储判断结果
    public:
        // 定义一个静态成员函数value，用于存储判断结果
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    // 定义一个模板类iterator_traits_impl，模板参数为迭代器和布尔值
    template <class Iterator, bool>
    struct iterator_traits_impl {};

    // 定义一个模板类iterator_traits_impl的模板特化，特化参数为迭代器和布尔值为true
    template <class Iterator>
    struct iterator_traits_impl<Iterator, true>
    {
        // 定义一个类型，为迭代器的迭代器类别
        typedef typename Iterator::iterator_category iterator_category;
        // 定义一个类型，为迭代器的值类型
        typedef typename Iterator::value_type        value_type;
        // 定义一个类型，为迭代器的指针类型
        typedef typename Iterator::pointer           pointer;
        // 定义一个类型，为迭代器的引用类型
        typedef typename Iterator::reference         reference;
        // 定义一个类型，为迭代器的差值类型
        typedef typename Iterator::difference_type   difference_type;
    };

    // 定义一个模板类iterator_traits_helper，该类有一个模板参数I，一个布尔参数b
    template <class Iterator, bool b>
    struct iterator_traits_helper {};

    template <class Iterator>
    // 定义一个结构体，用于处理输入输出迭代器
    struct iterator_traits_helper<Iterator, true>
    : public iterator_traits_impl<Iterator,
    // 如果迭代器的类别是输入迭代器或者输出迭代器，则这个结构体继承自输入输出迭代器
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
    {
    };

}

#endif // MYTINYSTL_ITERATOR_H_