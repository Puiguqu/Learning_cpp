#pragma once
#include <array>
#include <string>
#include <stdexcept>
#include <cctype>
#include <vector>
#include <utility>

// --- Helpers ---------------------------------------------------------------
namespace enigma {
    inline int mod26(int x) { x %= 26; return x < 0 ? x + 26 : x; }
    inline int ch2i(char c) { return std::toupper(static_cast<unsigned char>(c)) - 'A'; }
    inline char i2ch(int i) { return static_cast<char>('A' + mod26(i)); }
    inline bool isAZ(char c){ c=std::toupper(static_cast<unsigned char>(c)); return c>='A'&&c<='Z'; }

    // Convert "EKMFLG..." to mapping array 0..25 -> 0..25
    inline std::array<int,26> wiringFromString(const std::string& s) {
        if (s.size()!=26) throw std::invalid_argument("Wiring must be 26 chars");
        std::array<int,26> m{};
        std::array<bool,26> seen{};
        for (int i=0;i<26;++i){
            if(!isAZ(s[i])) throw std::invalid_argument("Wiring must be A-Z");
            int to = ch2i(s[i]);
            if (seen[to]) throw std::invalid_argument("Duplicate letter in wiring");
            seen[to]=true;
            m[i]=to;
        }
        return m;
    }

    inline std::array<int,26> invert(const std::array<int,26>& f){
        std::array<int,26> inv{};
        for(int i=0;i<26;++i) inv[f[i]]=i;
        return inv;
    }

    // Plugboard builder from pairs like {"AB","CD","EF"}
    inline std::array<int,26> plugboardFromPairs(const std::vector<std::string>& pairs){
        std::array<int,26> p{}; for(int i=0;i<26;++i) p[i]=i;
        for(const auto& pr : pairs){
            if(pr.size()!=2 || !isAZ(pr[0]) || !isAZ(pr[1]))
                throw std::invalid_argument("Plugboard pairs must be like \"AB\"");
            int a=ch2i(pr[0]), b=ch2i(pr[1]);
            if(p[a]!=a || p[b]!=b) throw std::invalid_argument("Letter already paired");
            p[a]=b; p[b]=a;
        }
        return p;
    }

    struct RotorSpec {
        std::array<int,26> wiring;   // forward map at ring=0,pos=0
        std::array<int,26> inv;      // backward map
        std::vector<int> notches;    // positions (0=A) where the *next* step occurs when rotor is at notch
    };

    // Canonical M3 rotors + Reflector B
    inline const RotorSpec ROTOR_I   { wiringFromString("EKMFLGDQVZNTOWYHXUSPAIBRCJ"), {}, { ch2i('Q') } };
    inline const RotorSpec ROTOR_II  { wiringFromString("AJDKSIRUXBLHWTMCQGZNPYFVOE"), {}, { ch2i('E') } };
    inline const RotorSpec ROTOR_III { wiringFromString("BDFHJLCPRTXVZNYEIWGAKMUSQO"), {}, { ch2i('V') } };
    inline const std::array<int,26> REFLECTOR_B = wiringFromString("YRUHQSLDPXNGOKMIEBFZCWVJAT");

    // Fill inverses at static init (constexpr-friendly workaround)
    inline RotorSpec withInv(RotorSpec s){ s.inv = invert(s.wiring); return s; }

    // A rotor instance (stateful)
    class Rotor {
    public:
        Rotor(RotorSpec s, int ring /*0-25*/, int pos /*0-25*/)
        : spec_(withInv(s)), ring_(mod26(ring)), pos_(mod26(pos)) {}

        // Step rotor by 1
        void step(){ pos_ = mod26(pos_ + 1); }

        // Is the rotor currently at a notch position?
        bool atNotch() const {
            for(int n: spec_.notches) if (pos_==n) return true;
            return false;
        }

        // Pass signal right->left (keyboard to reflector)
        int forward(int x) const {
            // account for ring setting (Ringstellung) and rotor position
            int shift = mod26(x + pos_ - ring_);
            int y = spec_.wiring[shift];
            return mod26(y - pos_ + ring_);
        }

        // Pass signal left->right (reflector to lampboard)
        int backward(int x) const {
            int shift = mod26(x + pos_ - ring_);
            int y = spec_.inv[shift];
            return mod26(y - pos_ + ring_);
        }

        void setPos(int p){ pos_ = mod26(p); }
        int  pos() const { return pos_; }
        int  ring() const{ return ring_; }

    private:
        RotorSpec spec_;
        int ring_; // 0..25 (A=0)
        int pos_;  // 0..25 (A=0)
    };

    class Enigma {
    public:
        // rotors: left, middle, right; ring & pos are 0..25 (A=0)
        Enigma(const RotorSpec& L, const RotorSpec& M, const RotorSpec& R,
               int ringL, int ringM, int ringR,
               int posL,  int posM,  int posR,
               const std::vector<std::string>& plugPairs = {})
        : L_(L, ringL, posL), M_(M, ringM, posM), R_(R, ringR, posR)
        {
            plug_ = plugboardFromPairs(plugPairs);
        }

        // Set starting window letters like "KCH"
        void setPositions(const std::string& window){ 
            if(window.size()!=3) throw std::invalid_argument("Positions must be 3 letters");
            L_.setPos(ch2i(window[0])); M_.setPos(ch2i(window[1])); R_.setPos(ch2i(window[2]));
        }

        std::string encrypt(const std::string& text){
            std::string out; out.reserve(text.size());
            for(char c : text){
                if(!isAZ(c)){ if(std::isspace(static_cast<unsigned char>(c))) continue; else continue; } // strip non-AZ
                out.push_back(encChar(std::toupper(static_cast<unsigned char>(c))));
            }
            return out;
        }

    private:
        // Implements proper stepping with double-step:
        // 1) Middle steps when RIGHT is at its notch on the keypress (since right will step).
        // 2) Double-step: if M at notch, L and M both step; R always steps.
        void stepRotors(){
            bool rightWillStep = true;              // R always steps
            bool middleAtNotch = M_.atNotch();      // double-step trigger
            bool rightAtNotch  = R_.atNotch();      // causes middle to step

            // Order matters: double-step means M steps if right-at-notch OR M-at-notch
            if (middleAtNotch) L_.step();
            if (rightAtNotch || middleAtNotch) M_.step();
            if (rightWillStep) R_.step();
        }

        char encChar(char C){
            stepRotors();

            int x = ch2i(C);

            // Plugboard
            x = plug_[x];

            // Rotors forward (R -> M -> L)
            x = R_.forward(x);
            x = M_.forward(x);
            x = L_.forward(x);

            // Reflect
            x = REFLECTOR_B[x];

            // Rotors backward (L -> M -> R)
            x = L_.backward(x);
            x = M_.backward(x);
            x = R_.backward(x);

            // Plugboard
            x = plug_[x];

            return i2ch(x);
        }

        Rotor L_, M_, R_;
        std::array<int,26> plug_{};
    };
}
