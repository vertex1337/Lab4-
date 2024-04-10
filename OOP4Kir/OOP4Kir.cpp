#ifndef USE_STD_VECTOR
#include <iostream>
#include <cstdlib> // Для использования функции rand()
#include <bitset>

template <typename T>
class Vector {
public:
    // Конструкторы
    Vector() : m_data(nullptr), m_size(0) {}
    Vector(size_t size) : m_data(new T[size]), m_size(size) {}
    Vector(const Vector& other) {
        m_size = other.m_size;
        m_data = new T[m_size];
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }

    // Деструктор
    ~Vector() { delete[] m_data; }

    // Доступ к элементам
    T& operator[](size_t index) { return m_data[index]; }
    const T& operator[](size_t index) const { return m_data[index]; }

    // Размер
    size_t size() const { return m_size; }

    // Добавление элемента
    void push_back(const T& value) {
        T* new_data = new T[m_size + 1];
        std::copy(m_data, m_data + m_size, new_data);
        new_data[m_size++] = value;
        delete[] m_data;
        m_data = new_data;
    }

    void reserve(size_t new_size) {
        if (new_size > m_size) {
            T* new_data = new T[new_size];
            std::copy(m_data, m_data + m_size, new_data);
            delete[] m_data;
            m_data = new_data;
            m_size = new_size;
        }
    }

private:
    T* m_data;
    size_t m_size;
};

// Bitwise AND function object
struct BitwiseAnd {
    template<typename T>
    T operator()(const T& a, const T& b) const {
        return a & b;
    }
};

// Bitwise OR function object
struct BitwiseOr {
    template<typename T>
    T operator()(const T& a, const T& b) const {
        return a | b;
    }
};

// Bitwise XOR function object
struct BitwiseXor {
    template<typename T>
    T operator()(const T& a, const T& b) const {
        return a ^ b;
    }
};

// Bitwise NOT function object
struct BitwiseNot {
    template<typename T>
    T operator()(const T& a) const {
        return ~a;
    }
};

// Функция для заполнения вектора случайными элементами
template<typename T>
void fillVectorRandom(Vector<T>& vec, int size, int minVal, int maxVal) {
    for (int i = 0; i < size; ++i) {
        T element = minVal + rand() % (maxVal - minVal + 1);
        vec.push_back(element);
    }
}

// Функция для вывода вектора в битовом представлении
template<typename T>
void printVectorInBits(const Vector<T>& vec) {
    std::cout << "Vector elements in bits: \n";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::bitset<sizeof(T) * 8> bitsetElement(vec[i]);
        std::string bitString = bitsetElement.to_string();
        // Проверка:
        if (bitString.size() != sizeof(T) * 8) {
            std::cerr << "Error: Bit string size is incorrect for element " << i << std::endl;
            return;
        }
        // Вывод:
        for (size_t j = 0; j < bitString.size(); j += 8) {
            std::cout << bitString.substr(j, 8) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Функция для вывода вектора
template<typename T>
void printVector(const Vector<T>& vec) {
    std::cout << "Vector elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (sizeof(T) == sizeof(char)) {
            // Вывод символа
            std::cout << static_cast<char>(vec[i]) << ' ';
        }
        else {
            // Вывод int
            std::cout << static_cast<int>(vec[i]) << ' ';
        }
    }
    std::cout << std::endl;
}



// Функция для применения битовых операций к векторам
template<typename T, typename Op>
Vector<T> applyBitwiseOperation(const Vector<T>& vec1, const Vector<T>& vec2, Op op) {
    Vector<T> result;
    if (vec1.size() != vec2.size()) {
        std::cerr << "Vectors are of different sizes.\n";
        return result;
    }
    result.reserve(vec1.size());
    for (size_t i = 0; i < vec1.size(); ++i) {
        result.push_back(op(vec1[i], vec2[i]));
    }
    return result;
}

int main() {

    std::cout << "NOT USING #include <vector>\n";
    Vector<char> charVector1, charVector2;
    Vector<int> intVector1, intVector2;

    // Заполнение векторов случайными элементами
    std::cout << "Enter size for the char vectors:\n";
    int size;
    std::cin >> size;
    fillVectorRandom(charVector1, size, 0, 127); // Заполнение для типа char
    fillVectorRandom(charVector2, size, 0, 127); // Заполнение для типа char
    // Вывод векторов char
    std::cout << "Char vector 1:\n";
    printVector(charVector1);
    printVectorInBits(charVector1);
    std::cout << "Char vector 2:\n";
    printVector(charVector2);
    printVectorInBits(charVector2);

    std::cout << "Enter size for the int vectors:\n";
    std::cin >> size;
    fillVectorRandom(intVector1, size, 0, 100); // Примерные значения для типа int
    fillVectorRandom(intVector2, size, 0, 100); // Примерные значения для типа int
    // Вывод векторов int
    std::cout << "Int vector 1:\n";
    printVector(intVector1);
    printVectorInBits(intVector1);
    std::cout << "Int vector 2:\n";
    printVector(intVector2);
    printVectorInBits(intVector2);

    // Применение битовых операций к векторам
    auto charAndResult = applyBitwiseOperation(charVector1, charVector2, BitwiseAnd());
    auto charOrResult = applyBitwiseOperation(charVector1, charVector2, BitwiseOr());
    auto charXorResult = applyBitwiseOperation(charVector1, charVector2, BitwiseXor());

    auto intAndResult = applyBitwiseOperation(intVector1, intVector2, BitwiseAnd());
    auto intOrResult = applyBitwiseOperation(intVector1, intVector2, BitwiseOr());
    auto intXorResult = applyBitwiseOperation(intVector1, intVector2, BitwiseXor());

    // Вывод результатов битовых операций
    std::cout << "\nResults of bitwise operations for char vectors:\n";
    std::cout << "\AND operation:\n";
    printVector(charAndResult);
    printVectorInBits(charAndResult);
    std::cout << "\OR operation:\n";
    printVector(charOrResult);
    printVectorInBits(charOrResult);
    std::cout << "\XOR operation:\n";
    printVector(charXorResult);
    printVectorInBits(charXorResult);

    std::cout << "\nResults of bitwise operations for int vectors:\n";
    std::cout << "\AND operation:\n";
    printVector(intAndResult);
    printVectorInBits(intAndResult);
    std::cout << "\OR operation:\n";
    printVector(intOrResult);
    printVectorInBits(intOrResult);
    std::cout << "\XOR operation:\n";
    printVector(intXorResult);
    printVectorInBits(intXorResult);

    return 0;
};
#endif

#ifdef USE_STD_VECTOR

#include <iostream>
#include <cstdlib> // Для использования функции rand()
#include <bitset>
#include <vector>


// Bitwise AND function object
struct BitwiseAnd {
    template<typename T>
    T operator()(const T& a, const T& b) const {
        return a & b;
    }
};

// Bitwise OR function object
struct BitwiseOr {
    template<typename T>
    T operator()(const T& a, const T& b) const {
        return a | b;
    }
};

// Bitwise XOR function object
struct BitwiseXor {
    template<typename T>
    T operator()(const T& a, const T& b) const {
        return a ^ b;
    }
};

// Bitwise NOT function object
struct BitwiseNot {
    template<typename T>
    T operator()(const T& a) const {
        return ~a;
    }
};

// Функция для заполнения вектора случайными элементами
template<typename T>
void fillVectorRandom(std::vector<T>& vec, int size, int minVal, int maxVal) {
    for (int i = 0; i < size; ++i) {
        T element = minVal + rand() % (maxVal - minVal + 1);
        vec.push_back(element);
    }
}

// Функция для вывода вектора в битовом представлении
template<typename T>
void printVectorInBits(const std::vector<T>& vec) {
    std::cout << "Vector elements in bits: \n";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::bitset<sizeof(T) * 8> bitsetElement(vec[i]);
        std::string bitString = bitsetElement.to_string();
        // Проверка:
        if (bitString.size() != sizeof(T) * 8) {
            std::cerr << "Error: Bit string size is incorrect for element " << i << std::endl;
            return;
        }
        // Вывод:
        for (size_t j = 0; j < bitString.size(); j += 8) {
            std::cout << bitString.substr(j, 8) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Функция для вывода вектора
template<typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "Vector elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (sizeof(T) == sizeof(char)) {
            // Вывод символа
            std::cout << static_cast<char>(vec[i]) << ' ';
        }
        else {
            // Вывод int
            std::cout << static_cast<int>(vec[i]) << ' ';
        }
    }
    std::cout << std::endl;
}



// Функция для применения битовых операций к векторам
template<typename T, typename Op>
std::vector<T> applyBitwiseOperation(const std::vector<T>& vec1, const std::vector<T>& vec2, Op op) {
    std::vector<T> result;
    if (vec1.size() != vec2.size()) {
        std::cerr << "Vectors are of different sizes.\n";
        return result;
    }
    result.reserve(vec1.size());
    for (size_t i = 0; i < vec1.size(); ++i) {
        result.push_back(op(vec1[i], vec2[i]));
    }
    return result;
}

int main() {
    std::cout << "USING #include <vector>\n";
    std::vector<char> charVector1, charVector2;
    std::vector<int> intVector1, intVector2;

    // Заполнение векторов случайными элементами
    std::cout << "Enter size for the char vectors:\n";
    int size;
    std::cin >> size;
    fillVectorRandom(charVector1, size, 0, 127); // Заполнение для типа char
    fillVectorRandom(charVector2, size, 0, 127); // Заполнение для типа char
    // Вывод векторов char
    std::cout << "Char vector 1:\n";
    printVector(charVector1);
    printVectorInBits(charVector1);
    std::cout << "Char vector 2:\n";
    printVector(charVector2);
    printVectorInBits(charVector2);

    std::cout << "Enter size for the int vectors:\n";
    std::cin >> size;
    fillVectorRandom(intVector1, size, 0, 100); // Примерные значения для типа int
    fillVectorRandom(intVector2, size, 0, 100); // Примерные значения для типа int
    // Вывод векторов int
    std::cout << "Int vector 1:\n";
    printVector(intVector1);
    printVectorInBits(intVector1);
    std::cout << "Int vector 2:\n";
    printVector(intVector2);
    printVectorInBits(intVector2);

    // Применение битовых операций к векторам
    auto charAndResult = applyBitwiseOperation(charVector1, charVector2, BitwiseAnd());
    auto charOrResult = applyBitwiseOperation(charVector1, charVector2, BitwiseOr());
    auto charXorResult = applyBitwiseOperation(charVector1, charVector2, BitwiseXor());

    auto intAndResult = applyBitwiseOperation(intVector1, intVector2, BitwiseAnd());
    auto intOrResult = applyBitwiseOperation(intVector1, intVector2, BitwiseOr());
    auto intXorResult = applyBitwiseOperation(intVector1, intVector2, BitwiseXor());

    // Вывод результатов битовых операций
    std::cout << "\nResults of bitwise operations for char vectors:\n";
    std::cout << "\AND operation:\n";
    printVector(charAndResult);
    printVectorInBits(charAndResult);
    std::cout << "\OR operation:\n";
    printVector(charOrResult);
    printVectorInBits(charOrResult);
    std::cout << "\XOR operation:\n";
    printVector(charXorResult);
    printVectorInBits(charXorResult);

    std::cout << "\nResults of bitwise operations for int vectors:\n";
    std::cout << "\AND operation:\n";
    printVector(intAndResult);
    printVectorInBits(intAndResult);
    std::cout << "\OR operation:\n";
    printVector(intOrResult);
    printVectorInBits(intOrResult);
    std::cout << "\XOR operation:\n";
    printVector(intXorResult);
    printVectorInBits(intXorResult);

    return 0;
};

#endif
