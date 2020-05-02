//
// Created by 김태윤 on 2019-04-05.
//

#include <ObjImporter.hpp>

ObjImporter::ObjImporter() {

}

/*
 Read and parse obj file
 Parameters:
 file -> obj file name in string
 */
std::vector<glm::vec3> ObjImporter::ImportObj(std::string file) {
    std::ifstream infile(file);
    std::vector<glm::vec3> result = std::vector<glm::vec3>();
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::string line;
    // store all vertices in vertices vector
    while (std::getline(infile, line)) {
        std::regex rgx("([a-z]) ([+-]?[0-9]*.[0-9]+) ([+-]?[0-9]*.[0-9]+) ([+-]?[0-9]*.[0-9]+)");
        std::smatch m;
        if (std::regex_search(line, m, rgx)) {
            if (m[1] == "v") {
                vertices.push_back(glm::vec3(std::stof(m[2]), std::stof(m[3]), std::stof(m[4])));
            }
        }
    }
    infile.clear();
    infile.seekg(0, infile.beg);
    // add three vertices of each face in counter-clockwise order
    while (std::getline(infile, line)) {
        std::regex rgx("([a-z]) ([0-9]+) ([0-9]+) ([0-9]+)");
        std::smatch m;
        if (std::regex_search(line, m, rgx)) {
            if (m[1] == "f") {
                result.push_back(vertices[std::stoi(m[2]) - 1]);
                result.push_back(vertices[std::stoi(m[3]) - 1]);
                result.push_back(vertices[std::stoi(m[4]) - 1]);
            }
        }
    }
    return result;
}
