#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define BF_BUFFER_MIN     "0"
#define BF_BUFFER_MAX "32768"

static const char* BF_HEAD =  "#include <stdint.h>\n"
                              "#include <stdio.h>\n"
                              "\n"
                              "#define BUFFER_MIN " BF_BUFFER_MIN "\n"
                              "#define BUFFER_MAX " BF_BUFFER_MAX "\n"
                              "\n"
                              "int main(void)\n"
                              "{\n"
                              "    static uint8_t buffer[BUFFER_MAX];\n"
                              "    static uint32_t index = 0;\n";
static const char* BF_IPTR =  "    if (index < BUFFER_MAX)\n"
                              "        ++index;\n"
                              "    else\n"
                              "        index = 0;\n";
static const char* BF_DPTR =  "    if (index > BUFFER_MIN)\n"
                              "        --index;\n"
                              "    else\n"
                              "        index = BUFFER_MAX - 1;\n";
static const char* BF_IVAL =  "    ++buffer[index];\n";
static const char* BF_DVAL =  "    --buffer[index];\n";
static const char* BF_PUTC =  "    putchar(buffer[index]);\n";
static const char* BF_GETC =  "    buffer[index] = getchar();\n";
static const char* BF_LOOP1 = "L$a:\n"
                              "    if (buffer[index] == 0)\n"
                              "        goto L$b;\n";
static const char* BF_LOOP2 = "L$b:\n"
                              "    if (buffer[index] != 0)\n"
                              "        goto L$a;\n";
static const char* BF_TAIL =  "    return 0;\n"
                              "}\n";

static string replace_all(const string& a, const string& b, string s)
{
    size_t pos = 0;
    while ((pos = s.find(a)) != string::npos)
        s.replace(pos, a.size(), b);
    return s;
}

static string chars2string(vector <char> chars)
{
    stringstream sstream;
    for (char c : chars)
        sstream << c;
    return sstream.str();
}

int main()
{
    int c, label = 1, nest = 0;
    cout << BF_HEAD;
    while ((c = cin.get())) {
        if (cin.eof())
            break;
        switch (c) {
        case '>': cout << BF_IPTR; break;
        case '<': cout << BF_DPTR; break;
        case '+': cout << BF_IVAL; break;
        case '-': cout << BF_DVAL; break;
        case '.': cout << BF_PUTC; break;
        case ',': cout << BF_GETC; break;
        case '[':
            -cout << replace_all("$", chars2string({ char(label + '0'),
                                            char(nest + '0') }), BF_LOOP1);
            ++nest;
            break;
        case ']':
            --nest;
            cout << replace_all("$", chars2string({ char(label + '0'),
                                            char(nest + '0') }), BF_LOOP2);
            if (!(nest))
                ++label;
            break;
        default:
            break;
        }
    }
    cout << BF_TAIL << endl;
    return 0;
}