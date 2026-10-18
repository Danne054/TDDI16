/*
 * fast < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "point.h"
#include "window.h"

using namespace std;

// bool operator!= (Point const& lhs, Point const& rhs){
//     return lhs.x != rhs.x && lhs.y != rhs.y;
// }

bool in_line(vector<Point> const& Points, Point const& compare){
    for (Point const& p : Points)
        if( p.x == compare.x && p.y == compare.y) return true;
    return false;
}

int partition(vector<Point>& vec, int low, int high, Point const& origin) {

    // Selecting the last element as the pivot
    double pivot = origin.slopeTo(vec[high]);

    // Index of the element smaller than the pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {

        // If the current element is smaller than
        // or equal to the pivot
        if (origin.slopeTo(vec[j]) <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Place the pivot in its correct position
    swap(vec[i + 1], vec[high]);

    // Return the partition index
    return i + 1;
}

void quickSort(vector<Point>& vec, int low, int high, Point const& origin) {

    // Base case
    if (low < high) {

        // Partition the array
        int pi = partition(vec, low, high, origin);

        // Sort elements before the pivot
        quickSort(vec, low, pi - 1, origin);

        // Sort elements after the pivot
        quickSort(vec, pi + 1, high, origin);
    }
}
//points_on_line.find(temp_key)->second, slope_sorted.at(i)
bool check_slope_point_vector(const vector<Point> & points, const vector<Point> & points_line, Point const& compare_point, int const o, double const tolerance){
    for (size_t i{1}; i < points_line.size(); ++i){
        if ( !(sameSlope(points.at(o).slopeTo(points_line.at(i)), points.at(o).slopeTo(compare_point), tolerance)) ){
            return false;
        } 
        // else if (i == 3 )
        //     return false;
    }

    return true;
}
    
void find_line(int const N, vector<Point> const& points, double const tolerance,  WindowPtr const& window){
    vector<Point> used_points{}; 
    if (points.empty()) return;
    for (int o{0}; o < N; ++o){
        vector<Point> slope_sorted{points};
        
        quickSort(slope_sorted, 0, slope_sorted.size()-1, points.at(o));
        
        unordered_map<double, vector<Point>> points_on_line{};
        double temp_key{ };
        if (!slope_sorted.empty()){
            temp_key = (points.at(o)).slopeTo(slope_sorted.at(0));
            points_on_line.emplace(temp_key, vector<Point>{slope_sorted.at(o), slope_sorted.at(0)});
        }    
        
        for (size_t i{1}; i < slope_sorted.size(); ++i){
            //if (sameSlope(points.at(o).slopeTo(slope_sorted.at(i-1)), points.at(o).slopeTo(slope_sorted.at(i) ), tolerance)){
            if (check_slope_point_vector(points, points_on_line.find(temp_key)->second, slope_sorted.at(i), o, tolerance)){
                vector <Point> tmp_points_vector{ points_on_line.find(temp_key)->second };
                tmp_points_vector.push_back(slope_sorted.at(i));
                points_on_line.erase(temp_key);
                points_on_line.emplace(temp_key, tmp_points_vector);
            }
            else{
                //kanske erase gammal om den är under 4 i storlek
                temp_key = points.at(o).slopeTo(slope_sorted.at(i));
                points_on_line.emplace(temp_key, vector<Point>{points.at(o), slope_sorted.at(i)});
            }
        }

        for (const std::pair<double, vector<Point>> P : points_on_line)
            if(P.second.size() >= 4){
                window->draw_line(P.second);

            }
    }
}


int main(int argc, const char* argv[]) {
    WindowPtr window = create_window(argc, argv);
    if (!window)
        return 1;

    // The array of points
    vector<Point> points;

    // Read tolerance from cin
    double tolerance{};
    cin >> tolerance;

    // Read points from cin
    int N{};
    cin >> N;
    
    // kör vårt program
    for (int i{0}; i < N; ++i) {
        double x{}, y{};
        cin >> x >> y;
        points.push_back(Point{x, y});
    }

    // draw points to screen all at once
    window->draw_points(points);

    auto begin = chrono::high_resolution_clock::now();
    
    /////////////////////////////////////////////////////////////////////////////
    // Draw any lines that you find in 'points' using the function 'window->draw_line'.
    /////////////////////////////////////////////////////////////////////////////
    find_line(N, points, tolerance, window);

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds.\n"
         << setprecision(2) << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()/60000.0
         << " min." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}
