#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::cout;
using std::endl;

// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.

//typedef vector<string> Dictionary;

struct Dictionary {
    static const int WORD_LEN = 4;
    static const int ALPHABET = 26;
    static const int CODES = ALPHABET * ALPHABET * ALPHABET * ALPHABET;

    vector<string> words;
    vector<int> table;

    // Bygger Dictionary från en lista med ord. 'table' skapas med CODES platser, alla satta till -1
    // ("ordet finns inte") innan vi fyller i de ord som faktiskt finns.
    explicit Dictionary(const vector<string> &list) : table(CODES, -1) {
        for (const string &w : list) {
            int c = encode(w);
            // ifall ogiltigt ord eller dubblett
            if (c < 0 || table[c] != -1)
                continue;                   
            table[c] = static_cast<int>(words.size());
            words.push_back(w);
        }
    }
    // Gör om ett ord till ett tal i bas 26 och -1 om det är oglittigt
    static int encode(const string &w) {
        if (w.size() != WORD_LEN) {
            return -1;
        }

        int code = 0;

        for (char ch : w) {
            //inga ogiltiga tecken
            if (ch < 'a' || ch > 'z'){
                return -1;
            }
            //ganska simmpelt ifall a så blir det 0*26+0 = 0
            code = code * ALPHABET + (ch - 'a');
        }
        return code;
    }

    //dena hittade jag för att kunna retunera ens nodnummer likt det vi gjorde på första labben
    int find(const string &word) const {
        int c = encode(word);
        return c < 0 ? -1 : table[c];
    }
};

//att göra 1. Implementera en typ av sökning typ "static int abc" för att gå från ord a till ord b  2.Implementera find_shortest och find_longest. 





//något simpelt jag hittade online för att hitta  tillbaka till rotnoden ish
static vector<string> follow_parents(const Dictionary &dict, const vector<int> &parent, int node) {
    vector<string> chain;
    for (; node != -1; node = parent[node])
        chain.push_back(dict.words[node]);
    return chain;
}

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */
vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to) {
    vector<string> result;
    //cout << "TODO: Implement me!" << endl;
    int a = dict.find(from);
    int b = dict.find(to);
    if (a < 0 || b < 0)
        return vector<string>();
    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word) {
    vector<string> result(1, word);
    cout << "TODO: Implement me!" << endl;
    return result;
}


/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
Dictionary read_dictionary() {
    string line;
    vector<string> result;
    while (std::getline(std::cin, line)) {
        if (line == "#")
            break;

        result.push_back(line);
    }

    return Dictionary(result.begin(), result.end());
}

/**
 * Skriv ut en ordkedja på en rad.
 */
void print_chain(const vector<string> &chain) {
    if (chain.empty())
        return;

    vector<string>::const_iterator i = chain.begin();
    cout << *i;

    for (++i; i != chain.end(); ++i)
        cout << " -> " << *i;

    cout << endl;
}

/**
 * Skriv ut ": X ord" och sedan en ordkedja om det behövs. Om ordkedjan är tom, skriv "ingen lösning".
 */
void print_answer(const vector<string> &chain) {
    if (chain.empty()) {
        cout << "ingen lösning" << endl;
    } else {
        cout << chain.size() << " ord" << endl;
        print_chain(chain);
    }
}

/**
 * Läs in alla frågor. Anropar funktionerna "find_shortest" eller "find_longest" ovan när en fråga hittas.
 */
void read_questions(const Dictionary &dict) {
    string line;
    while (std::getline(std::cin, line)) {
        size_t space = line.find(' ');
        if (space != string::npos) {
            string first = line.substr(0, space);
            string second = line.substr(space + 1);
            vector<string> chain = find_shortest(dict, first, second);

            cout << first << " " << second << ": ";
            print_answer(chain);
        } else {
            vector<string> chain = find_longest(dict, line);

            cout << line << ": ";
            print_answer(chain);
        }
    }
}

int main() {
    Dictionary dict = read_dictionary();
    read_questions(dict);

    return 0;
}
