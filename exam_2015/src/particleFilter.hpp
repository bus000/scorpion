#include <vector>
#include "particle.hpp"
#include "camera.hpp"
#include "worldMap.hpp"

class ParticleFilter {
public:
    ParticleFilter(std::vector<Particle> *particles, WorldMap *map,
            double fallbackDistVar, double fallbackAngleVar);
    void filter(Measurement measurement, Particle command = Particle(0,0));
    void addRandomParticles(int limit);

private:
    std::vector<Particle> *_particles;
    std::vector<double> _angleWeights;
    std::vector<double> _distanceWeights;
    double _fallbackDistVar;
    double _fallbackAngleVar;
    std::vector<double> measurements;
    double _distanceStdVariance;
    double _angleStdVariance;
    WorldMap *_map;

    static double GaussianDist(double x, double sigma, double my);
    void updateVariance(Measurement measurement);
    void dynamicModel(Particle command);
    void observationModel(Measurement measurement);
    void resample(int limit);
    void resetWeights();
    void mergeAndNormalizeWeights();
};
