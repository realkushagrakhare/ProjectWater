#include "Mesh.h"

int Indexer::add(const Vector &v) {
    if (map_.count(v) <= 0) {
        map_[v] = unique.size();
        unique.push_back(v);
    }
    return map_[v];
}

Mesh::Mesh(): 
    hasCoords(false), hasNormals(false),
    vao(-1), vertex_coord_buff(-1), texture_coord_buff(-1), normal_buff(-1), tri_index_buff(-1)
{
    // TODO
}

void Mesh::compile() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertex_coord_buff);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_coord_buff);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(VSShaderLib::VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VSShaderLib::VERTEX_COORD_ATTRIB, 3, GL_FLOAT, 0, 0, 0);  // note: it is 3 for xyz !

    if (hasCoords) {
        glGenBuffers(1, &texture_coord_buff);
        glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buff);
        glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(float), &texture_coords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(VSShaderLib::TEXTURE_COORD_ATTRIB);
        glVertexAttribPointer(VSShaderLib::TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
    }

    if (hasNormals) {
        glGenBuffers(1, &normal_buff);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buff);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(VSShaderLib::NORMAL_ATTRIB);
        glVertexAttribPointer(VSShaderLib::NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
    }

    glGenBuffers(1, &tri_index_buff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_index_buff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int), &triangles[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

}

Mesh *Mesh::plane(int detailX /* = 1 */, int detailY /* = 1 */) {
    Mesh *mesh = new Mesh();
    for (int y = 0; y <= detailY; y++) {
        float t = (float)y / (float)detailY;
        for (int x = 0; x <= detailX; x++) {
            float s = (float)x / (float)detailX;
            mesh->vertices.push_back(2.f * s - 1.f);
            mesh->vertices.push_back(2.f * t - 1.f);
            mesh->vertices.push_back(0.f);
            if (x < detailX && y < detailY) {
                int i = x + y * (detailX + 1);
                mesh->triangles.push_back(i);
                mesh->triangles.push_back(i + 1);
                mesh->triangles.push_back(i + detailX + 1);
                mesh->triangles.push_back(i + detailX + 1);
                mesh->triangles.push_back(i + 1);
                mesh->triangles.push_back(i + detailX + 2);
            }
        }
    }
    mesh->compile();
    return mesh;
}

int cubeData[6][7] = {
    {0, 4, 2, 6, -1, 0, 0}, 
    {1, 3, 5, 7, +1, 0, 0},
    {0, 1, 4, 5, 0, -1, 0}, 
    {2, 6, 3, 7, 0, +1, 0}, 
    {0, 2, 1, 3, 0, 0, -1}, 
    {4, 5, 6, 7, 0, 0, +1} 
};

void pickOctant(int input, int &x, int &y, int &z) {
    x = (input & 1) * 2 - 1;
    y = (input & 2) - 1;
    z = (input & 4) / 2 - 1;
}

Mesh *Mesh::cube() {
    Mesh *mesh = new Mesh();
	
	int v = 0;
    for (int i = 0; i < 6; i++) {
		/*if (i != 3) {
			continue;
		}*/
		int *data = cubeData[i];
        for (int j = 0; j < 4; j++) {
            int d = data[j];
            int x, y, z;
            pickOctant(d, x, y, z);
            mesh->vertices.push_back(x);
            mesh->vertices.push_back(y);
            mesh->vertices.push_back(z);
			//std::cout << x << " " << y << " " << z << "\n";
        }
		//std::cout << v << std::endl;
        mesh->triangles.push_back(v);
        mesh->triangles.push_back(v + 1);
        mesh->triangles.push_back(v + 2);
        mesh->triangles.push_back(v + 2);
        mesh->triangles.push_back(v + 1);
        mesh->triangles.push_back(v + 3);
		v += 4;
    }
    mesh->compile();
    return mesh;
}

Mesh *Mesh::sphere(int detail /* = 6 */) {
    auto tri = [](bool flip, int a, int b, int c){ return flip ? std::tuple<int,int,int>(a, c, b) : std::tuple<int,int,int>(a, b, c); };
    auto fix = [](float x){ return x + (x - x * x) / 2.f; };
    Mesh *mesh = new Mesh();
    Indexer indexer;

    for (int octant = 0; octant < 8; octant++) {
        int sx, sy, sz;
        pickOctant(octant, sx, sy, sz);
        bool flip = sx*sy*sz > 0;
        std::vector<int> data;
        for (int i = 0; i <= detail; i++) {
            // Generate a row of vertices on the surface of the sphere
            // using barycentric coordinates.
            for (int j = 0; i + j <= detail; j++) {
                float a = (float)i / (float)detail;
                float b = (float)j / (float)detail;
                float c = float(detail - i - j) / (float)detail;
                float vx = fix(a), vy = fix(b), vz = fix(c);
                double invlen = 1.0 / sqrt(double(vx*vx + vy*vy + vz*vz));
                vx *= (invlen*sx); vy *= (invlen*sy); vz *= (invlen*sz);
                // TODO: coords
                data.push_back(indexer.add(Vector(vx,vy,vz)));
            }

            // Generate triangles from this row and the previous row.
            if (i > 0) {
                for (int j = 0; i + j <= detail; j++) {
                    int a = (i - 1) * (detail + 1) + ((i - 1) - (i - 1) * (i - 1)) / 2 + j;
                    int b = i * (detail + 1) + (i - i * i) / 2 + j;
                    std::tuple<int,int,int> t = tri(flip, data[a], data[a + 1], data[b]);
                    mesh->triangles.push_back(std::get<0>(t));
                    mesh->triangles.push_back(std::get<1>(t));
                    mesh->triangles.push_back(std::get<2>(t));
                    if (i + j < detail) {
                        std::tuple<int,int,int> t2 = tri(flip, data[b], data[a + 1], data[b + 1]);
                        mesh->triangles.push_back(std::get<0>(t2));
                        mesh->triangles.push_back(std::get<1>(t2));
                        mesh->triangles.push_back(std::get<2>(t2));
                    }
                }
            }
        }
    }

    // Reconstruct the geometry from the indexer.
    for (int i = 0; i < indexer.unique.size(); i++) {
        auto &v = indexer.unique[i];
        mesh->vertices.push_back(v.x);
        mesh->vertices.push_back(v.y);
        mesh->vertices.push_back(v.z);
    }

    mesh->compile();
    return mesh;
}
