#ifndef DPS_H
#define DPS_H
#include <vector>
namespace dps {
struct point
{
        float x;
        float y;
        float z;
};


class DPS{
public:
    DPS() = default;

    std::vector< std::pair<point, int> > startDPS();
    void readPoints(std::vector< std::pair<point, int> > *input);

    ~DPS() = default;
private:
    double countPairwiseMatchings2D(std::vector< std::pair<point, int> > *input, float q);
    double countRadius(std::vector< std::pair<point, int> > *input, float q);
    std::vector< std::pair<point, int> > writePoints(std::vector< std::pair<point, int> > *input);
    void findPointsInCircle(std::vector< std::pair<point, int> > *input, double alpha, float r, bool isNormal);
    void eraseForAlpha(std::vector< std::pair<point, int> > *input, double alpha);
};


}


#endif // DPS_H
