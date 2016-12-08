// P - большое простое целое число
// G - большое целое число такое, что G < P
// X - секретный ключ, случайное целое число. 1 < X < (P-1)
// Y - открытый ключ, Y = G^X mod P
//
// m - сообщение
// M - хэш сообщения, 1 < M < (P-1)
// K - ключ сессии, случайное число такое, что K и (P-1) -взаимно простые, 1 < K < (P-1)
//
// S - цифровая подпись, состоящая из пары чисел a и b
// a = G ^ K mod P
// b = (M - X * a) * K^-1 mod (P-1)
//
// Тройка чисел (m, a, b) передается получателю
//
// Получатель проверяет истинность выражения
// (Y^a * a^b) mod P = G^M mod P
// Если выражение истинно, значит отправителем сообщения m был именно обладатель секретного ключа X, из которого был получен ключ Y

#include <iostream>
using namespace std;

// Нужно для нахождения обратного к К числа.
long long gcd(long long a, long long b, long long & x, long long & y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    long long x1, y1;
    long long d = gcd (b%a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

// Находит обратное к К число в кольце вычетов (P-1) с помощью расширенного алгоритма Евклида
long long find_inverse(long long P, long long K) {
    long long a = K,
              b = P - 1,
              x, y;

    gcd(a, b, x, y);
    return x;
}


struct Signature {
    long long a,
              b;

    Signature() {}
    Signature(long long _a, long long _b): a(_a), b(_b) {}
};


long long pow_mod(long long x, long long y, long long m) {
    // x^y mod m
    long long res = 1;

    while (y != 0) {
        res *= x;
        res %= m;

        y--;
    }
    return res;
}


long long get_mod() {
    int P;
    cin >> P;
    return P;
}

long long get_rand_numb() {
    int G;
    cin >> G;
    return G;
}

long long get_secret_key() {
    long long X;
    cin >> X;
    return X;
}

long long generate_public_key(long long P, long long G, long long X) {
    return pow_mod(G, X, P);
}


long long get_message() {
    long long m;
    cin >> m;
    return m;
}

long long generate_hash(long long m) {
    return m;
}

long long get_session_key() {
    long long K;
    cin >> K;
    return K;
}


Signature generate_signature(long long P, long long G, long long K, long long M, long long X) {
    // K_ это K^-1 - обратно к К число
    long long K_ = find_inverse(P, K); 

    long long a = pow_mod(G, K, P);
    long long b = (M - X * a) * K_ % (P - 1);

    // модуль всегда должен быть положительным
    if (b < 0)
        b += P - 1;

    return Signature(a, b);
}

bool check_signed_message(long long P, long long G, long long m, Signature S, long long Y) {
    long long M = generate_hash(m);

    long long a = (pow_mod(Y, S.a, P) * pow_mod(S.a, S.b, P)) % P;
    long long b = pow_mod(G, M, P);

    return a == b;
}


void task_generate_public_key() {
    cout << "Write mod number P\n";
    long long P = get_mod();

    cout << "Write random number G. G < P\n";
    long long G = get_rand_numb();

    cout << "Write secret key X. 1 < X < (P - 1)\n";
    long long X = get_secret_key();

    long long Y = generate_public_key(P, G, X);
    cout << "For mod " << P << endl
         << "random number " << G << endl
         << "secret key " << X << endl
         << "Public key Y is " << Y << endl;
}

void task_generate_signature() {
    cout << "Write mod number P\n";
    long long P = get_mod();

    cout << "Write random number G. G < P\n";
    long long G = get_rand_numb();

    cout << "Write session key K. 1 < K < (P - 1), K and P-1 are relatively prime\n";
    long long K = get_session_key();

    cout << "Write secret key X. 1 < X < (P - 1)\n";
    long long X = get_secret_key();

    cout << "Write message M\n";
    long long m = get_message();

    long long M = generate_hash(m);

    Signature S = generate_signature(P, G, K, M, X);
    cout << "For mod " << P << endl
         << "random number " << G << endl
         << "secret key " << X << endl
         << "session key " << K << endl
         << "message " << m << endl
         << "signature (a, b) is " << S.a << " " << S.b << endl;
}

void task_check_signature() {
    cout << "Write mod number P\n";
    long long P = get_mod();

    cout << "Write random number G. G < P\n";
    long long G = get_rand_numb();

    cout << "Write public key Y\n";
    long long Y;
    cin >> Y;

    cout << "Write message M\n";
    long long m;
    cin >> m;

    cout << "Write signature (a, b)\n";
    Signature S;
    cin >> S.a >> S.b;

    bool check = check_signed_message(P, G, m, S, Y);
    if (check)
        cout << "For message " << m << " signature " << S.a << " " << S.b << " is confirmed\n";
    else
        cout << "For message " << m << " signature " << S.a << " " << S.b << " is NOT confirmed\n";
}


int main() {
    int act = 1;

    while (act != 0) {
        cout << "Avialable actions are:\n"
             << "1. Generate public key\n"
             << "2. Generate signature\n"
             << "3. Check signature\n"
             << "0. Exit\n\n"
             << "Choose: ";

        cin >> act;
        cout << endl;

        if (act == 1)
            task_generate_public_key();
        else if (act == 2)
            task_generate_signature();
        else if (act == 3)
            task_check_signature();
        else if (act == 0)
            continue;
        else
            cout << "error\n";

        cout << endl;
    }

    return 0;
}
