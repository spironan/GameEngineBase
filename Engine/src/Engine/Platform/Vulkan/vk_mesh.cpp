#include "pch.h"

#include "Engine/Platform/Vulkan/vk_mesh.h"

#include <tiny_obj_loader.h>
#include <iostream>

VertexInputDescription Vertex::get_vertex_description()
{
    VertexInputDescription description{};

    //we will have just 1 vertex buffer binding, with a per-vertex rate
    VkVertexInputBindingDescription mainBinding{};
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(Vertex);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings.push_back(mainBinding);

    //posiiton will be stored at location 0
    VkVertexInputAttributeDescription positionAttribute{};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(Vertex, position);

    //normal will be soted at Location 1
    VkVertexInputAttributeDescription normalAttribute{};
    normalAttribute.binding = 0;
    normalAttribute.location = 1;
    normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    normalAttribute.offset = offsetof(Vertex, normal);

    //Color will be soted at Location 2
    VkVertexInputAttributeDescription colorAttribute{};
    colorAttribute.binding = 0;
    colorAttribute.location = 2;
    colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    colorAttribute.offset = offsetof(Vertex, color);

    // UV will be soted at Location 3
    VkVertexInputAttributeDescription uvAttrib{};
    uvAttrib.binding = 0;
    uvAttrib.location = 3;
    uvAttrib.format = VK_FORMAT_R32G32_SFLOAT;
    uvAttrib.offset = offsetof(Vertex, uv);

    description.attributes.push_back(positionAttribute);
    description.attributes.push_back(normalAttribute);
    description.attributes.push_back(colorAttribute);
    description.attributes.push_back(uvAttrib);

    return description;
}

bool Mesh::load_from_obj(const char *filename)
{
    // attrib will contain the vertex arrays of the file
    tinyobj::attrib_t attrib;
    // shapes contain the info for each seperate object in the file
    std::vector<tinyobj::shape_t> shapes;
    //materials contains the information about the material of each shape, but we wont be using it
    std::vector<tinyobj::material_t> materials;

    //error and warning output from the load function
    std::string warn;
    std::string err;

    //load the OBJ file
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, "C:\\Users\\J.Kong\\Desktop\\vkGuide\\vulkan-guide\\assets");
    //make sure to output the warnings to the console, in case there are issues with the file
    if (!warn.empty())
    {
        std::cout << "[Mesh loading] WARN: " << warn << std::endl;
    }
    if (!err.empty())
    {
        std::cerr << "[Mesh loading] ERROR: " << err << std::endl;
    return false;
    }

    //loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        //loop over faces (polygons)
        
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            // hardcode loading to triangles
            int fv = 3;

            // loop over vertices in the face
            for (size_t v = 0; v < fv; v++)
            {
                //access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                //vertex position
                tinyobj::real_t vx = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index )+ 0];
                tinyobj::real_t vy = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index )+ 1];
                tinyobj::real_t vz = attrib.vertices[3 * static_cast<size_t>(idx.vertex_index )+ 2];
                // vertex normal
                tinyobj::real_t nx = attrib.normals[3 * static_cast<size_t>(idx.normal_index )+ 0];
                tinyobj::real_t ny = attrib.normals[3 * static_cast<size_t>(idx.normal_index )+ 1];
                tinyobj::real_t nz = attrib.normals[3 * static_cast<size_t>(idx.normal_index )+ 2];

                // vertex UV
                tinyobj::real_t ux = attrib.texcoords[2 * static_cast<size_t>(idx.texcoord_index) + 0];
                tinyobj::real_t uy = attrib.texcoords[2 * static_cast<size_t>(idx.texcoord_index) + 1];

                //copy it into our vertex
                Vertex new_vert;
                new_vert.position.x = vx;
                new_vert.position.y = vy;
                new_vert.position.z = vz;
                
                new_vert.normal.x = nx;
                new_vert.normal.y = ny;
                new_vert.normal.z = nz;

                new_vert.uv.x = ux;
                // vulkan UV coordinates at 1-
                new_vert.uv.y = 1-uy;

                // we are setting the vertex color as the vertex normal. this just for display purposes
                new_vert.color = new_vert.normal;

                _vertices.push_back(new_vert);
            }
            index_offset += fv;
        }
    }

    return true;
}
