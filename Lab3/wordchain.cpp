#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::queue;
#include <algorithm>
using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;
// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.

typedef vector<string> Dictionary;

struct Node {
    vector <string > edges;
    bool visited = false;
    string previous;
};
vector <Node > graph;

// struct Dictionary {
//     vector<string> words;

//     template <typename Begining, typename Ending>
//     Dictionary(Begining const& begin_, Ending const& end_) 
//         : words{begin_, end_} 
//     { }

//     bool operator ==(string const& rhs) const{
//         for (const string &w: words){
//             if (w == rhs) return true;
//         }
//         return false;
//     }
     
//     size_t size() const{
//         return words.size();
//     }

// };
// struct Dictionary {
//     static const int WORD_LEN = 4;
//     static const int ALPHABET = 26;
//     static const int CODES = ALPHABET * ALPHABET * ALPHABET * ALPHABET;

//     vector<string> words;
//     vector<int> table;

//     template <typename Begining, typename Ending>
//     Dictionary(Begining const& begin_, Ending const& end_) 
//         : words{begin_, end_} 
//     { }

//     // Bygger Dictionary från en lista med ord. 'table' skapas med CODES platser, alla satta till -1
//     // ("ordet finns inte") innan vi fyller i de ord som faktiskt finns.
//     explicit Dictionary(const vector<string> &list) : table(CODES, -1) {
//         for (const string &w : list) {
//             int c = encode(w);
//             // ifall ogiltigt ord eller dubblett
//             if (c < 0 || table[c] != -1)
//                 continue;                   
//             table[c] = static_cast<int>(words.size());
//             words.push_back(w);
//         }
//     }
//     // Gör om ett ord till ett tal i bas 26 och -1 om det är oglittigt
//     static int encode(const string &w) {
//         if (w.size() != WORD_LEN) {
//             return -1;
//         }

//         int code = 0;

//         for (char ch : w) {
//             //inga ogiltiga tecken
//             if (ch < 'a' || ch > 'z'){
//                 return -1;
//             }
//             //ganska simmpelt ifall a så blir det 0*26+0 = 0
//             code = code * ALPHABET + (ch - 'a');
//         }
//         return code;
//     }

//     //dena hittade jag för att kunna retunera ens nodnummer likt det vi gjorde på första labben
//     int find(const string &word) const {
//         int c = encode(word);
//         return c < 0 ? -1 : table[c];
//     }
// };

//att göra 1. Implementera en typ av sökning typ "static int abc" för att gå från ord a till ord b  2.Implementera find_shortest och find_longest. 





//något simpelt jag hittade online för att hitta  tillbaka till rotnoden ish
// static vector<string> follow_parents(const Dictionary &dict, const vector<int> &parent, int node) {
//     vector<string> chain;
//     for (; node != -1; node = parent[node])
//         chain.push_back(dict.words[node]);
//     return chain;
// }

static vector<string> follow_parents(vector<Node> const& node_vector, const Dictionary &dict, Node const& end_node, const string &from) {
    Node current_node{end_node};
    vector<string> chain{};

    while (current_node.previous != from){
        chain.insert(chain.begin(), current_node.previous);
        current_node = node_vector.at(distance(dict.begin(), find(dict.begin(), dict.end(), current_node.previous)));
    }
    return chain;
}

// bool bfs(string from , string to) {
//     queue <string > q; q.push(from );
//     bool visited [width ][ height] = false;
//     visited [from.x][ from.y] = true;
//     while (!q.empty ()) {
//         string current = q.front (); q.pop ();
//         for (string x : current . neighbors ()) {
//             if (c == current ) return true;
//             // Om 'x' ej besökt , markera och lägg på kö.
//         }
//     }
//     return false;
// }

int word_diff(string const& word1, string const& word2) {
    int diff {};
    for (size_t i{}; i < word2.size(); ++i){
        if (word2.at(i) != word1.at(i)) ++diff;
        if (diff > 1) return diff;
    }
    return diff;
}

vector<string> get_neighbors(const Dictionary &dict, const string &from){
    vector<string> neighbors{};
    for (size_t i{}; i < dict.size(); ++i){
        if (word_diff (dict.at(i), from) == 1) neighbors.push_back(dict.at(i));
    }

    return neighbors;
}
bool temp_func(const Dictionary &dict, string const& to, int j, queue<string> & q, vector<string> & res, vector <vector<string>> & adj, vector<bool> & visited){
    if (!q.empty()){
        string curr = q.front();
        string tmp = q.front();
        q.pop();

        // visit all the unvisited
        // neighbours of current node
        adj.push_back(get_neighbors(dict, curr));
        
        for (size_t i{}; i < adj.at(j).size(); ++i) {
            auto it = distance(dict.begin(), find(dict.begin(), dict.end(), adj.at(j).at(i)));
            // std::cout << it << std::endl;
            if (!visited.at(it)) {
                std::cout << adj.at(j).at(i) << std::endl; 
                visited[it] = true;
                q.push(adj.at(j).at(i));
                if (adj.at(j).at(i) == to) {
                    res.push_back(adj.at(j).at(i));
                    return true;
                }
            }
        }
        ++j;
        if (temp_func(dict, to, j, q, res, adj, visited))
            res.push_back(tmp);
    }

    return false;
}

vector<string> bfs(const Dictionary &dict, string const& from, string const& to) {
    vector <vector<string>> adj{};
    vector<Node> node_vector(dict.size(), Node{vector <string >{}, false, string{}});
    vector<string> chain;
    queue<string> q;
    
    int src = 0;
    q.push(from);
    int j{};
    
    while (!q.empty()) {
        string curr = q.front();
        string tmp = q.front();

        q.pop();
  
        adj.push_back(get_neighbors(dict, curr));

        for (size_t i{}; i < adj.at(j).size(); ++i) {

            auto it = distance(dict.begin(), find(dict.begin(), dict.end(), adj.at(j).at(i)));

            if (!node_vector.at(it).visited) {

                node_vector.at(it).previous = tmp;
                node_vector.at(it).visited = true;
                q.push(adj.at(j).at(i));
                if (adj.at(j).at(i) == to) {
                    chain = follow_parents(node_vector, dict, node_vector.at(it) , from);
                    chain.push_back(adj.at(j).at(i));
                    return chain;
                }
            }
        }
        ++j;
        
    }
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
    // vector <vector <string>> neighbors {};
    // neighbors.push_back(get_neighbors(dict, from));
    // result = get_neighbors(dict, from);
    // for (const vector <string> & v_s: neighbors){
    result = bfs(dict,from,  to);
    // }
    // int a = dict.find(from);
    // int b = dict.find(to);
    // if (a < 0 || b < 0)
    //     return vector<string>();
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
    std::cout << "done" << std::endl;
    return 0;
}
