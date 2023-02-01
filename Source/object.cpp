//
// Created by linkinpony on 23-1-9.
//

#include "object.h"
#include <tiny_obj_loader.h>
#include <iostream>
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust triangulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"
#endif

int Object::readObjFromFile(const std::string& filename){
  //return 0 if read complete.
  //1 - object have no-triangle faces
  //TODO: support other face

  const std::string& inputfile = filename;
  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "./"; // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
      std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
  }

  if (!reader.Warning().empty()) {
    std::cerr << "TinyObjReader: " << reader.Warning();
  }

  auto& attrib = reader.GetAttrib();
  auto& shapes = reader.GetShapes();
  auto& materials = reader.GetMaterials();
  this->triangles_.reserve(shapes.size());

// Loop over shapes
  for (const auto & shape : shapes) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
      auto fv = size_t(shape.mesh.num_face_vertices[f]);
      if(fv != 3){
        //not triangle
        return 1;
      }
      Triangle temp_triangle;
      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
        tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
        tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];
        temp_triangle.v_[v] = Eigen::Vector4f (vx,vy,vz,1.0f);
        // Check if `normal_index` is zero or positive. negative = no normal data
        if (idx.normal_index >= 0) {
          tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
          tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
          tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
          temp_triangle.normal_[v] = Eigen::Vector3f (nx,ny,nz);
        }

        // Check if `texcoord_index` is zero or positive. negative = no texcoord data
        if (idx.texcoord_index >= 0) {
          tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
          tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
          temp_triangle.texture_coords_[v] = Eigen::Vector2f (tx,ty);
        }

        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
      }
      index_offset += fv;

      // per-face material
      temp_triangle.material_id_ =  shape.mesh.material_ids[f];
      triangles_.push_back(temp_triangle);
    }
  }
  return 0;
}
std::unique_ptr<TGAImage> Object::readTGA(const std::string &filename) {
  //TODO: avoid copy
  std::unique_ptr<TGAImage> temp = std::make_unique<TGAImage>(TGAImage());
  temp->read_tga_file(filename.c_str());
  return temp;
}
int Object::readDiffusemap(const std::string &filename) {
  this->diffuse_map_ = readTGA(filename + FILE_DIFFUSE_SUFFIX_);
  return 1;
}
int Object::readNormalmap(const std::string &filename) {
  this->normal_map_ = readTGA(filename + FILE_NORMAL_MAP_SUFFIX_);
  return 1;
}
