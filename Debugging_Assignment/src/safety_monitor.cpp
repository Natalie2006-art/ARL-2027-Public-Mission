#include "safety_monitor.hpp"
#include <optional> // mistake was not including this standard header
#include <cmath>
#define _USE_MATH_DEFINES
namespace arl {
namespace {

bool isFinite(const Detection& detection) {
    return std::isfinite(detection.forward)
        && std::isfinite(detection.left)
        && std::isfinite(detection.confidence);
}

}  // namespace

const double m_PI = 3.14159265358979323846; // Define M_PI if not defined

std::vector<Obstacle> processDetections(
    const std::vector<Detection>& detections,
    const RoverPose& pose,
    const SafetyConfig& config) {
    std::vector<Obstacle> obstacles;
    const double headingRadians = pose.headingDegrees * m_PI / 180.0; //mistake was not coverting degrees to radians
    const double cosine = std::cos(headingRadians);
    const double sine = std::sin(headingRadians);
//+1 in indexing
    for (std::size_t index = 0; index < detections.size(); ++index) {
        const auto& detection = detections[index];
        const double range = std::hypot(detection.forward, detection.left);
        const bool validConfidence = detection.confidence >= 0.0
            && detection.confidence >= config.minimumConfidence; // greater than or equal to minimumConfidence
        const bool validRange = range > 0.0 && range <= config.maximumRangeMeters;

        if (!isFinite(detection) || !validConfidence || !validRange) {
            continue;
        }

        obstacles.push_back({
            detection.id,
            detection.forward,
            detection.left,
            pose.worldX + cosine * detection.forward - sine * detection.left,// -ve sign for left coordinate
            pose.worldY + sine * detection.forward + cosine * detection.left,
            range,
        });
    }

    return obstacles;
}

std::optional<Obstacle> findNearestObstacle(const std::vector<Obstacle>& obstacles) {
    if (obstacles.empty()) {
        return std::nullopt;
    }

    const Obstacle* nearest = &obstacles.front();
    for (const auto& obstacle : obstacles) {
        if (obstacle.range < nearest->range) {
            nearest = &obstacle;
        }
    }
    // smaller than  nearest->range
    return *nearest;
}

double calculateStoppingDistance(double speedKph, const SafetyConfig& config) {
    const double speedMps = speedKph * 1000.0 / 3600.0; // Convert km/h to m/s
    const double reactionDistance = speedMps * config.reactionTimeSeconds;
    const double brakingDistance = speedMps * speedMps
        / (2.0 * config.maximumDecelerationMps2);
    return reactionDistance + brakingDistance;
}

bool shouldEmergencyBrake(const std::vector<Obstacle>& obstacles, double speedKph, const SafetyConfig& config) {
    
auto nearest = findNearestObstacle(obstacles);
    if (!nearest.has_value()) return false;
    double stopDist = calculateStoppingDistance(speedKph, config);
    return (nearest->range <= stopDist);

}
// problem in emergency brake logic
}  // namespace arl