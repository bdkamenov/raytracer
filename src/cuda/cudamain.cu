__device__ Color raytrace(const Ray& ray)
{
    // we use double for vectors, rays and so on and floats for colors
    const Node* closestNode = nullptr;
    double closestDist = INF;
    IntersectionInfo closestInfo;

    for (const auto& node : nodes)
    {
        IntersectionInfo info;
        if (!node._geometry->intersect(ray, info))
        {
            continue;
        }

        if (info._distance < closestDist)
        {
            closestDist = info._distance;
            closestNode = &node;
            closestInfo = info;
        }
    }

    // check if we hit the sky
    if (closestNode == nullptr)
    {
        return Color(0.f, 0.f, 0.f); // background color
    }
    else
    {
        return closestNode->_shader->shade(ray, closestInfo);
    }
}

__global__ void render(int width, int height)
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            Ray ray = camera.getScreenRay(x, y);
            vfb[y][x] = raytrace(ray);
        }
}