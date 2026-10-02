#include "image.h"
#include "window.h"
#include "load.h"
#include "hash_map.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;

const size_t image_size = 32;
/**
 * Class that stores a summary of an image.
 *
 * This summary is intended to contain a high-level representation of the
 * important parts of an image. I.e. it shall contain what a human eye would
 * find relevant, while ignoring things that the human eye would find
 * irrelevant.
 *
 * To approximate human perception, we store a series of booleans that indicate
 * if the brightness of the image has increased or not. We do this for all
 * horizontal lines and vertical lines in a downsampled version of the image.
 *
 * See the lab instructions for more details.
 *
 * Note: You will need to use this data structure as the key in a hash table. As
 * such, you will need to implement equality checks and a hash function for this
 * data structure.
 */
class Image_Summary {
public:

   // bool operator==(Image_Summary const & rhs){
     //   if (rhs.horizontal.size() != horizontal.size()) return false;
       // for (size_t i{}; i < horizontal.size(); ++i)
         //   if (rhs.horizontal.at(i) != horizontal.at(i)) return false;
        //return true;
    //}
    bool operator==(const Image_Summary &rhs) const {
        // insåg att std::vector har redan operator== som jämför storlek och innehåll.
        return horizontal == rhs.horizontal && vertical == rhs.vertical;
    }

    // Horizontal increases in brightness.
    vector<bool> horizontal;
    // Vertical increases in brightness.
    vector<bool> vertical;
};
// Definiera en typ som specialiserar std::hash för vår typ:
//tror inte vi behöver något i template
template <>
class std::hash<Image_Summary> {
public:
// här ska vi bygga ett 64-bit tal av tru/fal
    size_t operator ()(const Image_Summary &to_hash) const {
        //iden blir 4*h[2] + 2*h[1] + h[0]
        size_t h = 0;
        // Lägg in varje bool som en bit. Eftersom vi har 72 + 72 bitar
        // (8 rader * 9 kolumner * 2) men bara 64 bitar i size_t,
        // roterar vi i stället för att bara skifta, så att ingen
        // information försvinner helt.
        //osäker dock om det är så vi ska göra
        for (bool b : to_hash.horizontal)
        //shifar alla bitar med ett och plockar den översta biten
        //sedan XOR för att läggaa till nya biten
            h = ((h << 1) | (h >> 63)) ^ static_cast<size_t>(b);

        for (bool b : to_hash.vertical)
        //måste ha 32 för att inte råka göra två 
            h = ((h << 1) | (h >> 63)) ^ (static_cast<size_t>(b) << 32);
        return h;
    }
};

// Compute an Image_Summary from an image. This is described in detail in the
// lab instructions.
Image_Summary compute_summary(const Image &image) {
    const size_t summary_size = 8;
    Image_Summary result;

    // TODO: Finish the implementation.
    // The lines below are here to avoid warnings. They can be removed.
    Image shrunken_image {image.shrink(summary_size + 1, summary_size + 1)};
    // for (const auto &pixel : image)
    //     pixel.brightness()

    //vertical
    for (size_t x{}; x < summary_size + 1; x++){
        for (size_t y{}; y < summary_size; y++){
            Pixel pixel1 {shrunken_image.pixel(x, y)};
            Pixel pixel2 {shrunken_image.pixel(x, y+1)};
            result.vertical.push_back(pixel1.brightness() >= pixel2.brightness());
        }
    }

    //horizontal
    for (size_t y{}; y < summary_size + 1; y++){
        for (size_t x{}; x < summary_size; x++){
            Pixel pixel1 {shrunken_image.pixel(x, y)};
            Pixel pixel2 {shrunken_image.pixel(x+1, y)};
            result.horizontal.push_back(pixel1.brightness() >= pixel2.brightness());
        }
    }

    return result;
}

int main(int argc, const char *argv[]) {
    WindowPtr window = Window::create(argc, argv);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [--nopause] [--nowindow] <directory>" << endl;
        cerr << "Missing directory containing files!" << endl;
        return 1;
    }

    vector<string> files = list_files(argv[1]);
    cout << "Found " << files.size() << " image files." << endl;

    if (files.size() <= 0) {
        cerr << "No files found! Make sure you entered a proper path!" << endl;
        return 1;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    /**
     * TODO:
     * - For each file: 
     *   - Load the file
     *   - Compute its summary
     */
    std::vector<Image_Summary> image_summary_vector{};
    window->show_single("Loading images...", load_image(files[0]), false);

    for (const auto &file : files)
        image_summary_vector.push_back(compute_summary(load_image(file)));
    
    Hash_Map< Image_Summary, std::vector<std::string> > map{};
    for (size_t i{}; i < files.size(); ++i){

        std::hash<Image_Summary>{}(image_summary_vector[i]);
        auto elem = map.find( image_summary_vector[i] );

        if (elem != map.end()){
            
            std::vector tmp {(*elem).second};
            tmp.push_back(files[i]);
            map.erase( image_summary_vector[i]);
            map.insert( std::make_pair(image_summary_vector[i], tmp ));
        } else {
        map.insert( std::make_pair(image_summary_vector[i], std::vector{files[i]} ));
        }
    }


    // (void)has_map;

    auto end = std::chrono::high_resolution_clock::now();
    cout << "Total time: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    for (auto &key : image_summary_vector){

        auto element = map.find(key);
        
        if (element != map.end() && (*element).second.size() > 1){
            // std::cout << "show duplicets" << std::endl;
            window->report_match((*element).second);
        }
    }

        // auto elem = map.find(i);
       
        

        // if (matches.size() > 1)
        //     window->report_match(matches);

    
    /**
     * TODO:
     * - Display sets of files with equal summaries
     */

    return 0;
}
