
import bpy
import os
from bpy_extras import node_shader_utils
from bpy_extras.io_utils import path_reference, path_reference_copy
from typing import List

from dataclasses import dataclass
from typing import List
from random import random
from itertools import chain, islice, accumulate

import bmesh


from . import core 
from .core import Mesh,Surface,Material




def mesh_triangulate(me):
    
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()	

def create_surface(model,mesh, material_index):
    def rvec3d(v):
        return round(v[0], 6), round(v[1], 6), round(v[2], 6)

    def rvec2d(v):
        return round(v[0], 6), round(v[1], 6)

    
    mesh_triangulate(mesh)
    if not mesh.uv_layers:
        mesh.uv_layers.new(name="UVMap")
        print("Generate uvs")
    
    active_uv_layer = mesh.uv_layers.active.data
    mesh_verts = mesh.vertices
    vdict = [{} for i in range(len(mesh_verts))]
    ply_verts = []
    ply_faces = [[] for f in range(len(mesh.polygons))]

    vert_count = 0

    for i, f in enumerate(mesh.polygons):
        smooth = f.use_smooth
        if not smooth:
            normal = f.normal[:]
            normal_key = rvec3d(normal)

        uv = [
            active_uv_layer[l].uv[:]
            for l in range(f.loop_start, f.loop_start + f.loop_total)
        ]

        pf = ply_faces[i]

        for j, vidx in enumerate(f.vertices):
            v = mesh_verts[vidx]

            if smooth:
                normal = v.normal[:]
                normal_key = rvec3d(normal)

            uvcoord = uv[j][0], uv[j][1]
            uvcoord_key = rvec2d(uvcoord)

            key = normal_key, uvcoord_key

            vdict_local = vdict[vidx]
            pf_vidx = vdict_local.get(key)  # Será None inicialmente

            if pf_vidx is None:
                pf_vidx = vdict_local[key] = vert_count
                ply_verts.append((vidx, normal, uvcoord))
                vert_count += 1

            pf.append(pf_vidx)

    numVertex = len(ply_verts)
    numFaces = len(ply_faces)
    print("Malha ",mesh.name," ", numVertex, "vértices,", numFaces, "faces, material:", material_index)
    surface = model.addSurface(mesh.name,material_index)

    for i, v in enumerate(ply_verts):
        vtx = mesh_verts[v[0]].co[:]
        px, py, pz = vtx
        vtn = v[1]
        nx, ny, nz = vtn
        uvs = v[2]
        u, v = uvs
        surface.addVertex(px,py,pz,nx,ny,nz,u,v)

        print(f"Vertice: ({px}, {py}, {pz}) ({nx}, {ny}, {nz}) ({u}, {v})")

    for pf in ply_faces:
        a, b, c = pf[0], pf[1], pf[2]
        surface.addIndex(a,b,c)
        print(f"Face: {a}, {b}, {c}")

def export_h3d(filepath):
    source_dir = os.path.dirname(bpy.data.filepath)
    dest_dir = os.path.dirname(filepath)
    copy_set = set()
    selection = bpy.context.selected_objects

    model = Mesh()
    

    materials=[]
    for bmat in bpy.data.materials:
        mat_wrap = node_shader_utils.PrincipledBSDFWrapper(bmat)
        print(bmat.name)
        image_map = {
            "map_Kd": "base_color_texture",
            "map_Ks": "specular_texture",
            "map_Bump": "normalmap_texture",
        
        }
        material = model.addMaterial(bmat.name)
        for key, mat_wrap_key in image_map.items():
            if mat_wrap_key is None:
                continue
            tex_wrap = getattr(mat_wrap, mat_wrap_key, None)
            if tex_wrap is None:
                continue
            image = tex_wrap.image
            if image is None:
                continue
            texture_filepath = path_reference(image.filepath, source_dir, dest_dir, "AUTO", "", copy_set, image.library)
            material.addTexture(texture_filepath)
            print("texture:",texture_filepath)
        materials.append(material)


    for element in bpy.context.scene.objects:
        if element.type != "MESH":
            continue

        new_mesh = element.data
        material_index = 0
        material_names = [m.name for m in new_mesh.materials]
        for mat in materials:
            if mat.name in material_names:
                break
            material_index += 1
        print(f'Malha: {new_mesh.name} usando material {mat.name}, índice {material_index}')
        create_surface(model, new_mesh, material_index)
    print("Export file ", filepath)
    model.Save(filepath)

