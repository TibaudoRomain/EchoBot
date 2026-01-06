#ifndef MARKER_STRUCTURE_H
#define MARKER_STRUCTURE_H

#endif // MARKER_STRUCTURE_H

struct Marker
{
    int id;
    int image_x;
    int image_y;

    int distance;

    int world_x;
    int world_y;
    int world_z;

    int yaw;
    int pitch;
    int roll;

    Marker(int id, int image_x, int image_y, int distance, int world_x, int world_y, int world_z, int yaw, int pitch, int roll) :
        id(id),
        image_x(image_x),
        image_y(image_y),
        distance(distance),
        world_x(world_x),
        world_y(world_y),
        world_z(world_z),
        yaw(yaw),
        pitch(pitch),
        roll(roll)
    {}


};
