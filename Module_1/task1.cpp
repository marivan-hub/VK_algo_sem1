//Подсчитать кол-во единичных бит в входном числе , стоящих на четных позициях.
//Позиции битов нумеруются с 0.
//Необходимо использование битовых операций.
//Использование арифметических операций запрещено.
#include <iostream>
#include <cassert>


uint invertSelectedBit(uint N, int K) {
    uint mask = 1;
    mask = mask << K;
    return N ^ mask;
}

int main() {
    uint N;
    int K;
    std::cin >> N >> K;

    assert(N >= 0);
    assert(K >= 0);
    assert(K <= 31);

    uint result = invertSelectedBit(N, K);

    std::cout << result << std::endl;

    return 0;
}