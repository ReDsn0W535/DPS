
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <math.h>
#include <dps.h>
#include <cmath>
using namespace dps;
float binpow(float a, int n) {
        float res = 1.0;
        while (n) {
                if (n & 1)
                        res *= a;
                a *= a;
                n >>= 1;
        }
        return res;
}

int trash;

double DPS::countPairwiseMatchings2D(std::vector< std::pair<point, int> > *input, float q) {
        float R = 0.0f;
        for (int i = 0; i < input->size(); i++)
                for (int j = i+1; j < input->size(); j++) {
                        if ((*input)[j].first.x == (*input)[i].first.x && (*input)[j].first.y == (*input)[i].first.y) {
                                std::cout << "WARNING!!!, matching points, erase..." << std::endl;
                                (*input).erase((*input).begin() + i);
                                continue;
                        }
                        R += pow(sqrt(pow((*input)[i].first.x - (*input)[j].first.x, 2)
                                + pow((*input)[i].first.y - (*input)[j].first.y, 2)), q);

                }
        R /= ((*input).size() * ((*input).size() - 1) / 2);
        R = pow(R, 1 / q);
        return R;
};

double DPS::countRadius(std::vector< std::pair<point, int> > *input, float q) {
        std::cout << "Started counting radius..." << std::endl;
        double R = countPairwiseMatchings2D(input, q);
        std::cout << "Radius = " << R << std::endl;
        return R;
}


std::vector< std::pair<point, int> > DPS::writePoints(std::vector< std::pair<point, int> > *input) {
        std::ofstream output_stream;
        output_stream.open("C:\\Users\\GrEeNPoIsOn\\Desktop\\Crystal\\sprayedPoints2.txt");
        if (output_stream.is_open()) {
                output_stream << trash << std::endl;
                for (int i = 0; i < input->size(); i++) {
                        output_stream << "  " << (float)(*input)[i].first.x << ".0   " << (float)(*input)[i].first.y << ".0   " << (float)(*input)[i].first.z << ".0" << std::endl;
                }
        }
        return *input;
}

void DPS::readPoints(std::vector< std::pair<point, int> > *input) {

        std::ifstream input_stream("C:\\Users\\GrEeNPoIsOn\\Desktop\\Crystal\\testData2.txt");
        input_stream >> trash;
        while (!input_stream.eof())
        {
                point temp;
                input_stream >> temp.x;
                input_stream >> temp.y;
                input_stream >> temp.z;
                input->push_back(std::make_pair(temp,0));
        }
}

double np_sum(std::vector< std::pair<point, int> > *ms, double alpha){
    double temp_sum = 0.0;
    for (auto & i : *ms){
        temp_sum += ((alpha - i.second)/std::fmax(alpha, i.second));
    }
    temp_sum /= ms->size();
    return temp_sum;
}

double findAlphaFromBeta(double beta, std::vector< std::pair<point, int> > *input){
    double lv_min = 0.0, lv_max = 0.0;
    double zn_min = 0;
    double zn_max = -10000000;
    for (auto & i : *input){
        if (i.second > zn_max) zn_max = i.second;
    }
    lv_min = np_sum(input, zn_min);
    lv_max = np_sum(input, zn_max);
    while (lv_max < beta) {
        zn_max *= 2;
        lv_max=np_sum(input,zn_max);
    }
    double zn = (zn_min + zn_max) / 2;
    double lv_tmp = np_sum(input, zn);
    while (std::fabs(lv_tmp - beta) > 0.00001) {
        if (lv_tmp < beta){
            zn_min = zn;
        } else if (lv_tmp > beta){
            zn_max = zn;
        } else break;
        zn = (zn_min + zn_max) / 2;
        lv_tmp = np_sum(input,zn);
    }
    return zn;
}

void DPS::eraseForAlpha(std::vector< std::pair<point, int> > *input, double alpha) {
        for (size_t i = 0; i < input->size(); i++) {
                if ((*input)[i].second < alpha) {
                        (*input).erase((*input).begin() + i);
                }
        }
}



void DPS::findPointsInCircle(std::vector< std::pair<point, int> > *input, double alpha, float r, bool isNormalMode) {
        for (size_t i = 0; i < input->size(); i++) {
            (*input)[i].second = 0;
        }
        for (size_t i = 0; i < input->size(); i++) {
                for (size_t j = 0; j < input->size(); j++) {
                        if (i == j) continue;
                        if (pow(((*input)[j].first.x - (*input)[i].first.x), 2) +
                                pow(((*input)[j].first.y - (*input)[i].first.y), 2) < pow(r, 2)) {
                                (*input)[i].second++;
                        }
                }
        }
        int max = -100000; int min = 1000000;
        for (auto & i : *input){
            if (i.second > max) max = i.second;
            if (i.second < min) min = i.second;
        }
        std::cout << "---------------------------------------------" << max << "  " << min << std::endl;
        if(isNormalMode)eraseForAlpha(input, alpha);
}

std::vector< std::pair<point, int> > DPS::startDPS() {
        std::vector< std::pair<point, int> > points;
        std::cout << "Start reading text file with points.." << std::endl;
        readPoints(&points);
        std::cout << "Finish reading points, started DPS algoritm.." << std::endl;
        std::cout << "start size = " << points.size() << std::endl;
        unsigned int cur_size = points.size(), prev_size = -1;
        float r = countRadius(&points, -3.0f);
        double beta = 0.1;
        findPointsInCircle(&points, NULL, r, false);
        std::cout << "Starting  counting alpha for beta = " << beta << std::endl;
        double alpha = findAlphaFromBeta(beta, &points);
        std::cout << "Counting alpha finished, alpha = " << alpha << std::endl;
        while (cur_size != prev_size)
        {
                std::cout << ".............................." << std::endl;
                cur_size = points.size();
                findPointsInCircle(&points, alpha, r, true);  // alpha = 30
                prev_size = points.size();
                if (cur_size != prev_size) std::cout << "new size = " << points.size() << std::endl;
        }
        std::cout << "process finished!";
        writePoints(&points);
        return points;
}
