#include <iostream>
#include <cmath>
#include "bigint.h"

/// Construtor default.
/// Inicializa com um inteiro de 1 digito, valor 0 (zero).
/// NAO PODE SER MODIFICADO
BigInt::BigInt()
  : neg(false)
  , nDig(1)
  , d(new int8_t[nDig]{0})
{}

//Método privado Clear

void BigInt::Clear()
{
    //cout << "Deletando BigInt:" << this  << " d: " << d<< endl;
    //Libera memória
    if(d!=nullptr) delete[] d;

    //Anula todos os membros
    neg = false;
    nDig = 0;
    d = nullptr;
}

/// Destrutor
BigInt::~BigInt()
{
    Clear();
}


/// Construtor especifico PRIVADO que recebe o sinal e a quantidade de digitos
BigInt::BigInt(bool isNeg, int Size)
 : neg(isNeg)
 , nDig(Size>=1 ? Size : 1)    // Recebe size se for maior ou igual a 1, recebe 1 se for menor que 1
 , d(new int8_t[nDig]{0})       //Iniciado com todas nDig posições igual a 0
{}

/// Construtor por copia.
/// Delega ao construtor especifico privado.
/// NAO PODE SER MODIFICADO.
BigInt::BigInt(const BigInt& B)
  : BigInt(B.isNeg(), B.size())
{
  // Copia os digitos
  for (int i=0; i<size(); ++i) d[i] = B.d[i];
}

/// Atribuicao por copia
const BigInt& BigInt::operator=(const BigInt& other)
{
    //Evita auto-atribuição
    if(this == &other) return *this;

    //Liberando área antiga
    delete[] d;

    //Copia os novos dados
    neg = other.neg;
    nDig = other.nDig>0 ? other.nDig : 1;

    //Aloca nova área de memória
    d = new int8_t[nDig]{0};

    //Copia os elementos
    for(int i = 0; i < nDig; ++i) d[i] = other.d[i];

    return *this;
}



/// Construtor por movimento
BigInt::BigInt(BigInt&& other) noexcept
 : neg(other.neg)
 , nDig(other.nDig>0 ? other.nDig : 1) //
 , d(other.nDig>0 ? other.d : nullptr) //
{
    //Zera o objeto sem nome
    other.neg = false;
    other.nDig = 0;
    other.d = nullptr;
}

/// Atribuicao por movimento
const BigInt& BigInt::operator=(BigInt&& other) noexcept
{
    if(this == &other) return *this;

    //Desalocar memória antiga
    delete[] d;

    //Atribuição dos membros
    neg = other.neg;
    nDig = other.nDig;
    d = other.d;

    //Desatribuição dos membros temporários
    other.neg = false;
    other.nDig = 0;
    other.d = nullptr;

    return *this;
}

//Método de consulta this->operator[]


int BigInt::operator[](int i) const
{
    if(i < 0 || i >= nDig){
        return 0;
    } else {
    return d[i];
    }
}

/*int8_t& BigInt::operator[](int i)
{
    //Usado para passar uma referência.
    static int8_t zero(0);
    if(i < 0 || i >= nDig){
        return zero;
    }
    return d[i];
}*/

/// Construtor especifico a partir de inteiro longo.
/// Tambem conversor de long long int para BigInt.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(long long int N)
  : neg(N<0 ? true : false)
  , nDig(N == 0 ? 1 : (1+int(log10(fabs(N))))) //1 se for igual a zero, caso contrário, N = 1 + parte inteira do log_10 do valor absoluto de N
  , d(new int8_t[nDig])
{
  // Calcula os digitos, usando divisao inteira por 10
  for (int i=0; i<size(); ++i)
  {
    d[i] = abs(N%10); // Modulo do resto da divisao
    N /= 10;          // Divisao inteira
  }
}

/// Conversor de BigInt para long long int
long long int BigInt::toInt() const
{
    long long int val(0);
    for(int i = size()-1; i >= 0; --i){
        val = 10*val + (*this)[i];
        if(val < 0){std::cerr << "Objeto excede o tamanho de 'long long int'"; return 0;}
    }

    if(isNeg()) val = -val;

    return val;
}

/// ******************
/// * FIM DA PARTE 1 *
/// ******************

/// Funcao privada que corrige o numero, caso haja inconsistencias
void BigInt::correct()
{
    int newSize( size() );
    while(newSize > 1 && d[newSize-1] == 0){
        --newSize;
    }

    if(newSize != size()){
        BigInt prov(isNeg(), newSize);

        for(int i = 0; i < newSize; ++i) prov.d[i] = (*this)[i];

        *this = std::move(prov);
    }
    //Corrige zero "negativo"
    if(isZero()) neg = false;
}

/// Construtor especifico a partir de string.
/// Nao eh conversor de string para BigInt.
/// Delega ao construtor default.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(const std::string& S)
  : BigInt() // Valor inicial zero
{
  // Se string vazia, emite erro e permanece com valor inicial zero
  if (S.empty())
  {
    std::cerr << "empty string cannot create a BigInt\n";
    return;
  }

  // Posicao onde comecam os digitos, inicialmente zero
  size_t ini=0;
  // Leva em conta o sinal
  bool IsNeg = false;

  if (S[0]=='+' || S[0]=='-')
  {
    // Se nao tem nenhum digito alem do sinal, emite erro e permanece com valor inicial
    if (S.size()==1)
    {
      std::cerr << "sign-only string cannot create a BigInt\n";
      return;
    }
    IsNeg = (S[0]=='-');
    ++ini;
  }

  // Faz ter sinal (IsNeg) e numero de digitos (tamanho da string - ini) corretos
  /* ACRESCENTAR */
  BigInt prov(IsNeg, S.size() - ini);
  *this = std::move(prov);


  // Calculo dos digitos do BigInt
  for (int i=0; i<size(); ++i)
  {
    char c = S[S.size()-1-i];
    if (!isdigit(c))
    {
      *this = BigInt(); // = 0
      std::cerr << "string with invalid character cannot create a BigInt\n";
      return;
    }
    d[i] = static_cast<int8_t>(c-'0');
  }
  // Corrige eventuais numeros fora da especificacao
  correct();
}

///Fim parte 2

/// Insercao (impressao)
std::ostream &operator<<(std::ostream& O, const BigInt& B)
{
    if(B.neg) O << "-";

    for(int i = B.size() - 1; i >= 0; --i) O << static_cast<int>(B.d[i]);

    return O;
}

/// Extracao (digitacao).
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
std::istream &operator>>(std::istream& I, BigInt& B)
{
  // Valor inicial zero
  B = BigInt(); // = 0

  // Testa a stream de entrada e descarta eventuais separadores iniciais.
  // Em caso de erro, encerra a digitacao.
  std::istream::sentry s(I);
  if (!s) return I;

  // Caractere lido da stream
  int c;

  // Inspeciona o primeiro caractere que serah lido
  c = I.peek();

  // Testa se o primeiro caractere eh um sinal.
  // Se for, consome (elimina do buffer), processa e inspeciona o proximo caractere.
  if (c=='+' || c=='-')
  {
    // Consome
    c = I.get();
    // Atribui o sinal
    B.neg = (c=='-');
    // Obtem o proximo caractere
    c = I.peek();
  }

  // Numero de digitos que foram digitados
  int numDigitos = 0;

  // Testa se eh um caractere valido: digitos 0 a 9
  while (isdigit(c))
  {
    // Consome do buffer
    c = I.get();
    ++numDigitos;

    if (numDigitos>1)
    {
      // Faz o BigInt manter o sinal e passar a ter size()+1 digitos,
      // avancando todos para uma posicao mais significativa aa frente.
      /* ACRESCENTAR */
      BigInt prov(B.isNeg(), B.size()+1);
      for(int i(0); i < B.size(); ++i) prov.d[i+1] = B.d[i];
      B = std::move(prov);
    }

    // Acrescenta o novo digito como sendo o primeiro (o menos significativo)
    B.d[0] = static_cast<int8_t>(c-'0');

    // Inspeciona o proximo caractere que vai ser lido
    c = I.peek();
  }

  // Assinala erro na stream se nenhum digito foi lido
  if (numDigitos==0) I.setstate(std::ios::failbit);

  // Corrige eventuais erros na digitacao
  B.correct();

  // Encerra a digitacao
  return I;
}

///FIM DA PARTE 3

///Operador ==
///Retorna true se um BigInt for igual ao outro
bool BigInt::operator==(const BigInt& B) const
{

    if(neg == B.neg && nDig == B.nDig){
        for(int i = 0; i < size(); ++i)
            {
                if((*this)[i] != B.d[i]) return false;
            }

        return true;
    }

    return false;
}

///Operador não-igual
/// Retorna NOT(igual(?))
bool BigInt::operator!=(const BigInt& B) const
{
    return !(*this==B);
}

///Operador "menor"
///Retorna true se this < outro bigint, false caso contrário

bool BigInt::operator<(const BigInt& B) const
{
    //Sinais diferentes? Negativo sempre menor
    if(isNeg() != B.isNeg()){
        return isNeg();
    }

    //Os dois tem o mesmo sinal
    if(!isNeg())
        {
            //Ambos positivos ou nulos
            if(size() != B.size()) return (size() < B.size());

            //Mesmo número de digitos
            for(int i = size()-1; i >= 0; --i)
            {
                if((*this)[i] != B.d[i]) return (*this)[i] < B.d[i];
            }
        } else
            {
                //Ambos negativos
                if(size() != B.size()) return size() > B.size();

                //Mesmo numero de digitos
                for(int i = size()-1; i >= 0; --i)
                {
                    if((*this)[i] != B.d[i]) return (*this)[i] > B.d[i];
                }
            }

            //Número identicos
            return false;
}

//resto dos operadores são inline
//Em 'bigint.h'

///Fim parte 4

///Método privado increment
void BigInt::increment()
{
    int k(0);

    while(k < size() && d[k] == 9)
    {
        d[k] = 0;
        ++k;
    }

    if(k < size()) ++d[k]; //d[k] = d[k]+1;
    else
    {
        BigInt prov(neg, size()+1);
        prov.d[size()] = 1;

        *this = std::move(prov);
    }
}

///Método privado decrement
void BigInt::decrement()
{
    if(isZero())
    {
        d[0] = 1;
        neg = true;
        return;
    }

    int k(0);
    while(k < size() && d[k] == 0)
    {
        d[k] = 9;
        ++k;
    }

    --d[k]; // ou d[k] = d[k]-1;

    if(d[size()-1] == 0) correct();
}



/// Operador de incremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt& BigInt::operator++()
{
  if (!isNeg()) increment();
  else decrement();
  return *this;
}

/// Operador de decremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt& BigInt::operator--()
{
  if (isNeg()) increment();
  else decrement();
  return *this;
}

BigInt BigInt::operator++(int x)
{
    BigInt copia(*this);
    ++*this;

    return copia;
}

BigInt BigInt::operator--(int x)
{
    BigInt copia(*this);
    --*this;

    return copia;
}

///Fim Parte 5

//Função clássica
BigInt abs(const BigInt& X)
{
    if(X.isNeg()) return -X;

    return X;
}

const BigInt& BigInt::operator+() const
{
    return *this;
}

BigInt BigInt::operator-() const
{
    if(isZero()) return *this;

    BigInt copia(*this);
    copia.neg = !neg;

    return copia;
}

BigInt BigInt::operator+(const BigInt& B) const
{
    if(isNeg() == B.isNeg())
    {
        BigInt C(isNeg(), 1+std::max(size(), B.size()));

        int carry(0);

        for(int i = 0; i < C.size(); ++i)
        {
            C.d[i] = (*this)[i] + B[i] + int8_t(carry);
            if(C.d[i] > 9)
            {
                C.d[i] -= 10;
                carry = 1;
            } else carry = 0;
        }

        C.correct();
        return C;
    } else if(abs(*this) >= abs(B))
    {

        BigInt C(isNeg(), size());
        int borrow(0);


        for(int i = 0; i < C.size(); ++i)
        {
            C.d[i] = (*this)[i] - B[i] - int8_t(borrow);
            if(C.d[i] < 0)
            {
                C.d[i] += 10;
                borrow = 1;
            } else borrow = 0;
        }
        C.correct();
        return C;
    } else // |A| < |B|
    {
        return B + *this;
    }
}

BigInt BigInt::operator-(const BigInt& B) const
{
    return *this + -B;
}

///Fim Parte 6

BigInt BigInt::operator*(const BigInt& B) const
{
    if(isZero() || B.isZero()) return 0; // Ou BigInt(0)

    //Sinal tem valor true se os sinais forem diferentes, sinalizando neg = true
    //Ou false caso contrário, indicando que os sinais são iguais
    bool sinal = (isNeg() != B.isNeg()) ? true : false;

    //O construtor privado BigInt(bool, int) já inicia com todos
    //os valores zerados
    BigInt C(sinal, size() + B.size());

    for(int i(0); i < size(); ++i)
    {
        if((*this)[i] != 0)
        {
            for(int j(0); j < B.size(); ++j)
            {
                if(B[j] != 0)
                    {
                    int k(i+j);
                    C.d[k] += (*this)[i] * B[j];
                    while(C[k] > 9)
                    {
                        int carry = C[k] / 10;
                        C.d[k] = C[k] % 10;
                        ++k;
                        C.d[k] += carry;
                    }
                }
            }
        }
    }

    C.correct();
    return C;
}

BigInt BigInt::operator!() const
{
    if(isNeg())
    {
        std::cerr << "Impossivel tirar fatorial de um numero negativo" << std::endl;
        return 0;
    }

    BigInt N;
    BigInt C(1);

    for(N = 2; N <= *this; ++N)
    {
        C = C * N;
    }

    return C;
}

///Fim Parte 7

BigInt BigInt::operator<<(int n) const
{
    if(n <= 0 || isZero()) return *this;

    BigInt C(isNeg(), size() + n);

    for(int i(n); i < C.size(); ++i)
    {
        C.d[i] = (*this)[i-n];
    }

    return C;
}

BigInt BigInt::operator>>(int n) const
{
    if(n <= 0 || isZero()) return *this;
    if(n >= size()) return 0;

    BigInt C(isNeg(), size() - n);

    for(int i = 0; i < C.size(); ++i) C.d[i] = (*this)[i+n];

    return C;
}

void BigInt::division(const BigInt& D, BigInt& Q, BigInt& R) const
{
    Q = 0;
    if(isZero() || D.isZero())
    {
        if(D.isZero()) std::cerr << "Erro! Divisão por zero!";

        R = 0;
        return;
    }

    BigInt absD(abs(D));
    if(abs(*this) < absD)
    {
        R = *this;
        return;
    }

    R = 0;

    for(int i = size()-1; i >= 0; --i)
    {
        if(!(R.isZero())) R = R<<1;
        R.d[0] = (*this)[i];
        int div = 0;
        while(R >= absD)
        {
            R = R - absD;
            ++div;
        }
        if(!(Q.isZero())) Q = Q << 1;
        Q.d[0] = div;
    }

    Q.neg = (isNeg() != D.isNeg());
    if(!(R.isZero())) R.neg = isNeg();
}

BigInt BigInt::operator/(const BigInt& D) const
{
    BigInt Q;
    BigInt R;

    division(D, Q, R);

    return Q;
}

BigInt BigInt::operator%(const BigInt& D) const
{
    BigInt Q, R;
    division(D, Q, R);

    return R;
}

///Fim parte 8



