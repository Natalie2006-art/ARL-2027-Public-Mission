def monitor_following_distance(distances: list[float], speeds: list[float]) -> tuple[int, float, int]:
    """
    Analyzes following distance compared to safe distance (speed * 0.5).
    
    Args:
        distances (list[float]): Distance to the lead car at each second.
        speeds (list[float]): Speed of our car at each second.
        
    Returns:
        tuple[int, float, int]: (tailgating_seconds, minimum_distance, tailgate_incidents)
            - tailgating_seconds: total seconds distance was < safe distance
            - minimum_distance: absolute closest distance to the lead car (return 0.0 if empty list)
            - tailgate_incidents: number of separate instances the car started tailgating
    """

   
    if not distances:
        return (0,0.0,0)
    tailgating_seconds =0
    tailgate_incidents =0
    minimum_distance = min(distances)
    is_tailgating = False
    for speed,distance in zip(speeds,distances):
        safe_distance= speed * 0.5
        if distance>= safe_distance:
            print("Safe.")
            is_tailgating = False
        else:
            tailgating_seconds+=1
            if is_tailgating == False:
                tailgate_incidents+=1
                is_tailgating = True
    return(tailgating_seconds, minimum_distance, tailgate_incidents)
            
