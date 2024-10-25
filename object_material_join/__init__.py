import bpy



bl_info = {
    "name": "Combine Meshes by Material",
    "description": "Combine all meshes that share the same material and remove unused materials",
    "author": "Luís Santos aka DJOKER",
    "version": (1, 0),
    "blender": (4, 1, 0),
    "location": "Object > Combine Meshes by Material",
    "category": "Object",
}


class OBJECT_OT_combine_meshes_by_material(bpy.types.Operator):
    bl_idname = "object.combine_meshes_by_material"
    bl_label = "Combine Meshes by Material"
    bl_description = "Combine all meshes that share the same material and remove unused materials"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # Deselect all objects in the scene
        bpy.ops.object.select_all(action='DESELECT')


        material_groups = {}

        # Step 1: Organize all mesh objects by their active material
        for obj in list(context.scene.objects):  # Copy list to avoid accessing removed objects
            if obj.type == 'MESH' and obj.active_material:
                # Get the name of the material
                mat = obj.active_material.name
                # If the material is not in the dictionary, add it
                if mat not in material_groups:
                    material_groups[mat] = []
                # Add the object to the list for this material
                material_groups[mat].append(obj)

        # Step 2: Combine objects in each group that share the same material
        for mat, objs in material_groups.items():
            if len(objs) > 1:
                # Set the first object in the group as the active object
                context.view_layer.objects.active = objs[0]
                
                # Select all objects in this material group
                for obj in objs:
                    try:
                        obj.select_set(True)
                    except ReferenceError:
                        # Skip any object that has been removed
                        continue
                
                # Combine selected objects into one using the join operator
                bpy.ops.object.join()
                bpy.ops.object.select_all(action='DESELECT')

        # Step 3: Remove any unused materials from the scene
        for mat in bpy.data.materials:
            # Check if material has no users and remove it
            if not mat.users:
                bpy.data.materials.remove(mat)

        self.report({'INFO'}, "Meshes with the same material have been combined and unused materials removed.")
        return {'FINISHED'}

def menu_func(self, context):
    self.layout.operator(OBJECT_OT_combine_meshes_by_material.bl_idname)


def register():
    bpy.utils.register_class(OBJECT_OT_combine_meshes_by_material)
    bpy.types.VIEW3D_MT_object.append(menu_func)

def unregister():
    bpy.utils.unregister_class(OBJECT_OT_combine_meshes_by_material)
    bpy.types.VIEW3D_MT_object.remove(menu_func)

if __name__ == "__main__":
    register()

