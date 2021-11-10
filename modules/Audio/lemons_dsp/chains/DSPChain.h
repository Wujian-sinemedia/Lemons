#pragma once

namespace lemons::dsp
{

template<typename... Nodes>
class DspChain
{
public:
    
    template <int Index>       auto& get()       noexcept { return std::get<Index> (nodes); }
    
    template <int Index> const auto& get() const noexcept { return std::get<Index> (nodes); }
    
    void process()
    {
        forEachInTuple ([&] (auto& proc, size_t index)
        {
            
        }, nodes);
    }
    
private:
    template <typename Fn, typename Tuple, size_t... Ix>
    constexpr void forEachInTuple (Fn&& fn, Tuple&& tuple, std::index_sequence<Ix...>)
    noexcept (noexcept (std::initializer_list<int> { (fn (std::get<Ix> (tuple), Ix), 0)... }))
    {
        (void) std::initializer_list<int> { ((void) fn (std::get<Ix> (tuple), Ix), 0)... };
    }
    
    template <typename T>
    using TupleIndexSequence = std::make_index_sequence<std::tuple_size<std::remove_cv_t<std::remove_reference_t<T>>>::value>;
    
    template <typename Fn, typename Tuple>
    constexpr void forEachInTuple (Fn&& fn, Tuple&& tuple)
    noexcept (noexcept (forEachInTuple (std::forward<Fn> (fn), std::forward<Tuple> (tuple), TupleIndexSequence<Tuple>{})))
    {
        forEachInTuple (std::forward<Fn> (fn), std::forward<Tuple> (tuple), TupleIndexSequence<Tuple>{});
    }
    
    std::tuple<Nodes...> nodes;
};

}
