#include "tree.hpp"

using namespace cgp;

mesh create_cylinder_mesh(float radius, float height)
{
    int N = 20; // needs to be even
    mesh tronc;
    tronc.position.resize(N);
    for (int idx = 0; idx < N; ++idx)
    {
        float u = idx / (N - 1.0f);
        (idx % 2 == 0) ? tronc.position[idx] = vec3{radius * std::cos(u * 2 * 3.14f), radius * std::sin(u * 2 * 3.14f), 0.0f} : tronc.position[idx] = vec3{radius * std::cos(u * 2 * 3.14f), radius * std::sin(u * 2 * 3.14f), height};
    }

    for (int idx = 0; idx < N - 2; ++idx)
    {
        uint3 triangle;
        (idx % 2 == 0) ? triangle = {idx, idx + 1, idx + 2} : triangle = {idx + 2, idx + 1, idx};
        tronc.connectivity.push_back(triangle);
    }

    uint3 triangle = {N - 2, N - 1, 0};
    tronc.connectivity.push_back(triangle);
    triangle = {1, 0, N - 1};
    tronc.connectivity.push_back(triangle);

    tronc.fill_empty_field();

    return tronc;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    int N = 20;
    mesh leaves;
    leaves.position.resize(N);
    for (int idx = 0; idx < N - 1; ++idx)
    {
        float u = idx / (N - 2.0f);
        leaves.position[idx] = {radius * std::cos(u * 2 * 3.14f), radius * std::sin(u * 2 * 3.14f), z_offset};
    }
    leaves.position[N - 1] = {0.0f, 0.0f, height + z_offset};

    for (int idx = 0; idx < N - 2; ++idx)
    {
        uint3 triangle = {idx, idx + 1, N - 1};
        leaves.connectivity.push_back(triangle);
    }
    uint3 triangle = {N - 2, 0, N - 1};
    leaves.connectivity.push_back(triangle);

    leaves.fill_empty_field();
    return leaves;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.3f, 0.3f});

    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r)); // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r)); // top-cone
    foliage.translate({0, 0, h});                             // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});

    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}