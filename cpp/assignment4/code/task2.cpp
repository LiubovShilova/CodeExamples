
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

bool not_enough_stations(int number_beaming, int n, int k){
//return true if in principle given number of stations
// would not be enough to cover all cities
    return (number_beaming<(n/std::ceil(2*k+1)));
}

int number_of_needed(int n, int k, std::vector<bool> has_station){
    /*
     * Input: number of stations (beaming s. or catapults) n, how far do they throw k,
     * and bool vector, representing if a city has a station
     * Returns: #needed stations, -1 if there are not enough.
     */

    int i = 0; //first city after beginning/turned on station
    int needed = 0; //#needed stations
    int covers_last = 0; //index of last covered station

    //Each time we look at first city which is not covered so far
    // and choose the furthest station which could cover it
    while(i<n) {
        int turn_on_st = -1; //index of station that is turned on
        int boarder = std::min(covers_last + k, n - 1); //the last city that can be chosen

        //for first city after beginning/turned on station go through cities,
        // stations in which could cover the former. Choose the furthest.
        for (int j = i; j <= boarder; j++) {
            if (has_station[static_cast<unsigned int>(j)]) {
                turn_on_st = j;
            }
        }
        //if the station, which can cover it exists
        if (turn_on_st != -1) {
            needed++;                        //update the number of needed stations
            covers_last = turn_on_st + k;    //update the index of the last city that is covered
            if (covers_last >= n - 1) { break; } //bigger than index of last existing city: we're done

            //if no station can cover the city
        } else {
            return -1;
        }

        //update first city after beginning/turned on station
        i = turn_on_st + 1;
    }
    return needed;
}

int main() {

    //read the input checking its validity on the way
    int n = 0;
    int k = 0;
    int l = 0;

    //write input in int variables
    if(!(std::cin >> n >> k >> l)){
        std::cerr<<"Invalid Input\n";
        return 1;
    };

    //vectors representing if the city has BS/cata station
    std::vector<bool> has_beaming_station;
    std::vector<bool> has_catapult;

    //overall number of possible stations
    int number_beaming = 0;
    int number_catapult = 0;

    //read the status of each city: beaming st/catapult/nothing + check
    for (int i = 0; i<n; i++) {
        int status = 0;
        //for each city append true or false to respective vector, update #existing stations
        if (std::cin >> status) {
            if (status == 0) {
                has_beaming_station.push_back(false);
                has_catapult.push_back(false);
            } else if (status == 1) {
                has_beaming_station.push_back(true);
                has_catapult.push_back(false);
                number_beaming++;
            } else if (status == 2) {
                has_beaming_station.push_back(false);
                has_catapult.push_back(true);
                number_catapult++;
            } else {
                std::cerr << "Status of the city should be given as 0, 1 or 2 only\n";
                return 1;
            }
        } else {
            std::cerr << "Invalid Input\n";
            return 1;
        }
        //if not enough data in file
        if ((i < n - 1) && (std::cin.eof())) {
            std::cerr << "Please enter n, k, l and then status of each city\n";
            return 1;
        }
    }

    //if we have less beamers than necessary
    if (not_enough_stations(number_beaming,n,k)){
        std::cout<<"beamer:-1\n";
    }
        //consider the positions, calculate the number
    else{
        std::cout<<"beamer:"<<number_of_needed(n, k, has_beaming_station)<<"\n";
    }

    //if we have less catapults than necessary
    if (not_enough_stations(number_catapult, n, l)){
        std::cout<<"cata:-1\n";
    }
        //consider the positions, calculate the number
    else{
        std::cout<<"cata:"<<number_of_needed(n, l, has_catapult)<<"\n";
    }
    return 0;
}
