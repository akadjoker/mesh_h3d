
import struct
import sys
import time
import os

import bpy
import os


from . import stream 
from .stream import ByteStream


class Color:
    def __init__(self, r=1, g=1, b=1):
        self.r = r
        self.g = g
        self.b = b
    



class Vector3:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z



    
class Vector2:
    def __init__(self,u=0, v=0):
        self.u = u
        self.v = v
    


class Face: 
    def __init__(self,a,b,c):
        self.a = a
        self.b = b
        self.c = c
    


class Material:
    def __init__(self,name="material"):
        self.name = name
        self.texture = []
        self.shininess = 0.0
        self.opacity = 1.0
        self.diffuse = Color()
        self.specular = Color()
        self.ambient = Color()

    
    def addTexture(self, name):
        if name not in self.texture:
            self.texture.append(name)



class Surface:
    def __init__(self, name, material):
        self.name = name
        self.material = material
        self.num_vertices = 0
        self.num_indices = 0
        self.stride = 3 + 3 + 2
        self.vertices = []
        self.normals = []
        self.texcoords = []
        self.indices = []

    def addVertex(self, x, y, z, nx, ny, nz, u, v):
        self.vertices.append(Vector3(x, y, z))
        self.normals.append(Vector3(nx, ny, nz))
        self.texcoords.append(Vector2(u, v))
        self.num_vertices += 1
    
    def addIndex(self, a, b, c):
        self.indices.append(a)
        self.indices.append(b)
        self.indices.append(c)
        self.num_indices += 3
    
    def scale (self, scale):
        for  vertex in self.vertices:
            vertex.x *= scale.x
            vertex.y *= scale.y
            vertex.z *= scale.z
    def translate (self, translate):

        for  vertex in self.vertices:
            vertex.x += translate.x
            vertex.y += translate.y
            vertex.z += translate.z


class Mesh:
    def __init__(self):
        self.materials = []
        self.surfaces = []
        self.stack = []
        self.stream = ByteStream()
        self.cursor = 0



    def beginChunk(self, tag):
        self.stream.write_string_size(tag, 4)
        self.stack.append(self.stream.position())
        self.stream.write_int(0)
        self.cursor+=1


    
    def endChunk(self):
        pos = self.stream.position()  # Posição atual do cursor (final do chunk)

        
        start_pos = self.stack.pop()
       
        chunk_size = pos - (start_pos + 4)
        self.stream.seek(start_pos)
        self.stream.write_int(chunk_size)  # Escreve o tamanho do chunk
        

        self.cursor -= 1
        self.stream.seek(pos)


    def addMaterial(self,name):
        material = Material(name)        
        self.materials.append(material)
        return material


    def addSurface(self, name, material):
        surface = Surface(name, material)
        self.surfaces.append(surface)
        return surface

    def Save(self, filename):
        self.stack = []
        self.stream.create(filename)
        self.stream.write_char('H')
        self.stream.write_char('3')
        self.stream.write_char('D')
        self.stream.write_char('S')
        self.stream.write_short(5)#version

        
        for material in self.materials:
            self.beginChunk('MAT#')
            self.beginChunk('TEX#')
            for texture in material.texture:
                self.stream.write_UTFstring(texture)
            self.endChunk()
            self.endChunk()

        self.beginChunk('MESH')
        
        

        for surface in self.surfaces:
            self.beginChunk('SURF')
            self.stream.write_short(surface.material)
            print("save surface: ",surface.name," material ",surface.material, " vertices ", str(len(surface.vertices)), " triangles ", str(len(surface.indices)//3))
            self.beginChunk('VRTS')
            for index in range(len(surface.vertices)):
                vertex   = surface.vertices[index]
                normal   = surface.normals[index]
                texcoord = surface.texcoords[index]
                self.stream.write_float(vertex.x)
                self.stream.write_float(vertex.y)
                self.stream.write_float(vertex.z)
                self.stream.write_float(normal.x)
                self.stream.write_float(normal.y)
                self.stream.write_float(normal.z)
                self.stream.write_float(texcoord.u)
                self.stream.write_float(texcoord.v)
                print(f"Vertex: ({vertex.x}, {vertex.y}, {vertex.z}) ({normal.x}, {normal.y}, {normal.z}) ({texcoord.u}, {texcoord.v})")

            self.endChunk()#end VRTS

            self.beginChunk('TRIS')
            for index in range(len(surface.indices)//3):
                self.stream.write_int(surface.indices[index*3])
                self.stream.write_int(surface.indices[index*3+1])
                self.stream.write_int(surface.indices[index*3+2])
            self.endChunk()#end TRIS

            self.endChunk()#end SURF

        self.endChunk()#end MESH

        print("Saved file: ", filename," size ", self.stream.position())
        self.stream.close()

    def read_chunk(self):
        chunk_id = self.stream.read_string_size(4)
        chunk_size = self.stream.read_int()
        self.stack.append(self.stream.position() + chunk_size)
        return chunk_id, chunk_size
    
    def break_chunk(self):
        end_pos = self.stack.pop()  
        self.stream.seek(end_pos)   

    def chunk_size(self):
        back = self.stack[-1]
        return back - self.stream.position()


    def Load(self, filename):
        self.stack = []
        self.stream.load(filename)
        HEADER=['0','0','0','0']
        HEADER[0] = self.stream.read_char()
        HEADER[1] = self.stream.read_char()
        HEADER[2] = self.stream.read_char()
        HEADER[3] = self.stream.read_char()
        if HEADER[0] != 'H' or HEADER[1] != '3' or HEADER[2] != 'D' or HEADER[3] != 'S':
            print("Error loading file")
            return
        
        version  = self.stream.read_short()
        print(f"Version: {version}")
        if version != 5:
            print("Error loading file")
            return

  

        while self.stream.position() < self.stream.size():
            chunk_id, chunk_size = self.read_chunk()  # Lê o identificador e o tamanho do chunk
            #print(f"Lendo chunk: {chunk_id}, tamanho: {chunk_size}")
       
            if chunk_id == 'MAT#':  
                 self.read_material_data(chunk_size)
            elif chunk_id == 'MESH':
                self.read_mesh_data(chunk_size)

            
            self.break_chunk()

    def read_material_data(self, chunk_size):
        initial_pos = self.stream.position() 
        material = self.addMaterial("NONE")
        while self.stream.position() - initial_pos < chunk_size:
            tex_chunk_id, tex_chunk_size = self.read_chunk()  
            if tex_chunk_id == 'TEX#':
                self.read_textures(material,tex_chunk_size)  


    def read_textures(self, material, chunk_size):
        initial_pos = self.stream.position()  
        while self.stream.position() - initial_pos < chunk_size:
            texture_name = self.stream.read_UTFstring()  # Lê o nome da textura
            print(f"Textura: {texture_name}")
            material.texture.append(texture_name)


          
    def read_mesh_data(self, chunk_size):
        print(f"Mesh Data: {chunk_size}")
        while self.chunk_size() > 0:
            chunk_id, chunk_size = self.read_chunk()  # Lê o identificador e o tamanho do chunk
            print(f"Lendo chunk 'mehs' {chunk_id}, tamanho: {chunk_size}")
            if chunk_id == 'SURF':

                self.read_surface_data(chunk_size)
            self.break_chunk()
       


           
            
    def read_surface_data(self, chunk_size): 
        material = self.stream.read_short()
        surface = self.addSurface("", material)
        while self.chunk_size():
            chunk_id, chunk_size = self.read_chunk()  
            if chunk_id == 'VRTS':
                self.read_vertices(surface,chunk_size)
            elif chunk_id == 'TRIS':
                self.read_indices(surface,chunk_size)            
            self.break_chunk()

    def read_vertices(self,surface, chunk_size):
        stride = (3 + 3 + 2) * 4
        size = chunk_size // stride
        index = 0
        while index < size:
            x = self.stream.read_float()
            y = self.stream.read_float()
            z = self.stream.read_float()
            nx = self.stream.read_float()
            ny = self.stream.read_float()
            nz = self.stream.read_float()
            u = self.stream.read_float()
            v = self.stream.read_float()
            surface.addVertex(x, y, z, nx, ny, nz, u, v)
            index += 1
            #print(f"Vertice: ({x}, {y}, {z}) ({nx}, {ny}, {nz}) ({u}, {v})")

    def read_indices(self,surface, chunk_size):
        stride = 3 * 4
        size = chunk_size // stride
        index = 0
        while index < size:
            a = self.stream.read_int()
            b = self.stream.read_int()
            c = self.stream.read_int()
            surface.addIndex(a, b, c)
            index += 1
            #print(f"Indice: {a}, {b}, {c}")


    def Scale(self, scale):
        for surface in self.surfaces:
            surface.scale(scale)


