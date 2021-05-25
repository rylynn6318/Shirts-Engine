#pragma once
namespace se 
{
    enum class Key 
    {
        NONE,
        ESC,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        MOUSE_1,
        MOUSE_2,
        MOUSE_3,
        MOUSE_4,
        MOUSE_5,

        // for iter
        FIRST = static_cast<int>(NONE),
        LAST = static_cast<int>(Z)
    };

    auto operator++(Key& x)->Key;

    auto operator*(Key k)->Key;

    auto begin(Key)->Key;

    auto end(Key)->Key;
}