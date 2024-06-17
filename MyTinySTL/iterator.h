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

    //提取、总结出迭代器所具有的独特性质和特点
    template <class Iterator>
    struct iterator_traits
    : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

    //指针对原生指针的一种特殊化的版本
    template <class T>
    struct iterator_traits<T*>
    {
        //定义迭代器的类别为随机访问迭代器
        typedef random_access_iterator_tag          iterator_category;
        //定义值类型为T
        typedef T                                   value_type;
        //定义指针类型为T*
        typedef T*                                  pointer;
        //定义引用类型为T&
        typedef T&                                  reference;
        //定义差值类型为ptrdiff_t
        typedef ptrdiff_t                           difference_type;
    };
        
    //定义一个结构体，该结构体模板参数为T，T*
    template <class T>
    struct iterator_traits<const T*>
    {
        //定义迭代器的类别为随机访问迭代器
        typedef random_access_iterator_tag          iterator_category;
        //定义值类型为T
        typedef T                                   value_type;
        //定义指针类型为const T*
        typedef const T*                            pointer;
        //定义引用类型为const T&
        typedef const T&                            reference;
        //定义差值类型为ptrdiff_t
        typedef ptrdiff_t                           difference_type;
    };

    //定义一个模板函数，该函数模板参数为T，U，bool
    template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of
    : public m_bool_constant<std::is_convertible<
    //定义一个模板参数，该参数为T的迭代器类别，U的迭代器类别
    typename iterator_traits<T>::iterator_category, U>::value>
    {
    };


    // 模板特化，判断T类型是否具有U类型的迭代器标签
    template <class T, class U>
    struct has_iterator_cat_of<T, U, false> : m_false_type {};

    // 判断迭代器类型是否为输入迭代器
    template <class Iter>
    struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag>::type {};

    // 判断迭代器类型是否为输出迭代器
    template <class Iter>
    struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag>::type {};

    // 判断迭代器类型是否为正向迭代器
    template <class Iter>
    struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag>::type {};

    // 判断迭代器类型是否为双向迭代器
    template <class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag>::type {};

    // 判断迭代器类型是否为随机访问迭代器
    template <class Iter> 
    struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag>::type {};

    // 判断迭代器类型是否为迭代器
    template <class Iterator>
    struct is_iterator :
    public m_bool_constant<is_input_iterator<Iterator>::value ||
        is_output_iterator<Iterator>::value>
    {
    };

    template <class Iterator>
    // 获取迭代器的类型
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(Iterator)
    {
        // 获取迭代器的类型
        typedef typename iterator_traites<Iterator>::iterator_category Category;
        // 返回迭代器的类型
        return Categorty();
    }

    // 萃取某个迭代器的 distance_type
    template <class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&)
    {
    // 返回一个指向整型的指针，该整数表示迭代器的距离类型
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 萃取某个迭代器的 value_type
    template <class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&)
    {
    // 返回一个指向整数的指针，该整数表示迭代器的值类型
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }




}

#endif // MYTINYSTL_ITERATOR_H_