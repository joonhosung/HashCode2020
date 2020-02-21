#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


struct library{
    int libIndex;     // Index of the library
    int numLibBooks;  // Number of books in the library
    int numDaySignUp; // Number of days to sign up
    int numBookShip;  // Number of books we can ship in 1 day
    int booksToShip = 0;  // Number of books we want to ship 
    double libScore;  // Calculated score of the library
    vector<bool> books; // Vector of all books in the library (true if exists and want to send)
};

// Compare "Importance" of two libraries by score

bool compare(library const& lhs, library const& rhs) { return lhs.libScore > rhs.libScore ;}



int main() {
    string fileNames[6] = {"a_example.txt", "b_read_on.txt", "c_incunabula.txt", "d_tough_choices.txt", "e_so_many_books.txt", "f_libraries_of_the_world.txt"};
    
    for(int index = 0; index < 6; index++){
        ifstream file(fileNames[index]);
        int numBooks, numLibraries, numDays;

        file >> numBooks >> numLibraries >> numDays;
        vector<pair<int,int>> bookScores(numBooks);

        // Get all book score into array of ints
        for(int i = 0; i < numBooks; i++){
            bookScores[i].first = i;
            file >> bookScores[i].second;
        }

        sort(bookScores.begin(), bookScores.end(), [](auto &left, auto &right) {return left.second > right.second; });

        //cout << bookScores[0].second << " " << bookScores[numBooks-1].second << endl;
        //cout << bookScores[0].first << " " << bookScores[numBooks-1].first << endl;

        // Initialize array of libraries!
        library libraries[numLibraries];


        for(int i = 0; i < numLibraries; i++){
            libraries[i].libIndex = i;
            file >> libraries[i].numLibBooks;
            file >> libraries[i].numDaySignUp;
            file >> libraries[i].numBookShip;

            libraries[i].libScore = (double)(libraries[i].numBookShip) / libraries[i].numDaySignUp;
            libraries[i].books.resize(numBooks);

            // Initialize as false!
            for(int j = 0; j < numBooks; j++){
                libraries[i].books[j] = false;
            }

            // Put in books that libraries have
            for(int k = 0; k < libraries[i].numLibBooks; k++){
                int temp;
                file >> temp;

                libraries[i].books[temp] = true;
            }
        }
        std::cout << "Total number of books: " << numBooks << "\nTotal number of libraries: " << numLibraries << "\nNumber of days available: " << numDays << endl;
    
        // Sort libraries by their scores
        sort(libraries, libraries+numLibraries, compare);

        
        vector<bool> sentBooks(numBooks, false);    // Initialize vector of books already sent out (initially none!)
        bool includeLibrary[numLibraries];
        for(int i = 0; i < numLibraries; i++){ includeLibrary[i] = true;}
        int libraryCount = numLibraries;

        // Ship books!
        for(int i = 0; i < numLibraries; i++){

            for(int j = 0; j < numBooks; j+=(i%3+1)){
                if(libraries[i].books[bookScores[j].first] && !(sentBooks[bookScores[j].first])){ // If first time sending book
                    libraries[i].booksToShip ++;
                    sentBooks[bookScores[j].first] = true;
                }
                else if(libraries[i].books[bookScores[j].first] && sentBooks[bookScores[j].first]){
                    libraries[i].books[bookScores[j].first] = false; //if book already sent, don't send!
                }
            }

            if(libraries[i].booksToShip == 0){
                //cout << "setting library #" << libraries[i].libIndex << " to false" << endl; 
                includeLibrary[i] = false;
                libraryCount --;
            }
            
        }



        // Write to file!
        ofstream ofile("out_" + fileNames[index]);
        ofile << libraryCount << "\n"; 

        for(int i = 0; i < numLibraries; i++){
            //cout << includeLibrary[i] << endl;
            if(includeLibrary[i]){
                //cout << "Library #" << libraries[i].libIndex << ", books to ship: " << libraries[i].booksToShip << endl;

                ofile << libraries[i].libIndex << " " << libraries[i].booksToShip << "\n";

                for(int j = 0; j < numBooks; j++){
                    if(libraries[i].books[bookScores[j].first]){
                        ofile << bookScores[j].first << " ";
                    }
                }
                ofile << "\n";
            }
        }
        
        ofile.close();
        file.close();
        std::cout << "Done for: " << fileNames[index] << endl;
    }
}