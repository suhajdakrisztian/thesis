if test -f main; then
    rm main
fi

g++-10 main.cpp -o main -std=c++20 && ./main && rm main