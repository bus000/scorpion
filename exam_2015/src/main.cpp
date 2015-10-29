#include <opencv2/highgui/highgui.hpp>
#include "worldMap.hpp"
#include "mapPresenter.hpp"
#include <cmath>

int main(int argc, char **argv){
    WorldMap map(40, 30, 10);
    MapPresenter presenter(&map);

    cv::namedWindow("window");

    map.field(4,4,true);
    map.field(8,4,true);
    map.field(8,8,true);
    map.field(4,8,true);
    //map.field(1,1,true);
    cv::Mat img = presenter.draw(Particle(12.6, 10.8, M_PI*1.25));

    while(true){
        imshow("window", img);

        cv::waitKey(30);
    }

    return 0;
}
