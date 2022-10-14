// even uint64_t will overflow with longer strings and higher bases, be careful
// 
// 320153390        base 52
// 1721843294784702 base 56
// 179515209226024  base 42

#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>

#define out std::cout
#define in std::cin
#define nl '\n'
#define fo(v,s,e) for(v = s; v < e; ++v)
#define sleep(m) std::this_thread::sleep_for(std::chrono::milliseconds(loop(gen) * m));
#define fakework    fo(i, 0, loop(gen))                                                                 \
                    {                                                                                   \
                    msg = "---<CONVERTING>---";                                                         \
                    fo(j, 0, msgnum(gen))                                                               \
                        msg[msgidx(gen)] = std::to_string(msgnum(gen))[0];                              \
                    out << '\r' << msg;                                                                 \
                    std::this_thread::sleep_for(std::chrono::milliseconds(loop(gen)));                  \
                    }                                                                                   \
                                                                                                        \
                    sleep(25);                                                                          \

inline char to_glyph(uint32_t num)
{
    if (num < 10)
        return num + 48;
    else if (num < 36)
        return num + 55;
    else if (num < 62)
        return num + 61;
    else
        return '+';
}

inline uint16_t to_num(char glyph)
{
    if (glyph > 47 && glyph < 58)
        return glyph - 48;
    else if (glyph > 64 && glyph < 91)
        return glyph - 55;
    else if (glyph > 96 && glyph < 123)
        return glyph - 61;
    else
        return 62;
}

int main()
{
    uint64_t input;
    double overflow;
    uint16_t base, i, j;

    bool enc = false;
    
    std::string conv, msg;

    std::default_random_engine gen;
    std::uniform_int_distribution<int> loop(23, 38);
    std::uniform_int_distribution<int> msgidx(4, 13);
    std::uniform_int_distribution<int> msgnum(0, 9);

    while (true)
    {
        conv.clear();

        if (!enc)
        {
            out << "\nWELCOME. TO THE DECODER. ENTER DECIMAL 0 TO SWITCH MODE.\n";

            out << "\nENTER. A DECIMAL VALUE:\n";
            in >> input;

            if (input == 0)
            {
                enc = true;
                continue;
            }

            out << nl;

            out << "ENTER. A BASE. 0-63:\n"; // only went up to 63 because base64 has its own rules for letters and i didnt wann bother lol
            in >> base;
            out << nl;

            fakework

            out << "\r---<CONVERTING>---  DONE.\n\nYOUR NUMBER. IN BASE " << base << ". IS: ";

            if (base == 0) {}
            else if (base == 1)
                fo(i, 0, input)
                    out << '0';
            else
                while (input > 0)
                {
                    conv += to_glyph(input % base);
                    input /= base;
                }

            std::reverse(conv.begin(), conv.end());

            out << conv << nl;
            sleep(75);
        }
        else
        {
            out << "\nWELCOME. TO THE ENCODER. ENTER DECIMAL 0 TO SWITCH MODE.\n";

            out << "\nENTER. A NUMBER/MESSAGE TO ENCODE. 0-9 A-Z a-z +:\n";
            in >> conv;

            if (conv == "0")
            {
                enc = false;
                continue;
            }

            out << nl;

            input = 0;
            fo(i, 0, conv.length())
                if (conv[i] > input) input = conv[i];   // find lowest base that will still give us all the characters we need
            out << "ENTER. ITS BASE. " << (to_num((char)input)+1) << "-63:\n";
            in >> base;
            out << nl;

            fakework

            out << "\r---<CONVERTING>---  DONE.\n\nYOUR NUMBER/MESSAGE. IN BASE 10. IS: ";

            if (base == 0) {}
            else if (base == 1)
                out << conv.length() << nl;
            else
            {
                input = 0;
                overflow = 0.;
                fo(i, 0, conv.length())
                {
                    if (overflow < ULLONG_MAX)
                        overflow += to_num(conv[i]) * pow(base, conv.length() - i - 1);
                    else
                    {   // i was gonna make input a double but that brought up all kinds of issues and like, no, so i just did this instead lol
                        out << "\n!! OVERFLOW DETECTED, try shorter string/lower base !!\n"; 
                        break;
                    }
                        
                    input += to_num(conv[i]) * pow(base, conv.length() - i - 1);
                }

                out << input << nl;
            }
            
            sleep(75);
        }
    }
}