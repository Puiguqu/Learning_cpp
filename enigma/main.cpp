#include <iostream>
#include "enigma.hpp"

// Build & run:
//   g++ -std=gnu++17 -O2 main.cpp -o enigma && ./enigma
// Example input: "HELLOWORLD"  (letters only; spaces/punct ignored)

int main() {
    using namespace enigma;

    // Example: M3 with rotors I-II-III, reflector B.
    // Ring settings AAA (0,0,0), start window "AAA", plugboard pairs AB CD EF.
    Enigma m3(
        ROTOR_I, ROTOR_II, ROTOR_III,
        /*ring L,M,R*/ 0,0,0,
        /*pos  L,M,R*/ 0,0,0,
        /*plugboard*/  {"AB","CD","EF"}
    );

    m3.setPositions("AAA");

    std::string plaintext;
    std::cout << "Enter A-Z text: ";
    std::getline(std::cin, plaintext);

    auto ct = m3.encrypt(plaintext);
    std::cout << "Ciphertext: " << ct << "\n";

    // Demonstrate symmetry: encrypting ciphertext with same settings returns the original (filtered) text.
    Enigma m3_decode(
        ROTOR_I, ROTOR_II, ROTOR_III,
        0,0,0, 0,0,0, {"AB","CD","EF"}
    );
    m3_decode.setPositions("AAA");
    auto pt_roundtrip = m3_decode.encrypt(ct);
    std::cout << "Roundtrip:  " << pt_roundtrip << "\n";

    return 0;
}
