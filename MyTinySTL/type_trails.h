#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_

#include <type_traits>



//用于提取类型信息
namespace mastl
{

    //模板类 m_intergral_constant，用于存储一个整数值
    template <class T, T v>
    struct m_intergral_constant
    {
        static constexpr T value = v;//constexpr 表示该函数在编译时被计算
    };

    //模板类 m_bool_constant，用于存储一个布尔值
    template <bool b>
    using m_bool_constant = m_intergral_constant<bool, b>;
    
    //定义两个常量 m_true_type 和 m_false_type，用于表示布尔值
    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;


// type traits

// is_pair

// --- forward declaration begin
    //模板类 pair，用于存储两个值
    template <class T1, class T2>
    struct pair;
// --- forward declaration end

    //模板类 is_pair，用于判断是否为 pair 类型
    template <class T>
    struct is_pair : mastl::m_false_type {};

    
    //如果传入的类型是 pair 类型，则 is_pair 返回 m_true_type，否则返回 m_false_type
    template <class T1, class T2>
    struct is_pair<mastl::pair<T1, T2>> : mastl::m_true_type {};
    
}// namespace mastl
//模板类在编译时会被计算，所以它的性能开销相对较低
#endif