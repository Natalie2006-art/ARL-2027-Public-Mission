#pragma once
#include <optional>
#include <string>
#include <vector>

namespace arl {

struct RoverPose {
    double worldX;
    double worldY;
    double headingDegrees;
};

struct Detection {
    std::string id;
    double forward;
    double left;
    double confidence;
};

struct SafetyConfig {
    double minimumConfidence;
    double maximumRangeMeters;
    double laneHalfWidthMeters;
    double reactionTimeSeconds;
    double maximumDecelerationMps2;
};

struct Obstacle {
    std::string id;
    double forward;
    double left;
    double worldX;
    double worldY;
    double range;
};

std::vector<Obstacle> processDetections(
    const std::vector<Detection>& detections,
    const RoverPose& pose,
    const SafetyConfig& config);

std::optional<Obstacle> findNearestObstacle(const std::vector<Obstacle>& obstacles);

double calculateStoppingDistance(double speedKph, const SafetyConfig& config);

bool shouldEmergencyBrake(
    const std::vector<Obstacle>& obstacles,
    double speedKph,
    const SafetyConfig& config);

}  // namespace arl