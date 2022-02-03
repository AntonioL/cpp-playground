#include <type_traits>
#include <iostream>

using namespace std;

template<int... I>
struct Polynomial;

template<typename P>
constexpr int Degree;

template<int... I>
constexpr int Degree<Polynomial<I...>> = sizeof...(I) - 1;

template<int ...A>
struct Sequence {
    static constexpr int N = sizeof...(A);
};

template<int C, int... sequence>
struct reverse_sequence
{
    using type = typename reverse_sequence<C-1, sequence..., C>::type;
};

template<int... I>
struct reverse_sequence <0, I...>
{
    using type = Sequence<I..., 0>;
};

template<int X, typename P>
constexpr int Evaluate;

template<int X, typename C, typename D>
constexpr int EvaluateHelper;

template <int I, int C>
constexpr int pow = C * pow<I-1, C>;

template<int C>
constexpr int pow<0, C> = 1;

template<int X, int ...C, int ...D>
constexpr int EvaluateHelper<
                X,
                Polynomial<C...>,
                Sequence<D...>
                > = 
    ( (pow<D, X> * C) + ... );

template<int X, int ...Y>
constexpr int Evaluate<X, Polynomial<Y...>> =
    EvaluateHelper<X, Polynomial<Y...>, typename reverse_sequence<Degree<Polynomial<Y...>>>::type>;


int main(void) {
    static_assert ( Degree<Polynomial<0>> == 0 );
    static_assert( Degree<Polynomial<30, 10>> == 1 );

    static_assert ( std::is_same_v<reverse_sequence<1>::type, Sequence<1, 0>> );
    static_assert ( std::is_same_v<reverse_sequence<3>::type, Sequence<3, 2, 1, 0>> );

    static_assert ( Evaluate<2, Polynomial<3>> == 3 );
    static_assert ( Evaluate<1, Polynomial<10, 1>> == 11);
    static_assert ( Evaluate<3, Polynomial<10, 30, 3>> == 183 );
}

