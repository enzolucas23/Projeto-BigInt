#ifndef _BIGINT_H_
#define _BIGINT_H_
#include <iostream>
#include <cstdint>
#include <string>

class BigInt {
private:
    ///Membros privados da classe
    bool neg;
    int nDig;
    int8_t* d;

    ///Construtor privado
    BigInt(bool isNeg, int Size); //Inicia com isneg e size com todos os digitos zerados

    ///Métodos privados
    void Clear(); //Anula todos os valores

    void correct(); //Corrije o bigint

    void increment(); //Incrementa o bigint por 1

    void decrement(); //Decrementa o bigint por 1

public:
    ///Construtores/Atribuições
    BigInt(); //Construtor default

    ~BigInt(); //Destrutor

    BigInt(const BigInt& B); //Construtor por cópia

    const BigInt& operator=(const BigInt& other); //Operador por copia

    BigInt(BigInt&& other) noexcept; //Construtor por movimento

    const BigInt& operator=(BigInt&& other) noexcept; //Operador por movimento

    BigInt(long long int); //Contrutor a partir de LLI

    explicit BigInt(const std::string&); //Construtor a partir de uma string

    ///Métodos de consulta/Operadores

    inline bool isNeg() const { return neg;} //retorna true se for negativo
    inline int size() const { return nDig;} //retorna o tamanho do bigint
    bool isZero() const { return (nDig==1 && d[0] == 0); } //retorna  true se for 0
    int operator[](int i) const; //retorna o digito da posição i
    //int8_t& operator[](int i);
    friend std::ostream &operator<<(std::ostream&, const BigInt&); //Esses caras
    friend std::istream &operator>>(std::istream&, BigInt&);
    ///Operadores de igualdade
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    inline bool operator>(const BigInt& B) const {return (B < *this);}
    inline bool operator>=(const BigInt& B) const {return !(*this < B);}
    inline bool operator<=(const BigInt& B) const {return !(B < *this);}
    BigInt& operator++(); //operador ++ usa o metodo incremento
    BigInt& operator--(); //operador -- usa o metodo decremento
    BigInt operator++(int);
    BigInt operator--(int);
    const BigInt& operator+() const; //Retorna uma referencia a si
    BigInt operator-() const; //Retorna o negativo de si
    BigInt operator+(const BigInt&) const; //Operador de soma
    BigInt operator-(const BigInt&) const; //Operador de subtração
    BigInt operator*(const BigInt&) const; //Operador de multiplicação
    BigInt operator!() const; //Operador fatorial
    BigInt operator<<(int) const;
    BigInt operator>>(int) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator%(const BigInt&) const;

    ///Métodos específicos
    long long int toInt() const; //Retorna um long long int, pode ter problemas de precisão

    void division(const BigInt&, BigInt&, BigInt&) const;
};

///Funções clássicas

BigInt abs(const BigInt&); //Retorna o valor absoluto de um bigint

#endif
