import bpy

# Deselect all objects in the scene
bpy.ops.object.select_all(action='DESELECT')

# Dictionary to group objects by material name
material_groups = {}

# Step 1: Organize all mesh objects by their active material
for obj in list(bpy.context.scene.objects):  # Copy list to avoid accessing removed objects
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
        bpy.context.view_layer.objects.active = objs[0]
        
        # Select all objects in this material group
        for obj in objs:
            try:
                obj.select_set(True)
            except ReferenceError:
                # Skip any object that has been removed
                continue
        
        # Combine selected objects into one using the join operator
        bpy.ops.object.join()
        
        # Deselect all objects after combining
        bpy.ops.object.select_all(action='DESELECT')

# Step 3: Remove any unused materials from the scene
for mat in bpy.data.materials:
    # Check if material has no users and remove it
    if not mat.users:
        bpy.data.materials.remove(mat)

print("Meshes with the same material have been combined and unused materials removed.")

