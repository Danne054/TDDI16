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
#include <unordered_map>
using namespace std;
// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.

typedef unordered_set<string> Dictionary;

struct Node {
    bool visited = false;
    string previous { };
};
vector <Node > graph;

static vector<string> follow_parents(unordered_map<string, Node> const& node_vector, const Dictionary &dict, string const& end_node, const string &from, bool const reverse = false) {
    // std::cout << "working" << std::endl;
    Node current_node{ node_vector.at(end_node)};
    vector<string> chain{};

    while (current_node.previous != from){
        if (!reverse)
            chain.insert(chain.begin(), current_node.previous);
        else 
            chain.push_back(current_node.previous);
        current_node = node_vector.at(current_node.previous);
    }
    return chain;
}

// int word_diff(string const& word1, string const& word2) {
//     int diff {};
//     for (size_t i{}; i < word2.size(); ++i){
//         if (word2.at(i) != word1.at(i)) ++diff;
//         if (diff > 1) return diff;
//     }
//     return diff;
// }

vector<string> generate_word(const string &from){
    string word { from };
    vector<string> all_possible_neighbors{ };
    string alphabet {"abcdefghijklmnopqrstuvwxyz"};

    for (size_t j{}; j < from.size(); ++j){
        for (size_t i{from.at(j) +'a'}; i  < alphabet.size() + (from.at(j) +'a'); ++i){
            word.at(j) = alphabet.at(i%alphabet.size());
            all_possible_neighbors.push_back(word);
        }
        word = from;
    }

    return all_possible_neighbors;
}   

vector<string> get_neighbors(const Dictionary &dict, const string &from){
    vector<string> all_possible_neighbors {generate_word(from)};
    
    vector<string> neighbors{};
    for (const string & w : all_possible_neighbors)
        if (dict.count(w) == 1) 
            neighbors.push_back(w);
    
    return neighbors;
}

vector<string> bfs(const Dictionary &dict, string const& from, string const& to) {
    unordered_map<string, Node> graph;

    for (const string& word : dict) {
        graph.emplace(word, Node{false, string{}});
    }
    vector<string> chain { };
    queue<string> q;

    q.push(from);
    while (!q.empty()) {
        string curr = q.front();
        q.pop();
  
        vector<string> neighbors {get_neighbors(dict, curr)};
        if (neighbors.empty()) return chain;
        //titar på grannoderna
        for (size_t i{}; i < neighbors.size(); ++i) {
            
            
            auto it = graph.find(neighbors.at(i));

            if (it != graph.end() && !it->second.visited) {
                graph.at(neighbors.at(i)).previous = curr;
                graph.at(neighbors.at(i)).visited = true;
                q.push(neighbors.at(i));
                if (neighbors.at(i) == to) {
                    chain = follow_parents(graph, dict, neighbors.at(i) , from);
                    chain.push_back(neighbors.at(i));
                    return chain;
                }
            }
        }    
    }
    return chain;
}


vector<string> bfs_longest(const Dictionary &dict, string const& from) {
    // vector <vector<string>> adj{};
    unordered_map<string, Node> graph;

    for (const string& word : dict) {
        graph.emplace(word, Node{false, string{}});
    }

    vector<string> chain{ };
    vector<vector<string>> all_paths{ };
    queue<string> q;
    
    q.push(from);
    bool foud_end { };
    while (!q.empty()) {
        foud_end = true;
        string curr = q.front();
        q.pop();
        
        vector<string> neighbors {get_neighbors(dict, curr)};
        if (neighbors.empty()) return chain;

        for (size_t i{}; i < neighbors.size(); ++i) {
            auto it = graph.find(neighbors.at(i));

            if (it != graph.end() && !it->second.visited) {
                foud_end = false;
                graph.at(neighbors.at(i)).previous = curr;
                graph.at(neighbors.at(i)).visited = true;
                q.push(neighbors.at(i));
            }
        }  

        if (foud_end){
            vector <string> tmp { follow_parents(graph, dict, curr , from, true) };
            tmp.push_back(from);
            all_paths.push_back(tmp);
        }
    }
    // std::cout <<"paths" << all_paths.size() <<std::endl;

    for (size_t i{1}; i < all_paths.size(); ++i )
        if (all_paths.at(i) > chain)
            chain = all_paths.at(i);
    
    return chain;
}




/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */

vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to) {
    vector<string> result;
    // result = bfs(dict,from,  to);
    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word) {
    vector<string> result(1, word);
    // cout << "TODO: Implement me!" << endl;
    
    result = bfs_longest(dict, word);
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