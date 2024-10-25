
import sys
import struct
import array

import bpy
import os

from . import core 
from .core import Mesh
# import importlib

# if "core" in locals():
#     importlib.reload(core)





def import_h3d(modelToLoad, texture_path=None):

    f_name, f_ext = os.path.splitext(modelToLoad)
    meshName =os.path.basename(f_name)
    #modelPath = os.path.dirname(os.path.abspath(modelToLoad))
    print(f"Modelo: {modelToLoad}")

    new_collection = bpy.data.collections.new(meshName)
    bpy.context.scene.collection.children.link(new_collection)
    BlenderMaterials=[]

    mesh = core.Mesh()
    mesh.Load(modelToLoad)

    index = 0
    for material in mesh.materials:
        matName=meshName+"_mat"+str(index)
        index += 1
        print("Material: ",matName)
        #bmat = bpy.data.materials.get(matName)
        bmat = bpy.data.materials.new(name=matName)
        bmat.use_nodes = True
        for texture in material.texture:
            textureFileName=texture_path+texture
            base_name, ext = os.path.splitext(texture)
            normal_texture   = f"{base_name}_normal{ext}"  
            specular_texture = f"{base_name}_specular{ext}"  
            
            if os.path.exists(textureFileName):
                texImage = bmat.node_tree.nodes.new('ShaderNodeTexImage')
                texImage.image = bpy.data.images.load(textureFileName, check_existing=True)
                bsdf = bmat.node_tree.nodes["Principled BSDF"]
                print('Load TEXTURE ',textureFileName)
                bmat.node_tree.links.new(bsdf.inputs['Base Color'], texImage.outputs['Color'])
                if os.path.exists(texture_path+normal_texture):
                    texNormal = bmat.node_tree.nodes.new('ShaderNodeTexImage')
                    texNormal.image = bpy.data.images.load(texture_path+normal_texture, check_existing=True)
                    bmat.node_tree.links.new(bsdf.inputs['Normal'], texNormal.outputs['Color'])
                if os.path.exists(texture_path+specular_texture):
                    texSpecular = bmat.node_tree.nodes.new('ShaderNodeTexImage')
                    texSpecular.image = bpy.data.images.load(texture_path+specular_texture, check_existing=True)
                    bmat.node_tree.links.new(bsdf.inputs['Specular'], texSpecular.outputs['Color'])
            else:
                print("TEXTURE",textureFileName,' dont exists in path')

        BlenderMaterials.append(bmat)


    create_blender_mesh(new_collection,mesh, BlenderMaterials)



def create_blender_mesh(collection,mesh, materials):
    index = 0
    for surface in mesh.surfaces:
        name = "surface_"+str(index)
        index += 1
        uv_coorsList =[]
        contextMesh_vertls = []
        contextMesh_normsls = []
        contextMesh_facels = []
        
        for index in range(len(surface.vertices)):
            contextMesh_vertls.append(surface.vertices[index].x)
            contextMesh_vertls.append(surface.vertices[index].y)
            contextMesh_vertls.append(surface.vertices[index].z)

            contextMesh_normsls.append(surface.normals[index].x)
            contextMesh_normsls.append(surface.normals[index].y)
            contextMesh_normsls.append(surface.normals[index].z)

            uv_coorsList.append(surface.texcoords[index].u)
            uv_coorsList.append(surface.texcoords[index].v)
        

        for index in range(len(surface.indices)//3):
            v1 = surface.indices[index*3]
            v2 = surface.indices[index*3+1]
            v3 = surface.indices[index*3+2]
            contextMesh_facels.append((v1,v2,v3))


            

            

	

        new_mesh = bpy.data.meshes.new(name)
        print("Set material : ",surface.material,len(materials))
        if (surface.material < len(materials)):
            new_mesh.materials.append(materials[surface.material])
     




		
        new_mesh.vertices.add(len(contextMesh_vertls) // 3)
        new_mesh.vertices.foreach_set("co", contextMesh_vertls)

        nbr_faces = len(contextMesh_facels)
        new_mesh.polygons.add(nbr_faces)
        new_mesh.loops.add(nbr_faces * 3)
        eekadoodle_faces = []
        for v1, v2, v3 in contextMesh_facels:
            eekadoodle_faces.extend((v3, v1, v2) if v3 == 0 else (v1, v2, v3))


        new_mesh.polygons.foreach_set("loop_start", range(0, nbr_faces * 3, 3))
        new_mesh.polygons.foreach_set("loop_total", (3,) * nbr_faces)
        new_mesh.loops.foreach_set("vertex_index", eekadoodle_faces)

        uv_layer=(new_mesh.uv_layers.new())

        for face in new_mesh.polygons:
            for vert_idx, loop_idx in zip(face.vertices, face.loop_indices):
                uv_layer.data[loop_idx].uv =( uv_coorsList[vert_idx * 2 + 0],uv_coorsList[vert_idx * 2 + 1] )

        new_mesh.uv_layers.active = uv_layer


        new_mesh.validate()
        #new_mesh.calc_normals()
        new_mesh.update(calc_edges=True)
        new_object = bpy.data.objects.new('obj_'+str(index), new_mesh)
        collection.objects.link(new_object)

